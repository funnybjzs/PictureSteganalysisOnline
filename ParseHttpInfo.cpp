/*
 * ParseHttpInfo.c
 *
 *  Created on: 2014年6月3日
 *      Author: zhaoshuo
 */
#include "AppDataDefine.h"
#include "Parse.h"

//[输入]data:待解析的数据；num:选项数量（根据数据头的opt_num得到）；*hp:存放解析内容的结构体
//[输出]void
void GetHttpOption(char *data,int num,HttpServerInfo *hp)
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
      SetHttpOption(hp,opt_name,atoi(opt_len),index);
      index=index+atoi(opt_len);

    }

}

//[输入]*hp:存放解析内容的结构体；*opt_name:选项名称；length:选项内容长度；*index：指向选项内容的指针
//[输出]void
void SetHttpOption(HttpServerInfo *hp,char *opt_name,int length,char *index)
{

	if(strcmp(opt_name,"OPT_PROTOCOL")==0) {
		hp->common.OPT_PROTOCOL=(char *)malloc(length+1);
		memset(hp->common.OPT_PROTOCOL,0,length+1);
		memcpy(hp->common.OPT_PROTOCOL,index,length);
	}
	else if (strcmp(opt_name,"OPT_CLIENT_IP")==0){
		hp->common.OPT_CLIENT_IP=(char *)malloc(length+1);
		memset(hp->common.OPT_CLIENT_IP,0,length+1);
		memcpy(hp->common.OPT_CLIENT_IP,index,length);
	}
	else if (strcmp(opt_name,"OPT_CLIENT_PORT")==0){
		hp->common.OPT_CLIENT_PORT=(char *)malloc(length+1);
		memset(hp->common.OPT_CLIENT_PORT,0,length+1);
		memcpy(hp->common.OPT_CLIENT_PORT,index,length);
	}
	else if (strcmp(opt_name, "OPT_SERVER_IP")==0){
		hp->common.OPT_SERVER_IP=(char *)malloc(length+1);
		memset(hp->common.OPT_SERVER_IP,0,length+1);
		memcpy(hp->common.OPT_SERVER_IP,index,length);
	}
	else if (strcmp(opt_name, "OPT_SERVER_PORT")==0){
		hp->common.OPT_SERVER_PORT=(char *)malloc(length+1);
		memset(hp->common.OPT_SERVER_PORT,0,length+1);
		memcpy(hp->common.OPT_SERVER_PORT,index,length);
	}
	else if (strcmp(opt_name,"OPT_SERVICE")==0){
		hp->common.OPT_SERVICE=(char *)malloc(length+1);
		memset(hp->common.OPT_SERVICE,0,length+1);
		memcpy(hp->common.OPT_SERVICE,index,length);
	}
	else if (strcmp(opt_name, "OPT_SUB_SERVICE")==0){
		hp->common.OPT_SUB_SERVICE=(char *)malloc(length+1);
		memset(hp->common.OPT_SUB_SERVICE,0,length+1);
		memcpy(hp->common.OPT_SUB_SERVICE,index,length);
	}
	else if (strcmp(opt_name,"OPT_CAP_TIME")==0){
		hp->common.OPT_CAP_TIME=(char *)malloc(length+1);
		memset(hp->common.OPT_CAP_TIME,0,length+1);
		memcpy(hp->common.OPT_CAP_TIME,index,length);
	}
	else if (strcmp(opt_name, "OPT_URL")==0){
		hp->common.OPT_URL=(char *)malloc(length+1);
		memset(hp->common.OPT_URL,0,length+1);
		memcpy(hp->common.OPT_URL,index,length);
	}
	else if (strcmp(opt_name,"OPT_COOKIE")==0){
		hp->common.OPT_COOKIE=(char *)malloc(length+1);
		memset(hp->common.OPT_COOKIE,0,length+1);
		memcpy(hp->common.OPT_COOKIE,index,length);
	}
	else if (strcmp(opt_name, "OPT_REFERER")==0){
		hp->common.OPT_REFERER=(char *)malloc(length+1);
		memset(hp->common.OPT_REFERER,0,length+1);
		memcpy(hp->common.OPT_REFERER,index,length);
	}
	else if (strcmp(opt_name, "OPT_USER_NAME")==0){
		hp->common.OPT_USER_NAME=(char *)malloc(length+1);
		memset(hp->common.OPT_USER_NAME,0,length+1);
		memcpy(hp->common.OPT_USER_NAME,index,length);
	}
	else if (strcmp(opt_name, "OPT_PASSWORD")==0){
		hp->common.OPT_PASSWORD=(char *)malloc(length+1);
		memset(hp->common.OPT_PASSWORD,0,length+1);
		memcpy(hp->common.OPT_PASSWORD,index,length);
	}
	else if (strcmp(opt_name, "OPT_DATE")==0){
		hp->common.OPT_DATE=(char *)malloc(length+1);
		memset(hp->common.OPT_DATE,0,length+1);
		memcpy(hp->common.OPT_DATE,index,length);
	}
	else if (strcmp(opt_name, "OPT_FILE_NAME")==0){
		hp->common.OPT_FILE_NAME=(char *)malloc(length+1);
		memset(hp->common.OPT_FILE_NAME,0,length+1);
		memcpy(hp->common.OPT_FILE_NAME,index,length);
	}
	else if (strcmp(opt_name, "OPT_FILE_CONTENT")==0){
		hp->common.OPT_FILE_CONTENT=(char *)malloc(length+1);
		memset(hp->common.OPT_FILE_CONTENT,0,length+1);
		memcpy(hp->common.OPT_FILE_CONTENT,index,length);
	}
	else if (strcmp(opt_name, "OPT_FILE_SET")==0){
		hp->common.OPT_FILE_SET=(char *)malloc(length+1);
		memset(hp->common.OPT_FILE_SET,0,length+1);
		memcpy(hp->common.OPT_FILE_SET,index,length);
	}
	else if (strcmp(opt_name, "OPT_FLOW_ID_LIST")==0){
		hp->common.OPT_FLOW_ID_LIST=(char *)malloc(length+1);
		memset(hp->common.OPT_FLOW_ID_LIST,0,length+1);
		memcpy(hp->common.OPT_FLOW_ID_LIST,index,length);
	}
	else if (strcmp(opt_name, "OPT_TUPLE4_LIST")==0){
		hp->common.OPT_TUPLE4_LIST=(char *)malloc(length+1);
		memset(hp->common.OPT_TUPLE4_LIST,0,length+1);
		memcpy(hp->common.OPT_TUPLE4_LIST,index,length);
		//设置四元组信息
		SetHttpTuple4list(hp,length);
	}
	else if(strcmp(opt_name,"opt_http_request")==0)
	{
		//内层嵌套解析
       GetHttpInnerOption(hp,index,length,1);
	}
	else
	{
		//内层嵌套解析
	   GetHttpInnerOption(hp,index,length,0);
	}

}

