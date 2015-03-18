/*
 * ParseBody.c
 *
 *  Created on: 2014年6月3日
 *      Author: zhaoshuo
 */
#include "Parse.h"

// [输入]data:待解析的数据；*Head:预先定义的应用层数据结构头
// [输出]length:返回整个数据头的长度
int ParseHead(char *data, AppData_Head *head) {
	int frag_length;       //分段长度
	int frag_indicator = 0; //分段序号标记
	char *p = data;
	char content[100];  //分段内容
	int total_length = 0; //总长度

	InitAppHead(head);

	while (frag_indicator < APPDATA_HEARD_STARTNUM + APPDATA_HEARD_OPTNUM) {
		sscanf(p, "%s%n", content, &frag_length);           //得到分片数据和其长度
		//printf("%s %d\n",content,frag_length);

		if (frag_indicator < APPDATA_HEARD_STARTNUM)       //开始行
		{
			head->option[frag_indicator] = (char *)malloc(frag_length + 1);
			memset(head->option[frag_indicator], 0, frag_length + 1);
			strcpy(head->option[frag_indicator], content);
			//printf("%s\n",head->option[frag_indicator]);
			//free(head->option[frag_indicator]);
		} else {
			head->option[frag_indicator] =(char *)malloc(frag_length + 1);
			memset(head->option[frag_indicator], 0, frag_length + 1);
			sscanf(content, "%*[^:]:%[^\r\n]", head->option[frag_indicator]);
			//printf("%s\n",head->option[frag_indicator]);
		}

		p = p + frag_length;
		frag_indicator++;
		total_length+=frag_length;

		if((*(p+2)=='\r')&&(*(p+3)=='\n'))
		{
			break;
		}
	}

//	int m;
//	printf("head...\n");
//	for(m=0;m++;m<=32)
//	{
//		printf("%s",data[m]);
//	}
//	printf("..............\n");

		return total_length + 4;

}

//[输入]buffer:待解析的数据，opt_num:选项数量
//[输出]无
void ParseBody(char *buffer, int opt_num) {
//	int m;
//	printf("body...\n");
//	for(m=0;m++;m<=32)
//	{
//		printf("%s",buffer[m]);
//	}
//	printf(".............\n");
	//首先判断服务类型
	char *service = NULL;
	service = GetServiceType(buffer);

	if (service != NULL) {
	//	cout<<"应用层数据解析OK!"<<endl;
	//	cout<<"服务类型 :"<<service<<endl;

		if (strcmp(service, "SERVICE_HTTP") == 0) {
		         //	HttpParse(buffer, opt_num);
		}
		else if (strcmp(service, "SERVICE_SMTP") == 0) {
					cout<<"应用层数据解析OK!"<<endl;
					cout<<"服务类型 :"<<service<<endl;
			       SmtpParse(buffer, opt_num);
			       cout<<"-----------------------解析完成 !"<<endl;
		}
		else  //其它应用层数据类型
		{
		//	cout << "-----其它类型应用层数据------" << endl;
		}

	//	cout<<"-----------------------解析完成 !"<<endl;
	}

	free(service);
}

//[输入]data:待解析的数据
//[输出]char*:返回服务类型
char* GetServiceType(char *data) {
	char *index = data;
	char eachline[1024];        //每行内容
	int line_length;            //每行长度，用于统计指针偏移
	char opt_name[50];          //选项名称
	char opt_len[32];          //选项长度

	//选项名称
	sscanf(index, "%s%n", eachline, &line_length);
	sscanf(eachline, "%*[^:]:%[^\r\n]", opt_name);
	if(strcmp(opt_name,"OPT_SERVICE")==0)
	{
		index = index + line_length + 2;

		//选项内容长度
		sscanf(index, "%s%n", eachline, &line_length);
		sscanf(eachline, "%*[^:]:%[^\r\n]", opt_len);
		index = index + line_length + 2;

		//选项内容,是否考虑用calloc;
		char *service = (char *)malloc(atoi(opt_len)+1);
		memset(service, 0, atoi(opt_len) + 1);
		memcpy(service, index, atoi(opt_len));
		return service;
	}
	else
	{
		return NULL;
	}
}

