//
// Created by nins on 2022/3/11.
//

#include "rtk_response.h"


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

int rtk_response::rtk_http_connection(rtk_request rq,std::string value){this->keep_alive = (value == "keep-alive")?1:0;return 0;}

int rtk_response::rtk_http_if_modified(rtk_request rq,std::string value){
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
void rtk_response::rtk_http_handle_head(rtk_request rq) {
    if(rq.head_list.find("Host")!= rq.head_list.end()){
        rtk_http_ignore();
    }else if(rq.head_list.find("Connection") != rq.head_list.end()){
        rtk_http_connection(rq,rq.head_list["Connection"]);
    }else if(rq.head_list.find("If_Modified_Since") != rq.head_list.end()){
        rtk_http_if_modified(rq,rq.head_list["If_Modified_Since"]);
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