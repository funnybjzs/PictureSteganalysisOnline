/*
 * ParseMailInfo.cpp
 *
 *  Created on: 2014年8月15日
 *      Author: zhaoshuo
 */
#include "AppDataDefine.h"
#include "Parse.h"

//[输入]data:待解析的数据；num:选项数量（根据数据头的opt_num得到）；*mf:存放解析内容的结构体
//[输出]void
void GetMailOption(char *data,int num,MailServerInfo *mf)
{
    char *index=data;
    char eachline[1024];        //每行内容
    int line_length;            //每行长度，用于统计指针偏移
    char opt_name[50];          //选项名称
    char opt_len[32];          //选项长度

    int i;
    for(i=0;i<num;i++)
    {
      //选项名称
      sscanf(index,"%s%n",eachline,&line_length);
      sscanf(eachline,"%*[^:]:%[^\r\n]",opt_name);
      index=index+line_length+2;

      //选项内容长度
      sscanf(index,"%s%n",eachline,&line_length);
      sscanf(eachline,"%*[^:]:%[^\r\n]",opt_len);
      index=index+line_length+2;

      //存储到HttpServerInfo中
      SetMailOption(mf,opt_name,atoi(opt_len),index);
      index=index+atoi(opt_len);

    }

}

//[输入]*mf:存放解析内容的结构体；*opt_name:选项名称；length:选项内容长度；*index：指向选项内容的指针
void SetMailOption(MailServerInfo *mf,char *opt_name,int length,char *index)
{

	if(strcmp(opt_name,"OPT_PROTOCOL")==0) {
		mf->common.OPT_PROTOCOL=(char *)malloc(length+1);
		memset(mf->common.OPT_PROTOCOL,0,length+1);
		memcpy(mf->common.OPT_PROTOCOL,index,length);
	}
	else if (strcmp(opt_name,"OPT_CLIENT_IP")==0){
		mf->common.OPT_CLIENT_IP=(char *)malloc(length+1);
		memset(mf->common.OPT_CLIENT_IP,0,length+1);
		memcpy(mf->common.OPT_CLIENT_IP,index,length);
	}
	else if (strcmp(opt_name,"OPT_CLIENT_PORT")==0){
		mf->common.OPT_CLIENT_PORT=(char *)malloc(length+1);
		memset(mf->common.OPT_CLIENT_PORT,0,length+1);
		memcpy(mf->common.OPT_CLIENT_PORT,index,length);
	}
	else if (strcmp(opt_name, "OPT_SERVER_IP")==0){
		mf->common.OPT_SERVER_IP=(char *)malloc(length+1);
		memset(mf->common.OPT_SERVER_IP,0,length+1);
		memcpy(mf->common.OPT_SERVER_IP,index,length);
	}
	else if (strcmp(opt_name, "OPT_SERVER_PORT")==0){
		mf->common.OPT_SERVER_PORT=(char *)malloc(length+1);
		memset(mf->common.OPT_SERVER_PORT,0,length+1);
		memcpy(mf->common.OPT_SERVER_PORT,index,length);
	}
	else if (strcmp(opt_name,"OPT_SERVICE")==0){
		mf->common.OPT_SERVICE=(char *)malloc(length+1);
		memset(mf->common.OPT_SERVICE,0,length+1);
		memcpy(mf->common.OPT_SERVICE,index,length);
	}
	else if (strcmp(opt_name, "OPT_SUB_SERVICE")==0){
		mf->common.OPT_SUB_SERVICE=(char *)malloc(length+1);
		memset(mf->common.OPT_SUB_SERVICE,0,length+1);
		memcpy(mf->common.OPT_SUB_SERVICE,index,length);
	}
	else if (strcmp(opt_name,"OPT_CAP_TIME")==0){
		mf->common.OPT_CAP_TIME=(char *)malloc(length+1);
		memset(mf->common.OPT_CAP_TIME,0,length+1);
		memcpy(mf->common.OPT_CAP_TIME,index,length);
	}
	else if (strcmp(opt_name, "OPT_URL")==0){
		mf->common.OPT_URL=(char *)malloc(length+1);
		memset(mf->common.OPT_URL,0,length+1);
		memcpy(mf->common.OPT_URL,index,length);
	}
	else if (strcmp(opt_name,"OPT_COOKIE")==0){
		mf->common.OPT_COOKIE=(char *)malloc(length+1);
		memset(mf->common.OPT_COOKIE,0,length+1);
		memcpy(mf->common.OPT_COOKIE,index,length);
	}
	else if (strcmp(opt_name, "OPT_REFERER")==0){
		mf->common.OPT_REFERER=(char *)malloc(length+1);
		memset(mf->common.OPT_REFERER,0,length+1);
		memcpy(mf->common.OPT_REFERER,index,length);
	}
	else if (strcmp(opt_name, "OPT_USER_NAME")==0){
		mf->common.OPT_USER_NAME=(char *)malloc(length+1);
		memset(mf->common.OPT_USER_NAME,0,length+1);
		memcpy(mf->common.OPT_USER_NAME,index,length);
	}
	else if (strcmp(opt_name, "OPT_PASSWORD")==0){
		mf->common.OPT_PASSWORD=(char *)malloc(length+1);
		memset(mf->common.OPT_PASSWORD,0,length+1);
		memcpy(mf->common.OPT_PASSWORD,index,length);
	}
	else if (strcmp(opt_name, "OPT_DATE")==0){
		mf->common.OPT_DATE=(char *)malloc(length+1);
		memset(mf->common.OPT_DATE,0,length+1);
		memcpy(mf->common.OPT_DATE,index,length);
	}
	else if (strcmp(opt_name, "OPT_FILE_NAME")==0){
		mf->common.OPT_FILE_NAME=(char *)malloc(length+1);
		memset(mf->common.OPT_FILE_NAME,0,length+1);
		memcpy(mf->common.OPT_FILE_NAME,index,length);
	}
	else if (strcmp(opt_name, "OPT_FILE_CONTENT")==0){
		mf->common.OPT_FILE_CONTENT=(char *)malloc(length+1);
		memset(mf->common.OPT_FILE_CONTENT,0,length+1);
		memcpy(mf->common.OPT_FILE_CONTENT,index,length);
	}
	else if (strcmp(opt_name, "OPT_FILE_SET")==0){
		//mail附件
        GetFileSetInnerOption(mf,index,length);
	}
	else if (strcmp(opt_name, "OPT_FLOW_ID_LIST")==0){
		mf->common.OPT_FLOW_ID_LIST=(char *)malloc(length+1);
		memset(mf->common.OPT_FLOW_ID_LIST,0,length+1);
		memcpy(mf->common.OPT_FLOW_ID_LIST,index,length);
	}
	else if (strcmp(opt_name, "OPT_TUPLE4_LIST")==0){
		mf->common.OPT_TUPLE4_LIST=(char *)malloc(length+1);
		memset(mf->common.OPT_TUPLE4_LIST,0,length+1);
		memcpy(mf->common.OPT_TUPLE4_LIST,index,length);
		//设置四元组信息
		SetMailTuple4list(mf,length);
	}  //邮件服务专用选项
	else if(strcmp(opt_name, "OPT_MAIL_FROM")==0)
	{
		mf->mail.opt_mail_from=(char *)malloc(length+1);
		memset(mf->mail.opt_mail_from,0,length+1);
		memcpy(mf->mail.opt_mail_from,index,length);
	}
	else if(strcmp(opt_name, "OPT_MAIL_TO")==0)
	{
		mf->mail.opt_mail_to=(char *)malloc(length+1);
		memset(mf->mail.opt_mail_to,0,length+1);
		memcpy(mf->mail.opt_mail_to,index,length);
	}
	else if(strcmp(opt_name, "OPT_SUBJECT")==0)
	{
		mf->mail.opt_subject=(char *)malloc(length+1);
		memset(mf->mail.opt_subject,0,length+1);
		memcpy(mf->mail.opt_subject,index,length);
	}
	else if(strcmp(opt_name, "OPT_MAIL_CONTENT")==0)
	{
		mf->mail.opt_mail_content=(char *)malloc(length+1);
		memset(mf->mail.opt_mail_content,0,length+1);
		memcpy(mf->mail.opt_mail_content,index,length);
	}

}