void InitHttpServerInfo(HttpServerInfo *hsi) {
	hsi->common.OPT_CAP_TIME = NULL;
	hsi->common.OPT_CLIENT_IP = NULL;
	hsi->common.OPT_CLIENT_PORT = NULL;
	hsi->common.OPT_COOKIE = NULL;
	hsi->common.OPT_DATE = NULL;
	hsi->common.OPT_FILE_CONTENT = NULL;
	hsi->common.OPT_FILE_NAME = NULL;
	hsi->common.OPT_FILE_SET = NULL;
	hsi->common.OPT_FLOW_ID_LIST = NULL;
	hsi->common.OPT_PASSWORD = NULL;
	hsi->common.OPT_PROTOCOL = NULL;
	hsi->common.OPT_REFERER = NULL;
	hsi->common.OPT_SERVER_IP = NULL;
	hsi->common.OPT_SERVER_PORT = NULL;
	hsi->common.OPT_SERVICE = NULL;
	hsi->common.OPT_SUB_SERVICE = NULL;
	hsi->common.OPT_TUPLE4_LIST = NULL;
	hsi->common.OPT_URL = NULL;
	hsi->common.OPT_USER_NAME = NULL;

	hsi->http_request.OPT_COOKIE = NULL;
	hsi->http_request.OPT_DATE = NULL;
	hsi->http_request.OPT_REFERER = NULL;
	hsi->http_request.OPT_URL = NULL;
	hsi->http_request.opt_authorization = NULL;
	hsi->http_request.opt_charset = NULL;
	hsi->http_request.opt_connection = NULL;
	hsi->http_request.opt_cont_encoding = NULL;
	hsi->http_request.opt_cont_length = NULL;
	hsi->http_request.opt_cont_location = NULL;
	hsi->http_request.opt_cont_range = NULL;
	hsi->http_request.opt_cont_type = NULL;
	hsi->http_request.opt_etag = NULL;
	hsi->http_request.opt_expires = NULL;
	hsi->http_request.opt_http_body = NULL;
	hsi->http_request.opt_http_server = NULL;
	hsi->http_request.opt_language = NULL;
	hsi->http_request.opt_location = NULL;
	hsi->http_request.opt_method = NULL;
	hsi->http_request.opt_pragma = NULL;
	hsi->http_request.opt_proxy_authorization = NULL;
	hsi->http_request.opt_response_code = NULL;
	hsi->http_request.opt_traile = NULL;
	hsi->http_request.opt_transfer_encoding = NULL;
	hsi->http_request.opt_transfer_length = NULL;
	hsi->http_request.opt_user_agent = NULL;
	hsi->http_request.opt_via = NULL;
	hsi->http_request.opt_x_flash_version = NULL;

	hsi->http_response.OPT_COOKIE = NULL;
	hsi->http_response.OPT_DATE = NULL;
	hsi->http_response.OPT_REFERER = NULL;
	hsi->http_response.OPT_URL = NULL;
	hsi->http_response.opt_authorization = NULL;
	hsi->http_response.opt_charset = NULL;
	hsi->http_response.opt_connection = NULL;
	hsi->http_response.opt_cont_encoding = NULL;
	hsi->http_response.opt_cont_length = NULL;
	hsi->http_response.opt_cont_location = NULL;
	hsi->http_response.opt_cont_range = NULL;
	hsi->http_response.opt_cont_type = NULL;
	hsi->http_response.opt_etag = NULL;
	hsi->http_response.opt_expires = NULL;
	hsi->http_response.opt_http_body = NULL;
	hsi->http_response.opt_http_server = NULL;
	hsi->http_response.opt_language = NULL;
	hsi->http_response.opt_location = NULL;
	hsi->http_response.opt_method = NULL;
	hsi->http_response.opt_pragma = NULL;
	hsi->http_response.opt_proxy_authorization = NULL;
	hsi->http_response.opt_response_code = NULL;
	hsi->http_response.opt_traile = NULL;
	hsi->http_response.opt_transfer_encoding = NULL;
	hsi->http_response.opt_transfer_length = NULL;
	hsi->http_response.opt_user_agent = NULL;
	hsi->http_response.opt_via = NULL;
	hsi->http_response.opt_x_flash_version = NULL;

}

