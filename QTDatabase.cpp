/*
 * QTDatabase.cpp
 *
 *  Created on: 2014年10月29日
 *      Author: zhaoshuo
 */

#include "QTDatabase.h"

bool GetFormatTime(char *time,char *format_time)
{
	string months[12]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
	char getime[32];

	bool result=false;

	if(sscanf(time," %*[^ ] %[^+]",getime)!=1)
	{
		perror("sscanf fail !");
		return result;
	}
	char y[8],m[8],d[8],t[16];

	if(sscanf(getime,"%[^ ] %[^ ] %[^ ] %[^ ] ",d,m,y,t)!=4)
	{
		perror("sscanf fail !");
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

bool QTDatabase::Init()
{
	//初始化数据库环境
	char connection[64];
	sprintf(connection, "%s:%s/%s", host_, port_, service_);

	try
	{
         env=Environment::createEnvironment(Environment::DEFAULT);
		 cout<<"Database Environment Init OK !" <<endl;

		 conn = env->createConnection(username_, password_,connection);
		 cout<<"Database Connection Init OK !" <<endl;

		 st=conn->createStatement();
		 cout<<"Database SQL Init OK!"<<endl;

		 cout<<"----------Database Environment Init Finish !-----------"<<endl;

		 //获取设备信息表id,需要综合管理界面配好之后，人工告知，也可以通过数据库EQU_IP查询
		 qt_equ_info_id=10;  //人工添加设备后得到的id
		 qt_alert_type_id=set_qt_alert_type();

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

string QTDatabase::set_qt_file(char *file_name,int file_len,char *file_type,char *file_path)
{
		try
		{
			st->setSQL("insert into QT_FILE(FILE_ID,FILE_NAME,FILE_LEN,FILE_TYPE,FILE_PATH,FILE_DESCRIPTION)  values(FILE_ID_SEQ.nextval,:1,:2,:3,:4,:5)");

			st->setString(1, file_name);
			st->setInt(2, file_len);
			if(strcmp(file_type,".jpg")==0)
			{
				st->setInt(3,16);
			}
			else if(strcmp(file_type,".png")==0)
			{
				st->setInt(3,33);
			}
			else
			{
				st->setInt(3,-1);
			}
			st->setString(4, file_path);
			st->setString(5,"this is a stego picture");

			st->executeUpdate();
//	       conn->commit();

			string  file_id;
			ResultSet *rs=st->executeQuery("select FILE_ID_SEQ.currval from dual");
			while(rs->next())
			{
				file_id=rs->getString(1);
			}
			cout<<"Inert Table QT_FILE  OK! "<<endl;
			return file_id;
		}
		catch (SQLException e)
		{
			 cout<<e.what()<<endl;
			 return "";
		}
}

int QTDatabase::set_qt_alert_type()
{
		try
		{
			//st->setSQL("insert into QT_ALERT_TYPE(ALERT_TYPE_ID,ALERT_TYPE_LABEL,ALERT_TYPE_DESCRIPTION) values(ALERT_TYPE_ID_SEQ.nextval,:1,:2)");
			st->setSQL("insert into QT_ALERT_TYPE(ALERT_TYPE_ID,ALERT_TYPE_LABEL,ALERT_TYPE_DESCRIPTION) values(null,:1,:2)");
			st->setString(1,"13-01");
			st->setString(2,"steganography-communication, Picture Stego was Detected ");

			st->executeUpdate();

			int alert_type_id;
			ResultSet *rs=st->executeQuery("select ALERT_TYPE_ID_SEQ.currval from dual");
			while(rs->next())
			{
				alert_type_id=rs->getInt(1);
			}
			cout<<"Inert Table QT_ALERT_TYPE  OK! "<<endl;
			return alert_type_id;
		}
		catch (SQLException e)
		{
			 cout<<e.what()<<endl;
			 return 0;
		}
}

int QTDatabase::set_qt_service(const MailServerInfo &ml,string fileid)
{
		try
		{
			st->setSQL("insert into QT_SERVICE(SERVICE_ID,SERVICE_TYPE,FILE_NUM,FILE_ID,MAIL_FROM,MAIL_TO,MAIL_TIME,MAIL_SUBJECT) values(SERVICE_ID_SEQ.nextval,:1,:2,:3,:4,:5,to_date(:6,'yyyy-mm-dd hh24:mi:ss'),:7)");
			st->setInt(1,31);
			//st->setInt(2,ml.mail.AttachFileLength.size());    该行为文件个数，下面的统计的是告警文件的个数
			int file_num=0;
			for(int i=0;i<ml.mail.AttachFileNames.size();i++)
			{
				if	(ml.mail.AnalysisResults[i]==1)
				{
						file_num++;
				}
			}
			st->setInt(2,file_num);
			st->setString(3,fileid);
			st->setString(4,ml.mail.opt_mail_from);
			st->setString(5,ml.mail.opt_mail_to);

			char mail_time[32]; //转换时间戳格式
			st->setString(6,(GetFormatTime(ml.common.OPT_DATE,mail_time)==TRUE)?mail_time:NULL);
			st->setString(7,(ml.mail.opt_subject!=NULL)?ml.mail.opt_subject:NULL);
			st->executeUpdate();

			int service_id;
			ResultSet *rs=st->executeQuery("select SERVICE_ID_SEQ.currval from dual");
			while(rs->next())
			{
				service_id=rs->getInt(1);
			}
			cout<<"Inert Table QT_ALERT_SERVICE  OK! "<<endl;
			return service_id;
		}
		catch (SQLException e)
		{
			 cout<<e.what()<<endl;
			 return 0;
		}
}

void QTDatabase::set_qt_alert(const MailServerInfo &ml,int service_id,int result)
{
	try
	{

		st->setSQL("insert into QT_ALERT(ALERT_ID,EQU_ID,OCCUR_TIME,ALERT_LEVEL_ID,ALERT_TYPE_ID,ALERT_DESCRIPTION,CERTAINY_FACTOR,SIG_ID,OPERATE_STAGE,JUDGE_RESULT,"
				"FLOW_ID,SERVICE_TYPE,SERVICE_ID,SRC_IP,DST_IP,SRC_PORT,DST_PORT) values(ALERT_ID_SEQ.nextval,:1,(select sysdate from dual),:2,:3,:4,:5,:6,:7,:8,:9,:10,:11,:12,:13,:14,:15)");
		st->setInt(1,this->qt_equ_info_id);
		st->setInt(2,3);
		st->setInt(3,this->qt_alert_type_id);
		st->setString(4,"Picutre Stego Detected In SMTP !");
		st->setInt(5,result);
		st->setInt(6,0);
		st->setInt(7,1);
		st->setInt(8,1);

		st->setString(9,ml.common.OPT_FLOW_ID_LIST);
		st->setInt(10,31);
		st->setInt(11,service_id);
		st->setString(12,ml.common.OPT_SERVER_IP);
		st->setString(13,ml.common.OPT_CLIENT_IP);
		st->setInt(14,atoi(ml.common.OPT_SERVER_PORT));
		st->setInt(15,atoi(ml.common.OPT_CLIENT_PORT));

		st->executeUpdate();


		cout<<"Inert Table QT_ALERT  OK! "<<endl;
	}
	catch (SQLException e)
	{
		 cout<<e.what()<<endl;
	}

}

void QTDatabase::Commit()
{
	conn->commit();
}


