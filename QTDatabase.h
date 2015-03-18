/*
 * QTDatabase.h
 *
 *  Created on: 2014年10月29日
 *      Author: zhaoshuo
 */

#ifndef QTDATABASE_H_
#define QTDATABASE_H_

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "AppDataDefine.h"
#include "occi.h"
#include "StegoConfig.h"

using namespace std;
using namespace oracle::occi;

class QTDatabase
{
	public :
		QTDatabase(char  *xmlfile);
		virtual ~QTDatabase(){};

		bool  Init();
		bool  Release();
		string  set_qt_file(char *file_name,int file_len,char *file_type,char *file_path);
		int set_qt_alert_type();
		int set_qt_service(const MailServerInfo &ml,string id);
		void set_qt_alert(const MailServerInfo &ml,int service_id,int i);
		void Commit();

	private:
		//物理信息
		char dbinfo[32];
		char  username_[16];
		char  password_[16];

		//数据库环境句柄
		 Environment *env;
		Connection *conn ;
		Statement *st;

		//设备信息表id
		int	qt_equ_info_id;
		//告警类型表id
		int qt_alert_type_id;

};



#endif /* QTDATABASE_H_ */
