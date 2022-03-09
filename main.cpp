#include <iostream>

#include "utils.h"

#define CONF_FILE "../tkeed.conf"

RTK_config conf;



int main() {
    std::cout << "Hello, World!" << std::endl;
    //读配置
    read_conf(CONF_FILE,conf);
    //屏蔽掉SIGPIPE信号
    handle_for_sigpipe();



    return 0;
}
