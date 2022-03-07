//
// Created by nins on 2022/3/7.
//

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

#include "utils.h"

using namespace std;

bool str_valid_isdigit(string str){
    int n = str.length();
    if(n > 6) return false;
    for(int i = 0;i < str.length();i++){
        if(!isdigit(str[i])){
            return false;
        }
    }
    return true;
}

int read_conf(string filename, RTK_config cfg){

    fstream infile;
    infile.open(filename,ios::in); //只读打开
    if(!infile)
        return RTK_CONF_ERROR;

    string line;
    while(getline( infile,line)){

        size_t found = line.find(DELIM);
        //如果找不到分隔符
        if(found == std::string::npos){
            return RTK_CONF_ERROR;
        }
        string key = line.substr(0,found);
        string value = line.substr(found+1);

        //c++的string是类，所以可以直接用==来比较  char*是指针，==在比较两指针是否相等
        if(key == "root"){
            cfg.root = value;
        }

        if(key == "port"){
            if(!str_valid_isdigit(value)){
                return RTK_CONF_ERROR;
            }
            cfg.port = stoi(value);
        }

        if(key == "thread_num"){
            if(!str_valid_isdigit(value)){
                return RTK_CONF_ERROR;
            }
            cfg.port = stoi(value);
        }
    }

    infile.close();
    return RTK_CONF_OK;
}

