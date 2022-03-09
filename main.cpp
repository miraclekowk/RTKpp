
#include "utils.h"

#define CONF_FILE "../tkeed.conf"

RTK_config conf;


#ifndef DEBUG_MODE
int main() {
    std::cout << "Hello, World!" << std::endl;
    //读配置
    read_conf(CONF_FILE,conf);
    //屏蔽掉SIGPIPE信号
    handle_for_sigpipe();



    return 0;
}
#else

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#endif