//[输入]*hp:存放解析内容的结构体；*data:嵌套选项（这里是HttpRequest或HttpResponse)；length:嵌套选项内容长度；
// http_request:标志是否是request，非0即是
//[输出]void
void GetFileSetInnerOption(MailServerInfo *mf,char *data,int length)
{
	int line_length=0;
	int total_length=0;
	char *index=data;
	char eachline[1024];
	char opt_name[50];
	char opt_len[32];

    while(total_length<length)
    {
        sscanf(index,"%s%n",eachline,&line_length);
        sscanf(eachline,"%*[^:]:%[^\r\n]",opt_name);
        index=index+line_length+2;
        total_length=total_length+line_length+2;

        sscanf(index,"%s%n",eachline,&line_length);
        sscanf(eachline,"%*[^:]:%[^\r\n]",opt_len);
        index=index+line_length+2;
        total_length=total_length+line_length+2;


        SetFileInnerOption(mf,opt_name,atoi(opt_len),index);
        index=index+atoi(opt_len);
        total_length=total_length+atoi(opt_len);
    }

}

void SetFileInnerOption(MailServerInfo *mf,char *opt_name,int length,char *index)
{
	if(strcmp(opt_name,"OPT_FILE_NAME")==0)
	{
			//mf->common.OPT_FILE_NAME=(char *)malloc(length+1);
			//memset(mf->common.OPT_FILE_NAME,0,length+1);
			//memcpy(mf->common.OPT_FILE_NAME,index,length);
		  char *tmp=(char *)malloc(length+1);
		  memset(tmp,0,length+1);
		  memcpy(tmp,index,length);
		  mf->mail.AttachFileNames.push_back(tmp);
	}
	else if(strcmp(opt_name,"OPT_FILE_CONTENT")==0)
	{
		  char *tmp=(char *)malloc(length+1);
		  memset(tmp,0,length+1);
		  memcpy(tmp,index,length);

		  mf->mail.AttachFileLength.push_back(length);
		  mf->mail.AttachFileContent.push_back(tmp);
		//	mf->common.OPT_FILE_CONTENT=(char *)malloc(length+1);
		//	memset(mf->common.OPT_FILE_CONTENT,0,length+1);
		//	memcpy(mf->common.OPT_FILE_CONTENT,index,length);

	}
	else
	{
		;
	}
}

