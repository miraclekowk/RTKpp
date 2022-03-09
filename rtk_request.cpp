//
// Created by nins on 2022/3/9.
//

#include "rtk_request.h"

std::unordered_map<std::string,std::string> rtk_mime_type = {
        {".html", "text/html"},
        {".xml", "text/xml"},
        {".xhtml", "application/xhtml+xml"},
        {".txt", "text/plain"},
        {".rtf", "application/rtf"},
        {".pdf", "application/pdf"},
        {".word", "application/msword"},
        {".png", "image/png"},
        {".gif", "image/gif"},
        {".jpg", "image/jpeg"},
        {".jpeg", "image/jpeg"},
        {".au", "audio/basic"},
        {".mpeg", "video/mpeg"},
        {".mpg", "video/mpeg"},
        {".avi", "video/x-msvideo"},
        {".gz", "application/x-gzip"},
        {".tar", "application/x-tar"},
        {".css", "text/css"},
        //这样写报错 what():  basic_string::_M_construct null not valid
        //can not init string as a NULL!!!
        //{NULL ,"text/plain"}
        {"" ,"text/plain"},
};



rtk_request::rtk_request(std::string path):root(path),fd(0), epoll_fd(0),pos(0),last(0),state(0){

};

rtk_request::~rtk_request() {

};

