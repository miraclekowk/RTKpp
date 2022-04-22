//
// Created by nins on 2022/3/11.
//

#include "rtk_response.h"


#define TIMEOUT_DEFAULT 500


rtk_response::rtk_response(int fd) {
    this->fd = fd;
    //默认不断开
    this->keep_alive = 1;
    this->modified = 1;
    //默认状态码200，出错则修改
    this->status = 200;
}

rtk_response::~rtk_response() {

}

//headers形如  Connection:keep-alive
int rtk_response::rtk_http_ignore(){return 0;};

int rtk_response::rtk_http_connection(rtk_request* rq,std::string value){this->keep_alive = (value == "keep-alive")?1:0;return 0;}

int rtk_response::rtk_http_if_modified(rtk_request* rq,std::string value){
    struct tm tm;
    //c++的string转换成char*以适配strptime接口
    if(strptime(value.c_str(),"%a %d %b %Y %H:%M:%S GMT",&tm) == (char*)NULL){
        return 0;
    }

    time_t client_time = mktime(&tm);;
    double time_diff = difftime(this->mtime, client_time);
    //时间间隔小于10^-6则认为没有修改过
    if(fabs(time_diff) < 1e-6){
        this->modified = 0;
        this->status = RTK_HTTP_NOT_MODIFIED;
    }
    return 0;
}

//这里直接查map，相比handle控制效率高一些
void rtk_response::rtk_http_handle_head(rtk_request* rq) {
    if(rq->head_list.find("Host")!= rq->head_list.end()){
        rtk_http_ignore();
    }else if(rq->head_list.find("Connection") != rq->head_list.end()){
        rtk_http_connection(rq,rq->head_list["Connection"]);
    }else if(rq->head_list.find("If_Modified_Since") != rq->head_list.end()){
        rtk_http_if_modified(rq,rq->head_list["If_Modified_Since"]);
    }else{
        rtk_http_ignore();
    }
}

std::string rtk_response::get_shortmsg_from_status_code(int status_code){
    if(status_code == RTK_HTTP_OK){
        return "OK";
    }else if(status_code == RTK_HTTP_NOT_MODIFIED){
        return "Not Modified";
    }else if(status_code == RTK_HTTP_NOT_FOUND){
        return "Not Found";
    }
    return "Unknow";
}

void rtk_response::do_error(int fd, char* cause, char* err_num, char* short_msg,char* long_msg) {
    //为了使用unix的write(其第二个参数是const void* buf)，所以这里也建buf数组
    char header[MAXLINE];
    char body[MAXLINE];

    // 用log_msg和cause字符串填充错误响应体
    sprintf(body, "<html><title>RTKpp Error<title>");
    sprintf(body, "%s<body bgcolor=""ffffff"">\n", body);
    sprintf(body, "%s%s : %s\n", body, err_num, short_msg);
    sprintf(body, "%s<p>%s : %s\n</p >", body, long_msg, cause);
    sprintf(body, "%s<hr><em>TKeed web server</em>\n</body></html>", body);

    //返回错误码，组织错误响应头
    sprintf(header, "HTTP/1.1 %s %s\r\n", err_num, short_msg);
    sprintf(header, "%sServer: TKeed\r\n", header);
    sprintf(header, "%sContent-type: text/html\r\n", header);
    sprintf(header, "%sConnection: close\r\n", header);
    sprintf(header, "%sContent-length: %d\r\n\r\n", header, (int)strlen(body));

    //发送错误信息
    //rio_writen(fd, header, strlen(header));
    //rio_writen(fd, body, strlen(body));
    return;
}

int rtk_response::error_file_path(struct stat* sbufptr,std::string filename,int fd){
    //stat() return information about a file, in the buffer pointed to by statbuf.
    if(stat(filename.c_str(),sbufptr) < 0){
        this->do_error(fd, const_cast<char *>(filename.c_str()), "404", "Not Found", "RTKpp server can not found this file");
        return 1;
    }

    //权限问题  S_IRUSR is a mask value
    if(!(S_ISREG(sbufptr->st_mode)) || !(S_IRUSR & (sbufptr->st_mode) )){
        this->do_error(fd,const_cast<char *>(filename.c_str()),"403","Fobidden","RTKpp have no authority to read this file.");
        return 1;
    }

        return 0;
}


