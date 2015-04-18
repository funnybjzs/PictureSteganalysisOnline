/*
 * QTDatabase.cpp
 *
 *  Created on: 2014年10月29日
 *      Author: zhaoshuo
 */

#include "QTDatabase.h"
#include "systemall.h"

bool GetFormatTime(char *time,char *format_time)
{
	string months[12]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
	char getime[32]={0};

	bool result=false;

	//cout<<time<<endl;
	if(sscanf(time," %*[^0-3]%[^+]",getime)==-1)
	{
		perror("sscanf time fail !");
		return result;
	}
	char y[8]={0},m[8]={0},d[8]={0},t[16]={0};

	if(sscanf(getime,"%2s%3s%4s%s",d,m,y,t)==-1)
	{
		perror("sscanf ymdt fail!");
		return result;
	}

	for(int i=0;i<12;i++)
	{
		if(m==months[i])
		{
			char f_m[8];
			sprintf(f_m,"%d",i+1);
			sprintf(format_time,"%s-%s-%s %s",y,f_m,d,t);
			result = true;
		}
	}

	return result;
}
//通用解析。处理文件名，从右边过滤文件类型
char*GetFileFormat(char *s1,char const *s2)
{
	register char *last;
	register char *current;

	last=NULL;

	if (*s2!='\0') {
		current = strstr(s1,s2);

		while(current!=NULL)
		{
			last=current;
			current=strstr(last+1,s2);
		}
	}

	return last;
}

QTDatabase::QTDatabase()
{
	env=NULL;
	conn=NULL ;
	st=NULL;
}

void QTDatabase::InitDB(char *xmlfile)
{
	StegoConfig scfg(xmlfile);
	if(!(scfg.GetDBInfo(dbinfo)&&scfg.GetValue("Database","User",username_)&&scfg.GetValue("Database","Passwd",password_)))
	{
		cout<<"配置文件数据库基本信息错误!"<<endl;
		exit(1);
	}

	char tmp1[8]={0},tmp2[8]={0};
	if(!(scfg.GetValue("Database","EquId",tmp1)&&scfg.GetValue("Database","ATypeId",tmp2)))
	{
		cout<<"配置文件数据库表信息错误!"<<endl;
		exit(1);
	}

	this->qt_equ_info_id=atoi(tmp1);
	this->qt_alert_type_id=atoi(tmp2);

}

bool QTDatabase::Init(char  *xmlfile)
{
	InitDB(xmlfile);
	try
	{
         env=Environment::createEnvironment(Environment::DEFAULT);
		 cout<<"数据库资源初始化OK !" <<endl;

		 conn = env->createConnection(username_,password_,dbinfo);
		 cout<<"数据库连接初始化OK !" <<endl;

		 st=conn->createStatement();
		 cout<<"数据库SQL语句初始化OK!"<<endl;

		 cout<<"----------数据库环境准备OK !-----------"<<endl;

		 srand((unsigned)time(NULL));
		 return true	;
	}
	catch(SQLException e)
	{
	     cout<<e.what()<<endl;

	     return false;
	}
}

bool QTDatabase::Release()
{
	try
	{
		  conn->terminateStatement(st);
	      env->terminateConnection(conn);
	      Environment::terminateEnvironment(env);
	      cout<<"----------Database Environment Release Finish !-----------"<<endl;
	      return true;
	}
	catch (SQLException e)
	{
		  cout<<e.what()<<endl;
		  return false;
	}
}

void QTDatabase::CloseResultSet(ResultSet * rs)
{
	if(rs !=NULL)
	{
		st->closeResultSet(rs);
		//conn->terminateStatement(st);
	}
}

bool QTDatabase::ExecuteNonQuery(string sql_statement)
{
    try
    {
        st->setSQL(sql_statement);
        st->executeUpdate();
		//con->commit();
    }

    catch (SQLException &ex)
    {
		conn->rollback();
        cout << "Exception thrown for NonQuery" << endl;
        cout << "Error number: " << ex.getErrorCode() << endl;
        cout << "Error Msg: "<< ex.getMessage() << endl;
        cout << "SQL: "<<sql_statement << endl;
		//conn->terminateStatement(st);
		return false;

    }
    //conn->terminateStatement(st);
	return true;
}

ResultSet *QTDatabase::ExecuteQuery(string sql_statement)
{
	ResultSet *rs=NULL;

	try
	{
		st->setSQL(sql_statement);
		rs=st->executeQuery();

	}
	catch(SQLException &ex)
	{
		conn->rollback();
        cout << "Exception thrown for NonQuery" << endl;
        cout << "Error number: " << ex.getErrorCode() << endl;
        cout << "Error Msg: "<< ex.getMessage() << endl;
        cout << "SQL: "<<sql_statement << endl;
	}
	return rs;
}