int rtk_request::parse_request_line() {
    enum{
        sw_start = 0,
        sw_method,
        sw_spaces_before_uri,
        sw_after_slash_in_uri,
        sw_http,
        sw_http_H,
        sw_http_HT,
        sw_http_HTT,
        sw_http_HTTP,
        sw_first_major_digit,
        sw_major_digit,
        sw_first_minor_digit,
        sw_minor_digit,
        sw_spaces_after_digit,
        sw_almost_done
    }state;
    int cur_state = this->state;

    //ch 当前字符,*p当前指针,*m
    u_char ch,*p,*m;
    size_t pi;   //point index
    std::string tmp_method = "";

    for(pi = this->pos ;pi < this->last;pi++){
        //p为指向当前地址的指针，循环读取buff
        p = (u_char*) &this->buff[pi%MAX_BUF];
        ch = *p;

        switch (cur_state) {
            case sw_start:
                this->request_start = p;

                //遇到结束符，直接退出
                if(ch == CR || ch == LF)
                    break;
                if((ch < 'A' || ch > 'Z') && ch != '_')
                    return RTK_HTTP_PARSE_INVALID_METHOD;

                //切换state状态
                cur_state = sw_method;
                tmp_method.push_back(buff[pi]);
                break; //move pointer

            case sw_method:

                this->method = RTK_HTTP_UNKNOW; //default method
                tmp_method.push_back(buff[pi]);

                //读取至GET space
                if(ch == ' '){
                    this->request_start = p;
                    //标记方法的开头，这里使用双指针，如GET标记G，p指针继续向前移动
                    //m = (u_char*)this->request_start;

                    int len = tmp_method.length();

                    //可以拼成GET ,POST ,HEAD (注意方法后都有空格)
                    if( len == 4){
                        if(tmp_method == "GET "){
                            this->method = RTK_HTTP_GET;
                        }else{
                            break; //buff_point to next
                        }
                    }else if(len == 5){
                        if(tmp_method == "POST "){
                            this->method = RTK_HTTP_POST;
                        }else if(tmp_method == "HEAD "){
                            this->method = RTK_HTTP_HEAD;
                        }else{
                            break;
                        }
                    }

                    cur_state = sw_spaces_before_uri; //method is ok
                    break;
                }

                if((ch < 'A' || ch > 'Z') && ch != '_')
                    return RTK_HTTP_PARSE_INVALID_METHOD;
                break;

            case sw_spaces_before_uri:
                if(ch == '/'){
                    this->uri_start = p + 1;  //uri begin
                    cur_state = sw_after_slash_in_uri;
                    break;
                }

                if(ch == ' ')  //still space,continue move the pointer
                    break;
                else
                    return RTK_HTTP_PARSE_INVALID_REQUEST;

                break;  //if not '/' or ' ',continue

            case sw_after_slash_in_uri:
                //uri 形如 http://baidu.com/example/xxx/4?key=abc space 以空格做结束划分
                if(ch == ' '){
                    this->uri_end = p;
                    cur_state = sw_http;
                    break;
                }
                break;

            case sw_http:
                if(ch == ' ') {
                    break;
                }else if(ch == 'H'){
                    cur_state = sw_http_H;
                    break;
                }else{
                    return RTK_HTTP_PARSE_INVALID_REQUEST;
                }

            case sw_http_H:
                if(ch == 'T'){
                    cur_state = sw_http_HT;
                    break;
                }else{
                    return RTK_HTTP_PARSE_INVALID_REQUEST;
                }
                break;

            case sw_http_HT:
                if(ch == 'T'){
                    cur_state = sw_http_HTT;
                    break;
                }else{
                    return RTK_HTTP_PARSE_INVALID_REQUEST;
                }

            case sw_http_HTT:
                if(ch == 'P'){
                    cur_state = sw_http_HTTP;
                    break;
                }else{
                    return RTK_HTTP_PARSE_INVALID_REQUEST;
                }

            case sw_http_HTTP:
                if(ch == '/'){
                    cur_state = sw_first_major_digit;
                    break;
                }else{
                    return RTK_HTTP_PARSE_INVALID_REQUEST;
                }

            case sw_first_major_digit:
                if(ch > '0' && ch < '9'){
                    this->http_major = ch - '0';
                    cur_state = sw_major_digit;
                }
                    break;


            case sw_major_digit:
                if(ch == '.'){
                    cur_state = sw_first_minor_digit;
                    break;
                }

                if(ch < '0' || ch > '9'){
                    return RTK_HTTP_PARSE_INVALID_REQUEST;
                }
                this->http_major = this->http_major * 10 + (ch - '0');
                break;

            case sw_first_minor_digit:
                if(ch < '0' || ch > '9')
                    return RTK_HTTP_PARSE_INVALID_REQUEST;
                this->http_minor = ch - '0';
                cur_state = sw_minor_digit;
                break;

            case sw_minor_digit:
                if(ch == CR){
                    cur_state = sw_almost_done;
                    break;
                }
                if(ch == LF){
                    goto done;
                }
                //if minor number contain a space
                if(ch == ' '){
                    cur_state = sw_spaces_after_digit;
                    break;
                }
                if(ch < '0' || ch > '9')
                    return RTK_HTTP_PARSE_INVALID_REQUEST;

                this->http_minor = this->http_minor * 10 + (ch - '0');
                break;

            case sw_spaces_after_digit:
                if(ch == ' '){
                    break;
                }else if(ch == CR){
                    cur_state = sw_almost_done;
                    break;
                }else if(ch == LF){
                    goto done;
                }else{
                    return RTK_HTTP_PARSE_INVALID_REQUEST;
                }

            case sw_almost_done:
                this->request_end = p - 1;
                if(ch == LF){
                    goto done;
                }else{
                    return RTK_HTTP_PARSE_INVALID_REQUEST;
                }
        } //switch...
        //when you break switch and do not return/goto,you move the buff_pointer
    } //for...

    //没读完，保存状态
    this->pos = pi;
    this->state = cur_state;
    return RTK_AGAIN;   //返回一个EGAIN错误，表示还没读完

    //这部分是否有必要还需要考虑
    done:
    this->pos = pi + 1;
    if(NULL == this->request_end){
        this->request_end = p;
    }
    this->state = sw_start;
    return 0;
};

int rtk_request::parse_request_body() {

};

int rtk_request::parse_uri() {

};

void rtk_request::close() {

};

void rtk_request::test_make_bufs() {
    //uri是一个本地的
    std::string str = "GET  /home/x/4?key=abc HTTP/1.12";
    for(int i = 0;i < str.length();i++){
        this->buff[i] = str[i];
    }
    this->last = str.length();
}

#ifdef DEBUG_MODE

TEST(TestCase,test_prase_request_line){
    rtk_request rq("../");
    rq.test_make_bufs();


    EXPECT_EQ(0,rq.parse_request_line());
}


#endif
