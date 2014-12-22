/*
 * AppDataDefine.h
 *
 *  Created on: 2014年6月3日
 *      Author: zhaoshuo
 */

#ifndef APPDATADEFINE_H_
#define APPDATADEFINE_H_
#include  <vector>
using namespace std;
//应用层数据消息头：开始行3个域的数据，其它包括7个域的数据
#define APPDATA_HEARD_STARTNUM 3
#define APPDATA_HEARD_OPTNUM 7

//应用层数据消息头
typedef struct{
	char *option[APPDATA_HEARD_OPTNUM+APPDATA_HEARD_STARTNUM];
} AppData_Head ;

//应答码及描述定义
const static char RESOPONSE_MSG[2][3][50]={{"200","400","500"},{"OK","Bad Request","Internal Server Error"}};
//应用层数据消息头-选项名称枚举
typedef enum {
	Start_Method,
	Start_Type,
	Start_Version,
	Msg_Type,Data_Seq, CAP_IP, TRANS_FLAG, OPT_NUM, CONT_LENGTH, HIT_ID_LIST
}Head_Option;

//应用层数据体-通用选项
typedef struct{
	char *OPT_PROTOCOL;
	char *OPT_CLIENT_IP;
	char *OPT_CLIENT_PORT;
	char *OPT_SERVER_IP;
	char *OPT_SERVER_PORT;
	char *OPT_SERVICE;
	char *OPT_SUB_SERVICE;
	char *OPT_CAP_TIME;
	char *OPT_URL;
	char *OPT_COOKIE;
	char *OPT_REFERER;
	char *OPT_USER_NAME;
	char *OPT_PASSWORD;
	char *OPT_DATE;
	char *OPT_FILE_NAME;
	char *OPT_FILE_CONTENT;
	char *OPT_FILE_SET;
	char *OPT_FLOW_ID_LIST;
	char *OPT_TUPLE4_LIST;
}Common_Option;

//应用层数据体-Http专用选项 (大写是与通用选项重复的选项，小写是专用选项）
typedef	struct {
	char *OPT_URL;
	char *opt_method;
	char *OPT_REFERER;
	char *opt_user_agent;
	char *opt_proxy_authorization;
	char *opt_authorization;
	char *OPT_COOKIE;
	char *opt_location;
	char *opt_http_server;
	char *opt_etag;
	char *opt_response_code;
	char *OPT_DATE;
	char *opt_traile;
	char *opt_transfer_encoding;
	char *opt_via;
	char *opt_pragma;
	char *opt_connection;
	char *opt_cont_encoding;
	char *opt_language;
	char *opt_cont_location;
	char *opt_cont_range;
	char *opt_cont_length;
	char *opt_cont_type;
	char *opt_charset;
	char *opt_expires;
	char *opt_x_flash_version;
	char *opt_transfer_length;
	char *opt_http_body;
}Http_Option;

//应用层数据体-Http服务
typedef struct {
	Common_Option common;
	Http_Option http_request;
	Http_Option http_response;
}HttpServerInfo;

//应用层数据体-Mail服务专用选项
typedef struct {
	char *opt_mail_from;
	char *opt_mail_to;
	char *opt_subject;
	char *opt_mail_content;
	//附件集
	vector<char *> AttachFileNames;
	vector<int> AttachFileLength;
	vector<char *>AttachFileContent;
	//检测结果
	vector<double> AnalysisResults;
}Mail_Option;

typedef struct {
	Common_Option common;
	Mail_Option mail;
}MailServerInfo;

#endif /* APPDATADEFINE_H_ */