string QTDatabase::set_qt_file(const MailServerInfo & msi,int index)
{
		char sql_image[512];
		sprintf(sql_image,"insert into QT_IMAGE_INFO(IMAGE_ID,IMAGE_DEC_VAL) values(IMAGE_ID_SEQ.nextval,%f)",msi.mail.LevelResults[index]);
		if(!ExecuteNonQuery(sql_image))
		{
			exit(1);
		}
		ResultSet *rs1=ExecuteQuery ("select IMAGE_ID_SEQ.currval from dual");
		int img_info_id;
		while(rs1->next())
		{
			img_info_id=rs1->getInt(1);
		}

		char sql[1024];
		char *file_type = GetFileFormat(msi.mail.AttachFileNames[index], ".");

		if(strcmp(file_type,".jpg")==0)
		{
			sprintf(sql,"insert into QT_FILE(FILE_ID,FILE_NAME,FILE_LEN,FILE_TYPE,FILE_PATH,FILE_STORAGE_NAME,FILE_DETAIL_TABLE,FILE_DETAIL_TABLE_ID) "
					" values(FILE_ID_SEQ.nextval,'%s',%d,%d,'%s','%s','%s',%d)",msi.mail.AttachFileNames[index],msi.mail.AttachFileLength[index],16,PICTURE_TO_STORE,
					msi.mail.AttachFileStoredNames[index].c_str(),"QT_IMAGE_INFO",img_info_id);
		}
		else if(strcmp(file_type,".png")==0)
		{
			sprintf(sql,"insert into QT_FILE(FILE_ID,FILE_NAME,FILE_LEN,FILE_TYPE,FILE_PATH,FILE_STORAGE_NAME,FILE_DETAIL_TABLE,FILE_DETAIL_TABLE_ID) "
					" values(FILE_ID_SEQ.nextval,'%s',%d,%d,'%s','%s','%s',%d)",msi.mail.AttachFileNames[index],msi.mail.AttachFileLength[index],33,PICTURE_TO_STORE,
					msi.mail.AttachFileStoredNames[index].c_str(),"QT_IMAGE_INFO",img_info_id);
		}
		else
		{
			;
		}

		if(!ExecuteNonQuery(sql))
		{
			exit(1);
		}

		string file_id;
		ResultSet *rs2=ExecuteQuery("select FILE_ID_SEQ.currval from dual");
		while(rs2->next())
		{
			file_id=rs2->getString(1);
		}

		//CloseResultSet(rs1);
		//CloseResultSet(rs2);
		return file_id;
}

int QTDatabase::set_qt_service(const MailServerInfo &ml,string fileid)
{
		char sql[1024];
		int file_num=0;
		for(int i=0;i<ml.mail.AttachFileNames.size();i++)
		{
			if	(ml.mail.AnalysisResults[i]==1)
			{
					file_num++;
			}
		}
		char mail_time[32]; //转换时间戳格式
		const char *fids=fileid.c_str();
		sprintf(sql,"insert into QT_SERVICE(SERVICE_ID,SERVICE_TYPE,FILE_NUM,FILE_ID,MAIL_FROM,MAIL_TO,MAIL_SUBJECT,MAIL_TIME) "
				"values(SERVICE_ID_SEQ.nextval,%d,%d,'%s','%s','%s','%s',to_date('%s','yyyy-mm-dd hh24:mi:ss'))",31,file_num,fids,
				ml.mail.opt_mail_from,ml.mail.opt_mail_to,ml.mail.opt_subject,(GetFormatTime(ml.common.OPT_DATE,mail_time)==TRUE)?mail_time:NULL);

		if(!ExecuteNonQuery(sql))
		{
			exit(1);
		}

		int service_id;
		ResultSet *rs=ExecuteQuery("select SERVICE_ID_SEQ.currval from dual");
		while(rs->next())
		{
			service_id=rs->getInt(1);
		}
		//CloseResultSet(rs);
		return service_id;
}

void QTDatabase::set_qt_alert(const MailServerInfo &ml,int service_id)
{
	char sql[1024];
	string info;
	for(int i=0;i<ml.mail.AttachFileNames.size();i++)
	{
		if	(ml.mail.AnalysisResults[i]==1)
		{
			string tmp=ml.mail.AttachFileNames[i];
			info+=tmp;
			info+=",";
		}
	}
	info.erase(info.end()-1);
	info="发现"+info+"中包含隐写信息!";
	const char *des=info.c_str();

	sprintf(sql,"insert into QT_ALERT(ALERT_ID,EQU_ID,OCCUR_TIME,ALERT_LEVEL_ID,ALERT_TYPE_ID,ALERT_DESCRIPTION,CERTAINY_FACTOR,SIG_ID,OPERATE_STAGE,JUDGE_RESULT,"
			"FLOW_ID,SERVICE_TYPE,SERVICE_ID,SRC_IP,DST_IP,SRC_PORT,DST_PORT) values(ALERT_ID_SEQ.nextval,%d,(select sysdate from dual),%d,%d,'%s',%d,%d,%d,%d,'%s',%d,%d,'%s','%s',%d,%d)",
			qt_equ_info_id,3,qt_alert_type_id,des,1,0,1,0,ml.common.OPT_FLOW_ID_LIST,31,service_id,
			ml.common.OPT_SERVER_IP,ml.common.OPT_CLIENT_IP,atoi(ml.common.OPT_SERVER_PORT),atoi(ml.common.OPT_CLIENT_PORT));

	if(!ExecuteNonQuery(sql))
	{
		exit(1);
	}
}

void QTDatabase::Commit()
{
	conn->commit();
}


