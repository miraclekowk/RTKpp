//
// Created by nins on 2022/3/12.
//

#include "rtk_io.h"

//对write进行封装，应对一次写不完的情况
ssize_t rtk_io_writen(int fd,void* userbuf,size_t n){
    size_t nleft = n;
    ssize_t nwritten;
    char* bufp = (char*) userbuf; //读取用户态Buf的指针

    //还未读完buff就while一直读
    while(nleft > 0){
        //write()的第三个参数是期望count，实际可能小于这个值
        //因为底层物理空间或调用信号被处理器中断都可能导致实际值较小
        if((nwritten = write(fd,bufp,nleft)) <= 0){
            //errnor is the number of last error
            if(errno == EINTR){
                nwritten = 0;
            }else{
                return -1;
            }
        }
        nleft -= nwritten;
        bufp += nwritten;
    }

    return n; //正常读取到期望的n个字符
}