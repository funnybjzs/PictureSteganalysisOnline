#ifndef _COMMON_H_
#define _COMMON_H_

#include "SendHeartbeat.h"
#include "RecvCmdAndConfig.h"
#include "Config.h"
//接口类型宏定义
#define CMDINTERFACE         0x1                  //指令接口
#define CONFIGINTERFACE      0x2                  //配置接口
#define HEARTBEATINTERFACE   0x3                  //状态（心跳）接口

//web服务器IP地址和端口 此项以后应该写在xml中
//#define WEBSERVERIP          "172.16.0.19"          //web服务器IP地址 
//#define WEBSERVERPORT        12345                //web服务器端口号
#define CMDANDCONFIGPORT     12346

//心跳的两种状态
#define HEARTSERVICECLOSE    0x1f                 //程序启动 人为关闭
#define HEARTSERVICERUN      0x11                 //程序启动 正常运行

//心跳间隔时间 5s
#define HEARTBEATINTERVAL    5

//指令类型宏定义
#define EQUSTART             0x1
#define EQUSTOP              0xf

//程序状态变量
extern volatile bool heartCondition;

#endif

