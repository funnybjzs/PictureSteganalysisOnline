/*
 * Parse.h
 *
 *  Created on: 2014年6月10日
 *      Author: zhaoshuo
 */

#ifndef PARSE_H_
#define PARSE_H_
#include "AppDataDefine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "systemall.h"

#define DEBUG

#ifdef DEBUG
#define DebugPrint(errormsg)       fprintf(stderr,"File : %s, Line: %d, func: %s , errno:%d, strerror: %s, diag: %s\n",__FILE__,__LINE__,__func__,errno,strerror(errno),errormsg);
#else
#define DebugPrint(errormsg)
#endif

//通用解析
int ParseHead(char *data,AppData_Head *head);
void ParseBody(char *buffer,int opt_num);
char* GetServiceType(char *data);
char* str_reverse(char *s1,char const *s2);
//http服务
void GetHttpOption(char *data,int num,HttpServerInfo *hp);
void SetHttpOption(HttpServerInfo *hp,char *opt_name,int length,char *index);
void GetHttpInnerOption(HttpServerInfo *hp,char *data,int length,int http_request);
void SetHttpInnerOption(HttpServerInfo *hp,char *opt_name,int length,char *index,int http_request);
void SetHttpTuple4list(HttpServerInfo *hp,int length);
//Mail服务
void GetMailOption(char *data,int num,MailServerInfo *mf);
void SetMailOption(MailServerInfo *mf,char *opt_name,int length,char *index);
void GetFileSetInnerOption(MailServerInfo *mf,char *data,int length);
void SetFileInnerOption(MailServerInfo *mf,char *opt_name,int length,char *index);
void SetMailTuple4list(MailServerInfo *mf,int length);
//http处理
void InitHttpServerInfo(HttpServerInfo *hp);
void FreeHttpServerInfo(HttpServerInfo *hp);
void InitAppHead(AppData_Head *head);
void FreeAppHead(AppData_Head *head);
int HasResponseOpt(HttpServerInfo *hsi);
//mail处理
void InitMailServerInfo(MailServerInfo *mf);
void FreeMailServerInfo(MailServerInfo *mf);

//封装
void HttpParse(char  *data ,int opt_num);
void SmtpParse(char  *data ,int opt_num);
void ProcessSmtpAttachments(MailServerInfo & msi);

#endif /* PARSE_H_ */