void FreeHttpServerInfo(HttpServerInfo *hsi) {
	free(hsi->common.OPT_CAP_TIME);
	free(hsi->common.OPT_CLIENT_IP);
	free(hsi->common.OPT_CLIENT_PORT);
	free(hsi->common.OPT_COOKIE);
	free(hsi->common.OPT_DATE);
	free(hsi->common.OPT_FILE_CONTENT);
	free(hsi->common.OPT_FILE_NAME);
	free(hsi->common.OPT_FILE_SET);
	free(hsi->common.OPT_FLOW_ID_LIST);
	free(hsi->common.OPT_PASSWORD);
	free(hsi->common.OPT_PROTOCOL);
	free(hsi->common.OPT_REFERER);
	free(hsi->common.OPT_SERVER_IP);
	free(hsi->common.OPT_SERVER_PORT);
	free(hsi->common.OPT_SERVICE);
	free(hsi->common.OPT_SUB_SERVICE);
	free(hsi->common.OPT_TUPLE4_LIST);
	free(hsi->common.OPT_URL);
	free(hsi->common.OPT_USER_NAME);

	free(hsi->http_request.OPT_COOKIE);
	free(hsi->http_request.OPT_DATE);
	free(hsi->http_request.OPT_REFERER);
	free(hsi->http_request.OPT_URL);
	free(hsi->http_request.opt_authorization);
	free(hsi->http_request.opt_charset);
	free(hsi->http_request.opt_connection);
	free(hsi->http_request.opt_cont_encoding);
	free(hsi->http_request.opt_cont_length);
	free(hsi->http_request.opt_cont_location);
	free(hsi->http_request.opt_cont_range);
	free(hsi->http_request.opt_cont_type);
	free(hsi->http_request.opt_etag);
	free(hsi->http_request.opt_expires);
	free(hsi->http_request.opt_http_body);
	free(hsi->http_request.opt_http_server);
	free(hsi->http_request.opt_language);
	free(hsi->http_request.opt_location);
	free(hsi->http_request.opt_method);
	free(hsi->http_request.opt_pragma);
	free(hsi->http_request.opt_proxy_authorization);
	free(hsi->http_request.opt_response_code);
	free(hsi->http_request.opt_traile);
	free(hsi->http_request.opt_transfer_encoding);
	free(hsi->http_request.opt_transfer_length);
	free(hsi->http_request.opt_user_agent);
	free(hsi->http_request.opt_via);
	free(hsi->http_request.opt_x_flash_version);

	free(hsi->http_response.OPT_COOKIE);
	free(hsi->http_response.OPT_DATE);
	free(hsi->http_response.OPT_REFERER);
	free(hsi->http_response.OPT_URL);
	free(hsi->http_response.opt_authorization);
	free(hsi->http_response.opt_charset);
	free(hsi->http_response.opt_connection);
	free(hsi->http_response.opt_cont_encoding);
	free(hsi->http_response.opt_cont_length);
	free(hsi->http_response.opt_cont_location);
	free(hsi->http_response.opt_cont_range);
	free(hsi->http_response.opt_cont_type);
	free(hsi->http_response.opt_etag);
	free(hsi->http_response.opt_expires);
	free(hsi->http_response.opt_http_body);
	free(hsi->http_response.opt_http_server);
	free(hsi->http_response.opt_language);
	free(hsi->http_response.opt_location);
	free(hsi->http_response.opt_method);
	free(hsi->http_response.opt_pragma);
	free(hsi->http_response.opt_proxy_authorization);
	free(hsi->http_response.opt_response_code);
	free(hsi->http_response.opt_traile);
	free(hsi->http_response.opt_transfer_encoding);
	free(hsi->http_response.opt_transfer_length);
	free(hsi->http_response.opt_user_agent);
	free(hsi->http_response.opt_via);
	free(hsi->http_response.opt_x_flash_version);
}

void InitAppHead(AppData_Head *head)
{
	int i;
	for (i = 0; i < APPDATA_HEARD_OPTNUM + APPDATA_HEARD_STARTNUM; i++) {
			head->option[i]=NULL;
	}
}

void FreeAppHead(AppData_Head *head) {
	int i;
	for (i = 0; i < APPDATA_HEARD_OPTNUM + APPDATA_HEARD_STARTNUM; i++) {
		if(head->option[i]!=NULL)
		{
			free(head->option[i]);
			head->option[i]=NULL;
		}
	}
}