//[输入]*mf:存放解析内容的结构体；length:四元组选项的长度
void SetMailTuple4list(MailServerInfo *mf,int length)
{
	char src[32];
	char dst[256];

	sscanf(mf->common.OPT_TUPLE4_LIST,"%[^-]",src);
	sscanf(mf->common.OPT_TUPLE4_LIST,"%*[^-]-%s",dst);

    mf->common.OPT_CLIENT_IP=(char *)malloc(length+1);
    memset(mf->common.OPT_CLIENT_IP,0,length+1);

    mf->common.OPT_CLIENT_PORT=(char *)malloc(length+1);
    memset(mf->common.OPT_CLIENT_PORT,0,length+1);

    mf->common.OPT_SERVER_IP=(char *)malloc(length+1);
    memset(mf->common.OPT_SERVER_IP,0,length+1);

    mf->common.OPT_SERVER_PORT=(char *)malloc(length+1);
    memset(mf->common.OPT_SERVER_PORT,0,length+1);

	sscanf(src,"%[^:]",mf->common.OPT_SERVER_IP);
	sscanf(dst,"%[^:]",mf->common.OPT_CLIENT_IP);
	sscanf(src,"%*[^:]:%s",mf->common.OPT_SERVER_PORT);

	if(sscanf(dst,"%*[^:]:%[^,]",mf->common.OPT_CLIENT_PORT)==-1)
	{
		sscanf(dst,"%*[^:]:%s",mf->common.OPT_CLIENT_PORT);
	}
}

