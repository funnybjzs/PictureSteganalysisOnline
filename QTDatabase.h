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
		QTDatabase();
		virtual ~QTDatabase(){};

		void InitDB(char  *xmlfile);
		bool Init(char  *xmlfile);
		bool  Release();

		bool ExecuteNonQuery(string sql_statement);
		ResultSet *ExecuteQuery(string sql_statement);
		void CloseResultSet(ResultSet *rs);

		string  set_qt_file(const MailServerInfo & msi,int index);
		int set_qt_alert_type();
		int set_qt_service(const MailServerInfo &ml,string id);
		void set_qt_alert(const MailServerInfo &ml,int service_id);
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