//[输入]*hp:存放解析内容的结构体；*data:嵌套选项（这里是HttpRequest或HttpResponse)；length:嵌套选项内容长度；
// http_request:标志是否是request，非0即是
//[输出]void
void GetHttpInnerOption(HttpServerInfo *hp,char *data,int length,int http_request)
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


        SetHttpInnerOption(hp,opt_name,atoi(opt_len),index,http_request);
        index=index+atoi(opt_len);
        total_length=total_length+atoi(opt_len);
    }

}

//[输入]*hp:存放解析内容的结构体；*opt_name:选项名称；length:选项内容长度；*index：指向选项内容的指针
// http_request:标志是否是request，非0即是
//[输出]void
void SetHttpInnerOption(HttpServerInfo *hp,char *opt_name,int length,char *index,int http_request)
{
	if(strcmp(opt_name,"OPT_URL")==0)
	{
		if(http_request)
		{
			hp->http_request.OPT_URL=(char *)malloc(length+1);
			memset(hp->http_request.OPT_URL,0,length+1);
			memcpy(hp->http_request.OPT_URL,index,length);
		}
		else
		{
			hp->http_response.OPT_URL=(char *)malloc(length+1);
			memset(hp->http_response.OPT_URL,0,length+1);
			memcpy(hp->http_response.OPT_URL,index,length);
		}
	}
	else if(strcmp(opt_name,"opt_method")==0)
	{
		if(http_request)
		{
			hp->http_request.opt_method=(char *)malloc(length+1);
			memset(hp->http_request.opt_method,0,length+1);
			memcpy(hp->http_request.opt_method,index,length);
		}
		else
		{
			hp->http_response.opt_method=(char *)malloc(length+1);
			memset(hp->http_response.opt_method,0,length+1);
			memcpy(hp->http_response.opt_method,index,length);
		}
	}
	else if(strcmp(opt_name,"OPT_REFERER")==0)
	{
		if(http_request)
		{
			hp->http_request.OPT_REFERER=(char *)malloc(length+1);
			memset(hp->http_request.OPT_REFERER,0,length+1);
			memcpy(hp->http_request.OPT_REFERER,index,length);
		}
		else
		{
			hp->http_response.OPT_REFERER=(char *)malloc(length+1);
			memset(hp->http_response.OPT_REFERER,0,length+1);
			memcpy(hp->http_response.OPT_REFERER,index,length);
		}
	}
	else if(strcmp(opt_name,"opt_user_agent")==0)
	{
		if(http_request)
		{
			hp->http_request.opt_user_agent=(char *)malloc(length+1);
			memset(hp->http_request.opt_user_agent,0,length+1);
			memcpy(hp->http_request.opt_user_agent,index,length);
		}
		else
		{
			hp->http_response.opt_user_agent=(char *)malloc(length+1);
			memset(hp->http_response.opt_user_agent,0,length+1);
			memcpy(hp->http_response.opt_user_agent,index,length);
		}
	}
	else if(strcmp(opt_name,"opt_proxy_authorization")==0)
	{
		if(http_request)
		{
			hp->http_request.opt_proxy_authorization=(char *)malloc(length+1);
			memset(hp->http_request.opt_proxy_authorization,0,length+1);
			memcpy(hp->http_request.opt_proxy_authorization,index,length);
		}
		else
		{
			hp->http_response.opt_proxy_authorization=(char *)malloc(length+1);
			memset(hp->http_response.opt_proxy_authorization,0,length+1);
			memcpy(hp->http_response.opt_proxy_authorization,index,length);
		}
	}
	else if(strcmp(opt_name,"opt_authorization")==0)
	{
		if(http_request)
		{
			hp->http_request.opt_authorization=(char *)malloc(length+1);
			memset(hp->http_request.opt_authorization,0,length+1);
			memcpy(hp->http_request.opt_authorization,index,length);
		}
		else
		{
			hp->http_response.opt_authorization=(char *)malloc(length+1);
			memset(hp->http_response.opt_authorization,0,length+1);
			memcpy(hp->http_response.opt_authorization,index,length);
		}

	}
	else if(strcmp(opt_name,"opt_http_server")==0)
	{
		if(http_request)
		{
			hp->http_request.opt_http_server=(char *)malloc(length+1);
			memset(hp->http_request.opt_http_server,0,length+1);
			memcpy(hp->http_request.opt_http_server,index,length);
		}
		else
		{
			hp->http_response.opt_http_server=(char *)malloc(length+1);
			memset(hp->http_response.opt_http_server,0,length+1);
			memcpy(hp->http_response.opt_http_server,index,length);
		}
	}
	else if(strcmp(opt_name,"opt_etag")==0)
	{
		if(http_request)
		{
			hp->http_request.opt_etag=(char *)malloc(length+1);
			memset(hp->http_request.opt_etag,0,length+1);
			memcpy(hp->http_request.opt_etag,index,length);
		}
		else
		{
			hp->http_response.opt_etag=(char *)malloc(length+1);
			memset(hp->http_response.opt_etag,0,length+1);
			memcpy(hp->http_response.opt_etag,index,length);
		}
	}
	else if(strcmp(opt_name,"opt_response_code")==0)
	{
		if(http_request)
		{
			hp->http_request.opt_response_code=(char *)malloc(length+1);
			memset(hp->http_request.opt_response_code,0,length+1);
			memcpy(hp->http_request.opt_response_code,index,length);
		}
		else
		{
			hp->http_response.opt_response_code=(char *)malloc(length+1);
			memset(hp->http_response.opt_response_code,0,length+1);
			memcpy(hp->http_response.opt_response_code,index,length);
		}

	}
	else if(strcmp(opt_name,"OPT_DATE")==0)
	{
		if(http_request)
		{
			hp->http_request.OPT_DATE=(char *)malloc(length+1);
			memset(hp->http_request.OPT_DATE,0,length+1);
			memcpy(hp->http_request.OPT_DATE,index,length);
		}
		else
		{
			hp->http_response.OPT_DATE=(char *)malloc(length+1);
			memset(hp->http_response.OPT_DATE,0,length+1);
			memcpy(hp->http_response.OPT_DATE,index,length);
		}
	}
	else if(strcmp(opt_name,"opt_user_agent")==0)
	{
		if(http_request)
		{
			hp->http_request.opt_user_agent=(char *)malloc(length+1);
			memset(hp->http_request.opt_user_agent,0,length+1);
			memcpy(hp->http_request.opt_user_agent,index,length);
		}
		else
		{
			hp->http_response.opt_user_agent=(char *)malloc(length+1);
			memset(hp->http_response.opt_user_agent,0,length+1);
			memcpy(hp->http_response.opt_user_agent,index,length);
		}
	}
	else if(strcmp(opt_name,"opt_traile")==0)
	{
		if(http_request)
		{
			hp->http_request.opt_traile=(char *)malloc(length+1);
			memset(hp->http_request.opt_traile,0,length+1);
			memcpy(hp->http_request.opt_traile,index,length);
		}
		else
		{
			hp->http_response.opt_traile=(char *)malloc(length+1);
			memset(hp->http_response.opt_traile,0,length+1);
			memcpy(hp->http_response.opt_traile,index,length);
		}
	}
	else if(strcmp(opt_name,"opt_transfer_encoding")==0)
	{
		if(http_request)
		{
			hp->http_request.opt_transfer_encoding=(char *)malloc(length+1);
			memset(hp->http_request.opt_transfer_encoding,0,length+1);
			memcpy(hp->http_request.opt_transfer_encoding,index,length);
		}
		else
		{
			hp->http_response.opt_transfer_encoding=(char *)malloc(length+1);
			memset(hp->http_response.opt_transfer_encoding,0,length+1);
			memcpy(hp->http_response.opt_transfer_encoding,index,length);
		}
	}
	else if(strcmp(opt_name,"opt_via")==0)
	{
		if(http_request)
		{
			hp->http_request.opt_via=(char *)malloc(length+1);
			memset(hp->http_request.opt_via,0,length+1);
			memcpy(hp->http_request.opt_via,index,length);
		}
		else
		{
			hp->http_response.opt_via=(char *)malloc(length+1);
			memset(hp->http_response.opt_via,0,length+1);
			memcpy(hp->http_response.opt_via,index,length);
		}
	}
	else if(strcmp(opt_name,"opt_pragma")==0)
	{
		if(http_request)
		{
			hp->http_request.opt_pragma=(char *)malloc(length+1);
			memset(hp->http_request.opt_pragma,0,length+1);
			memcpy(hp->http_request.opt_pragma,index,length);
		}
		else
		{
			hp->http_response.opt_pragma=(char *)malloc(length+1);
			memset(hp->http_response.opt_pragma,0,length+1);
			memcpy(hp->http_response.opt_pragma,index,length);
		}
	}
	else if(strcmp(opt_name,"opt_connection")==0)
	{
		if(http_request)
		{
			hp->http_request.opt_connection=(char *)malloc(length+1);
			memset(hp->http_request.opt_connection,0,length+1);
			memcpy(hp->http_request.opt_connection,index,length);
		}
		else
		{
			hp->http_response.opt_connection=(char *)malloc(length+1);
			memset(hp->http_response.opt_connection,0,length+1);
			memcpy(hp->http_response.opt_connection,index,length);
		}
	}
	else if(strcmp(opt_name,"opt_cont_encoding")==0)
	{
		if(http_request)
		{
			hp->http_request.opt_cont_encoding=(char *)malloc(length+1);
			memset(hp->http_request.opt_cont_encoding,0,length+1);
			memcpy(hp->http_request.opt_cont_encoding,index,length);
		}
		else
		{
			hp->http_response.opt_cont_encoding=(char *)malloc(length+1);
			memset(hp->http_response.opt_cont_encoding,0,length+1);
			memcpy(hp->http_response.opt_cont_encoding,index,length);
		}
	}
	else if(strcmp(opt_name,"opt_language")==0)
	{
		if(http_request)
		{
			hp->http_request.opt_language=(char *)malloc(length+1);
			memset(hp->http_request.opt_language,0,length+1);
			memcpy(hp->http_request.opt_language,index,length);
		}
		else
		{
			hp->http_response.opt_language=(char *)malloc(length+1);
			memset(hp->http_response.opt_language,0,length+1);
			memcpy(hp->http_response.opt_language,index,length);
		}
	}
	else if(strcmp(opt_name,"opt_cont_location")==0)
	{
		if(http_request)
		{
			hp->http_request.opt_cont_location=(char *)malloc(length+1);
			memset(hp->http_request.opt_cont_location,0,length+1);
			memcpy(hp->http_request.opt_cont_location,index,length);
		}
		else
		{
			hp->http_response.opt_cont_location=(char *)malloc(length+1);
			memset(hp->http_response.opt_cont_location,0,length+1);
			memcpy(hp->http_response.opt_cont_location,index,length);
		}
	}
	else if(strcmp(opt_name,"opt_cont_range")==0)
	{
		if(http_request)
		{
			hp->http_request.opt_cont_range=(char *)malloc(length+1);
			memset(hp->http_request.opt_cont_range,0,length+1);
			memcpy(hp->http_request.opt_cont_range,index,length);
		}
		else
		{
			hp->http_response.opt_cont_range=(char *)malloc(length+1);
			memset(hp->http_response.opt_cont_range,0,length+1);
			memcpy(hp->http_response.opt_cont_range,index,length);
		}
	}
	else if(strcmp(opt_name,"opt_cont_length")==0)
	{
		if(http_request)
		{
			hp->http_request.opt_cont_length=(char *)malloc(length+1);
			memset(hp->http_request.opt_cont_length,0,length+1);
			memcpy(hp->http_request.opt_cont_length,index,length);
		}
		else
		{
			hp->http_response.opt_cont_length=(char *)malloc(length+1);
			memset(hp->http_response.opt_cont_length,0,length+1);
			memcpy(hp->http_response.opt_cont_length,index,length);
		}
	}
	else if(strcmp(opt_name,"opt_cont_range")==0)
	{
		if(http_request)
		{
			hp->http_request.opt_cont_range=(char *)malloc(length+1);
			memset(hp->http_request.opt_cont_range,0,length+1);
			memcpy(hp->http_request.opt_cont_range,index,length);
		}
		else
		{
			hp->http_response.opt_cont_range=(char *)malloc(length+1);
			memset(hp->http_response.opt_cont_range,0,length+1);
			memcpy(hp->http_response.opt_cont_range,index,length);
		}
	}
	else if(strcmp(opt_name,"opt_cont_type")==0)
	{
		if(http_request)
		{
			hp->http_request.opt_cont_type=(char *)malloc(length+1);
			memset(hp->http_request.opt_cont_type,0,length+1);
			memcpy(hp->http_request.opt_cont_type,index,length);
		}
		else
		{
			hp->http_response.opt_cont_type=(char *)malloc(length+1);
			memset(hp->http_response.opt_cont_type,0,length+1);
			memcpy(hp->http_response.opt_cont_type,index,length);
		}
	}
	else if(strcmp(opt_name,"opt_charset")==0)
	{
		if(http_request)
		{
			hp->http_request.opt_charset=(char *)malloc(length+1);
			memset(hp->http_request.opt_charset,0,length+1);
			memcpy(hp->http_request.opt_charset,index,length);
		}
		else
		{
			hp->http_response.opt_charset=(char *)malloc(length+1);
			memset(hp->http_response.opt_charset,0,length+1);
			memcpy(hp->http_response.opt_charset,index,length);
		}
	}
	else if(strcmp(opt_name,"opt_expires")==0)
	{
		if(http_request)
		{
			hp->http_request.opt_expires=(char *)malloc(length+1);
			memset(hp->http_request.opt_expires,0,length+1);
			memcpy(hp->http_request.opt_expires,index,length);
		}
		else
		{
			hp->http_response.opt_expires=(char *)malloc(length+1);
			memset(hp->http_response.opt_expires,0,length+1);
			memcpy(hp->http_response.opt_expires,index,length);
		}
	}
	else if(strcmp(opt_name,"opt_x_flash_version")==0)
	{
		if(http_request)
		{
			hp->http_request.opt_x_flash_version=(char *)malloc(length+1);
			memset(hp->http_request.opt_x_flash_version,0,length+1);
			memcpy(hp->http_request.opt_x_flash_version,index,length);
		}
		else
		{
			hp->http_response.opt_x_flash_version=(char *)malloc(length+1);
			memset(hp->http_response.opt_x_flash_version,0,length+1);
			memcpy(hp->http_response.opt_x_flash_version,index,length);
		}
	}
	else if(strcmp(opt_name,"opt_transfer_length")==0)
	{
		if(http_request)
		{
			hp->http_request.opt_transfer_length=(char *)malloc(length+1);
			memset(hp->http_request.opt_transfer_length,0,length+1);
			memcpy(hp->http_request.opt_transfer_length,index,length);
		}
		else
		{
			hp->http_response.opt_transfer_length=(char *)malloc(length+1);
			memset(hp->http_response.opt_transfer_length,0,length+1);
			memcpy(hp->http_response.opt_transfer_length,index,length);
		}
	}
	else if(strcmp(opt_name,"opt_http_body")==0)
	{
		if(http_request)
		{
			hp->http_request.opt_http_body=(char *)malloc(length+1);
			memset(hp->http_request.opt_http_body,0,length+1);
			memcpy(hp->http_request.opt_http_body,index,length);
		}
		else
		{
			hp->http_response.opt_http_body=(char *)malloc(length+1);
			memset(hp->http_response.opt_http_body,0,length+1);
			memcpy(hp->http_response.opt_http_body,index,length);
		}
	}
}

