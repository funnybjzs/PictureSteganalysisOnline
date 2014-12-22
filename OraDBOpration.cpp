/*
 * OraDBOpration.cpp
 *
 *  Created on: 2014-4-16
 *      Author: liulei
 */

#include "OraDBOpration.h"
#include "Config.h"

OraDBOpration::OraDBOpration()
{
	env=NULL;
	con=NULL;
	stmt=NULL;
}
bool OraDBOpration::Init()
{
	char oracledbuser[32]={0},oracledbpasswd[32]={0},oracledbip[32]={0},oracledbport[32]={0},oracledbsid[32]={0};
	GetPrivateProfileString("OracleDB","OracleDBUser","",oracledbuser,32,"./config.ini");
	GetPrivateProfileString("OracleDB","OracleDBPasswd","",oracledbpasswd,32,"./config.ini");
	GetPrivateProfileString("OracleDB","OracleDBIp","",oracledbip,32,"./config.ini");
	GetPrivateProfileString("OracleDB","OracleDBPort","",oracledbport,32,"./config.ini");
	GetPrivateProfileString("OracleDB","OracleDBSid","",oracledbsid,32,"./config.ini");
	if(InitOracleDB(oracledbuser,oracledbpasswd,oracledbip,oracledbport,oracledbsid))
		return true;
	else
		return false;

}

bool OraDBOpration::InitOracleDB(string user,string passwd,string dbip,string dbport,string dbsid)
{
	string db = dbip+":"+dbport+"/"+dbsid;
    env = Environment::createEnvironment("UTF8","UTF8");
	try
	{
		con = env->createConnection(user,passwd,db);
	}
	catch(SQLException &ex)
	{
		cout<<"Connect DB Error! Error number: "<<ex.getErrorCode()<<" "<<"Error Msg: "<<ex.getMessage()<<endl;
		return false;
	}
	return true;
}

OraDBOpration::~OraDBOpration() 
{
	if(con!=NULL)
	{
		env->terminateConnection(con);
	}
	if(env!=NULL)
	{
		Environment::terminateEnvironment(env);
	}

}
/*
 * @sql_statement SQL语句字符串
 */
bool OraDBOpration::ExecuteNonQuery(string sql_statement)
{
    
    Statement *stmtement = con->createStatement();
    
    try
    {
        stmtement->setSQL(sql_statement);
        stmtement->executeUpdate();
		//con->commit();
    }

    catch (SQLException &ex)
    {
		con->rollback();
        cout << "Exception thrown for NonQuery" << endl;
        cout << "Error number: " << ex.getErrorCode() << endl;
        cout << "Error Msg: "<< ex.getMessage() << endl;
        cout << "SQL: "<<sql_statement << endl;
		con->terminateStatement(stmtement);
		return false;

    }
    con->terminateStatement(stmtement);
	return true;
}
ResultSet *OraDBOpration::ExecuteQuery(string sql_statement)
{	
	stmt = con->createStatement();
	ResultSet *rs=NULL;

	try
	{
		stmt->setSQL(sql_statement);
		rs=stmt->executeQuery();

	}
	catch(SQLException &ex)
	{
		con->rollback();
        cout << "Exception thrown for NonQuery" << endl;
        cout << "Error number: " << ex.getErrorCode() << endl;
        cout << "Error Msg: "<< ex.getMessage() << endl;
        cout << "SQL: "<<sql_statement << endl;
	}
	return rs;
}
void OraDBOpration::CloseResultSet(ResultSet *rs)
{
	if(rs !=NULL)
	{
		stmt->closeResultSet(rs);
		con->terminateStatement(stmt);
	}
}

