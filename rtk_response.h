//
// Created by nins on 2022/3/11.
//

#ifndef RTKPP_RTK_RESPONSE_H
#define RTKPP_RTK_RESPONSE_H

#include <cstring>
#include <sys/stat.h>

#include "stdafx.h"
#include "rtk_request.h"
#include "rtk_io.h"





#define MAXLINE 8845
#define SHORTLINE 512



class rtk_response{
public:
    rtk_response(int fd);
    ~rtk_response();

public:
    int fd;
    int keep_alive;
    time_t mtime;
    int modified;
    int status;

public:
    //响应控制函数
    //headers形如  Connection:keep-alive
    int rtk_http_ignore();
    int rtk_http_connection(rtk_request,std::string);
    int rtk_http_if_modified(rtk_request,std::string);

    //拼接错误响应信息
    void do_error(int fd,char* cause,char* err_num,char* short_msg,char* long_msg);
    //找不到目标文件的响应
    int error_file_path(struct stat* sbufptr,std::string filename,int fd);



    //根据状态码返回短句
    std::string get_shortmsg_from_status_code(int);
    //根据请求类型,设置不同标志位
    void rtk_http_handle_head(rtk_request);
    //filesize是struct stat中的一个属性,stat是内核获取指定文件的信息结构体
    void serve_static(rtk_request rq,std::string filename,size_t filesize);

    //void do_request(rtk_request rq);



};

extern std::unordered_map<std::string,std::string> rtk_mime_type;

/*
//利用handle遍历显然不如map的二叉树结构查询，其次，函数指针指向非静态成员函数时一定要把类实例化
//如果不实例化，就只能指向非静态成员函数，但是keep-alive属性的赋值，又需要在静态函数中访问非静态成员，因此只能把对象做一个静态全局化
typedef int(rtk_response::*rtk_http_header_handle_pt)(rtk_request,std::string,rtk_response res);
extern std::pair<std::string,rtk_http_header_handle_pt> rtk_http_headers_in[]; */



#endif //RTKPP_RTK_RESPONSE_H
