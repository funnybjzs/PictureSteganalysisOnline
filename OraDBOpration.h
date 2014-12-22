/*
 * OraDBOpration.h
 *
 *  Created on: 2014-4-16
 *      Author: liulei
 */

#ifndef ORADBOPRATION_H_
#define ORADBOPRATION_H_

#include "occi.h"
#include <iostream>
using namespace oracle::occi;
using namespace std;


class OraDBOpration {

public:

	//初始化对象 连接数据库
	OraDBOpration();

	bool Init();
	bool InitOracleDB(string user,string passwd,string dbip,string dbport,string dbname);
	
	bool ExecuteNonQuery(string sql_statement);
	ResultSet *ExecuteQuery(string sql_statement);
	void CloseResultSet(ResultSet *rs);
	
	virtual ~OraDBOpration();


private:
	  Environment *env;
	  Connection  *con;
	  Statement   *stmt;
};


#endif /* ORADBOPRATION_H_ */
