

/*

 * system.h
 *
 *  Created on: 2014年8月16日
 *      Author: zhaoshuo
 */

#ifndef __SYSTEM__H__
#define __SYSTEM__H__
#include <iostream>
#include "include/stego_analysis.h"
#include "QTDatabase.h"

using namespace std;

//初始化数据库环境资源
void InitSystemEnv();

//释放数据库环境资源
void FreeSystemEnv();

#endif
