

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
//#include "StegoConfig.h"

using namespace std;

extern char FILTER_TYPE[32];
extern char PICTURE_TO_STORE[64];

//模板信息
extern sys_setting  *setting;
extern QTDatabase qt;

//初始化数据库环境资源
void InitSystemEnv(char *xmlfile);

//释放数据库环境资源
void FreeSystemEnv();

#endif