//[输入]*hp:存放解析内容的结构体；length:四元组选项的长度
void SetHttpTuple4list(HttpServerInfo *hp,int length)
{
	char src[32];
	char dst[256];

	sscanf(hp->common.OPT_TUPLE4_LIST,"%[^-]",src);
	sscanf(hp->common.OPT_TUPLE4_LIST,"%*[^-]-%s",dst);

    hp->common.OPT_CLIENT_IP=(char *)malloc(length+1);
    memset(hp->common.OPT_CLIENT_IP,0,length+1);

    hp->common.OPT_CLIENT_PORT=(char *)malloc(length+1);
    memset(hp->common.OPT_CLIENT_PORT,0,length+1);

    hp->common.OPT_SERVER_IP=(char *)malloc(length+1);
    memset(hp->common.OPT_SERVER_IP,0,length+1);

    hp->common.OPT_SERVER_PORT=(char *)malloc(length+1);
    memset(hp->common.OPT_SERVER_PORT,0,length+1);

	sscanf(src,"%[^:]",hp->common.OPT_SERVER_IP);
	sscanf(dst,"%[^:]",hp->common.OPT_CLIENT_IP);
	sscanf(src,"%*[^:]:%s",hp->common.OPT_SERVER_PORT);

	if(sscanf(dst,"%*[^:]:%[^,]",hp->common.OPT_CLIENT_PORT)==-1)
	{
		sscanf(dst, "%*[^:]:%s", hp->common.OPT_CLIENT_PORT);
	}
}