int HasResponseOpt(HttpServerInfo *hsi) {
	return !(hsi->http_response.OPT_COOKIE == NULL
			&& hsi->http_response.OPT_DATE == NULL
			&& hsi->http_response.OPT_REFERER == NULL
			&& hsi->http_response.OPT_URL == NULL
			&& hsi->http_response.opt_authorization == NULL
			&& hsi->http_response.opt_charset == NULL
			&& hsi->http_response.opt_connection == NULL
			&& hsi->http_response.opt_cont_encoding == NULL
			&& hsi->http_response.opt_cont_length == NULL
			&& hsi->http_response.opt_cont_location == NULL
			&& hsi->http_response.opt_cont_range == NULL
			&& hsi->http_response.opt_cont_type == NULL
			&& hsi->http_response.opt_etag == NULL
			&& hsi->http_response.opt_expires == NULL
			&& hsi->http_response.opt_http_body == NULL
			&& hsi->http_response.opt_http_server == NULL
			&& hsi->http_response.opt_language == NULL
			&& hsi->http_response.opt_location == NULL
			&& hsi->http_response.opt_method == NULL
			&& hsi->http_response.opt_pragma == NULL
			&& hsi->http_response.opt_proxy_authorization == NULL
			&& hsi->http_response.opt_response_code == NULL
			&& hsi->http_response.opt_traile == NULL
			&& hsi->http_response.opt_transfer_encoding == NULL
			&& hsi->http_response.opt_transfer_length == NULL
			&& hsi->http_response.opt_user_agent == NULL
			&& hsi->http_response.opt_via == NULL
			&& hsi->http_response.opt_x_flash_version == NULL);
}

void InitMailServerInfo(MailServerInfo *mf){
	mf->common.OPT_CAP_TIME = NULL;
	mf->common.OPT_CLIENT_IP = NULL;
	mf->common.OPT_CLIENT_PORT = NULL;
	mf->common.OPT_COOKIE = NULL;
	mf->common.OPT_DATE = NULL;
	mf->common.OPT_FILE_CONTENT = NULL;
	mf->common.OPT_FILE_NAME = NULL;
	mf->common.OPT_FILE_SET = NULL;
	mf->common.OPT_FLOW_ID_LIST = NULL;
	mf->common.OPT_PASSWORD = NULL;
	mf->common.OPT_PROTOCOL = NULL;
	mf->common.OPT_REFERER = NULL;
	mf->common.OPT_SERVER_IP = NULL;
	mf->common.OPT_SERVER_PORT = NULL;
	mf->common.OPT_SERVICE = NULL;
	mf->common.OPT_SUB_SERVICE = NULL;
	mf->common.OPT_TUPLE4_LIST = NULL;
	mf->common.OPT_URL = NULL;
	mf->common.OPT_USER_NAME = NULL;

	mf->mail.opt_mail_content=NULL;
	mf->mail.opt_mail_from=NULL;
	mf->mail.opt_mail_to=NULL;
	mf->mail.opt_subject=NULL;

}

void FreeMailServerInfo(MailServerInfo *mf){

	free(mf->common.OPT_CAP_TIME);
	free(mf->common.OPT_CLIENT_IP);
	free(mf->common.OPT_CLIENT_PORT);
	free(mf->common.OPT_COOKIE);
	free(mf->common.OPT_DATE);
	free(mf->common.OPT_FILE_CONTENT);
	free(mf->common.OPT_FILE_NAME);
	free(mf->common.OPT_FILE_SET);
	free(mf->common.OPT_FLOW_ID_LIST);
	free(mf->common.OPT_PASSWORD);
	free(mf->common.OPT_PROTOCOL);
	free(mf->common.OPT_REFERER);
	free(mf->common.OPT_SERVER_IP);
	free(mf->common.OPT_SERVER_PORT);
	free(mf->common.OPT_SERVICE);
	free(mf->common.OPT_SUB_SERVICE);
	free(mf->common.OPT_TUPLE4_LIST);
	free(mf->common.OPT_URL);
	free(mf->common.OPT_USER_NAME);

	free(mf->mail.opt_mail_content);
	free(mf->mail.opt_mail_from);
	free(mf->mail.opt_mail_to);
	free(mf->mail.opt_subject);

	if(mf->mail.AttachFileNames.size()!=0)
	{
		for(int i=0;i<mf->mail.AttachFileNames.size();i++)
		{
			free(mf->mail.AttachFileNames[i]);
			free(mf->mail.AttachFileContent[i]);
		}

		mf->mail.AttachFileNames.clear();
		mf->mail.AttachFileLength.clear();
		mf->mail.AttachFileContent.clear();
		mf->mail.AnalysisResults.clear();
	}

}

