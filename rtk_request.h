//
// Created by nins on 2022/3/9.
//

#ifndef RTKPP_RTK_REQUEST_H
#define RTKPP_RTK_REQUEST_H

//#include <string>
//#include <unordered_map>
#include "stdafx.h"

#define RTK_AGAIN EAGAIN

#define RTK_HTTP_PARSE_INVALID_METHOD 10
#define RTK_HTTP_PARSE_INVALID_REQUEST 11
#define RTK_HTTP_PARSE_INVALID_HEADER 12

#define RTK_HTTP_UNKNOW 0x0001
#define RTK_HTTP_GET 0x0002
#define RTK_HTTP_HEAD 0x0004
#define RTK_HTTP_POST 0x0008

#define CR '\r'
#define LF '\n'

#define RTK_HTTP_OK 200
#define RTK_HTTP_NOT_MODIFIED 304
#define RTK_HTTP_NOT_FOUND 404

#define MAX_BUF 6657


extern std::unordered_map<std::string,std::string> rtk_mime_type;

class rtk_request{
public:
    std::string root;
    std::string uri;
    std::string req_file_type;
    int method;
    int fd;
    int epoll_fd;
    char buff[MAX_BUF];
    int state;
    size_t pos; //解析指针位
    size_t last; //读取指针位

    //断点读取buff的指针
    void* request_start;
    void* request_end;
    void* uri_start;
    void* uri_end;

    int http_major;
    int http_minor;

    std::unordered_map<std::string,std::string> head_list; //存储请求头具体类别
    //这样的构造函数无意义，因为不可能传递一个fd为0的请求
    //rtk_request(std::string path);
    rtk_request(std::string path,int fd,int epoll_fd);
    ~rtk_request();

    //关闭fd，释放请求的内存
    void RTK_close();
    //解析请求行
    int parse_request_line();
    //解析请求体
    int parse_request_body();
    //解析请求uri,获取请求文件类型
    std::string parse_uri();


    //测试func
    void test_make_bufs();

};



#endif //RTKPP_RTK_REQUEST_H
