#include <iostream>


#include "utils.h"


#define CONF_FILE "../tkeed.conf"

RTK_config conf;



int main() {
    std::cout << "Hello, World!" << std::endl;

    read_conf(CONF_FILE,conf);



    return 0;
}
