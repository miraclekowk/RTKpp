//
// Created by nins on 2022/3/10.
// stdafx.h means mutual header in precompiled process
// stdafx.h是一个公有头，在预编译阶段发挥作用  还能控制googletest做单元测试

#ifndef RTKPP_STDAFX_H
#define RTKPP_STDAFX_H

//common STL
#include <iostream>
#include <unordered_map>
#include <string>
#include <map>
#include <algorithm>
#include <queue>
#include <cmath>
//common UNIX-api
#include <unistd.h>
#include <time.h>


//test control --如果不想走测试，注释掉此行代码
#define DEBUG_MODE


#ifdef DEBUG_MODE
#include "gtest/gtest.h"
#endif

#endif //RTKPP_STDAFX_H