//处理静态文件(主要是读取)
void rtk_response::serve_static(rtk_request* rq, std::string filename, size_t filesize) {
    char header[MAXLINE];
    char buff[MAXLINE];
    struct tm tm;

    //返回响应报文头，包含HTTP版本号状态码及状态码对应的短描述
    sprintf(header, "HTTP/1.1 %d %s\r\n", this->status, rtk_response::get_shortmsg_from_status_code(this->status).c_str());

    //返回响应头
    //Connection，Keep-Alive，Content-type，Content-length，Last-Modified
    if(this->keep_alive){
        // 返回默认的持续连接模式及超时时间（默认500ms）
        sprintf(header, "%sConnection: keep-alive\r\n", header);
        sprintf(header, "%sKeep-Alive: timeout=%d\r\n", header, TIMEOUT_DEFAULT);
    }
    //修改过
    if(this->modified){
        // 得到文件类型并填充Content-type字段
        const char* filetype =   rq->req_file_type.c_str();
        sprintf(header, "%sContent-type: %s\r\n", header, filetype);
        // 通过Content-length返回文件大小
        sprintf(header, "%sContent-length: %zu\r\n", header, filesize);
        // 得到最后修改时间并填充Last-Modified字段
        localtime_r(&(this->mtime), &tm);
        strftime(buff, SHORTLINE,  "%a, %d %b %Y %H:%M:%S GMT", &tm);
        sprintf(header, "%sLast-Modified: %s\r\n", header, buff);
    }
    sprintf(header, "%sServer : RTKpp\r\n", header);

    //空行（must）
    sprintf(header, "%s\r\n", header);

    //发送报文头部并校验完整性
    size_t send_len = (size_t)rtk_io_writen(fd, header, strlen(header));
    if(send_len != strlen(header)){
        perror("Send header failed");
        return;
    }

    //发送文件主体
    int request_file_fd = open(filename.c_str(),O_RDONLY,0);
    //利用mmap函数完成映射，加快处理速度
    void* src_addr  = mmap(NULL,filesize,PROT_READ,MAP_PRIVATE,request_file_fd,0);
    close(request_file_fd);

    send_len = rtk_io_writen(fd,(char*)src_addr,filesize);
    if(send_len != filesize){
        perror("Send file failed");
        return;
    }
    munmap(src_addr,filesize);
}

#ifdef DEBUG_MODE

TEST(TestCase,test4_do_request){
    rtk_request rq("../");
    const int maxbuf = 6666;
    char buf[] ="GET / HTTP/1.1\n"
                "Host: 127.0.0.1:3000\n"
                "Connection: keep-alive\n"
                "Cache-Control: max-age=0\n"
                "sec-ch-ua: \" Not A;Brand\";v=\"99\", \"Chromium\";v=\"98\", \"Google Chrome\";v=\"98\"\n"
                "sec-ch-ua-mobile: ?0\n"
                "sec-ch-ua-platform: \"Linux\"\n"
                "Upgrade-Insecure-Requests: 1\n"
                "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/98.0.4758.102 Safari/537.36\n"
                "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\n"
                "Sec-Fetch-Site: none\n"
                "Sec-Fetch-Mode: navigate\n"
                "Sec-Fetch-User: ?1\n"
                "Sec-Fetch-Dest: document\n"
                "Accept-Encoding: gzip, deflate, br\n"
                "Accept-Language: zh-CN,zh;q=0.9\n"
                "If-Modified-Since: Sun, 06 Mar 2022 16:03:28 GMT\r\n";
    int fp = open("/home/nins/Code/test4.txt",O_RDWR|O_CREAT);
    rq.fd = fp;
    //char rbuf[maxbuf] = "";
    //int len = read(rq.fd,rbuf,maxbuf);

    rtk_response rsp(rq.fd);
    //rsp.do_request(rq,_);

    EXPECT_EQ(maxbuf,rq.last);
}

#endif