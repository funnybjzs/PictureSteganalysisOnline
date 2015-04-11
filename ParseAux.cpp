/*
 * ParseAux.cpp
 *
 *  Created on: 2014年10月14日
 *      Author: zhaoshuo
 */
#include "Parse.h"
#include "QTDatabase.h"
#include <string>

char PICTURE_TO_STORE[64];
char FILTER_TYPE[32];
sys_setting  *setting;
QTDatabase qt;

//通用解析。处理文件名，从右边过滤文件类型
char* str_reverse(char *s1,char const *s2)
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

//存储附件
void StoreAttachFiles(char *file_name,int file_length,char *file_content)
{
	char write_to_picture[100];
	strcpy(write_to_picture, PICTURE_TO_STORE);
	strcat(write_to_picture, file_name);
	//附件写到硬盘
	FILE *pFile;
	pFile = fopen(write_to_picture, "wb");
	if (pFile != NULL) {
		fwrite(file_content, sizeof(char), file_length, pFile);
	}
	fclose(pFile);
}

void HttpParse(char *data,int opt_num)
{
			HttpServerInfo http;
			InitHttpServerInfo(&http);
			GetHttpOption(data, opt_num, &http);
			if(http.common.OPT_FLOW_ID_LIST!=NULL)
			{
				cout<<"流数据ID : "<<http.common.OPT_FLOW_ID_LIST<<endl;
			}

			FreeHttpServerInfo(&http);
}

void SmtpParse(char *data, int opt_num)
{
	   MailServerInfo msi;
	   InitMailServerInfo(&msi);
	   GetMailOption(data,opt_num,&msi);

       //FLOW_ID是否为空
	   if(msi.common.OPT_FLOW_ID_LIST!=NULL) //L31
	   {
			cout<<"流数据ID : "<<msi.common.OPT_FLOW_ID_LIST<<endl;
	   }
       //邮件基本信息是否为空
	   if (msi.mail.opt_mail_from!=NULL&&msi.mail.opt_mail_to!=NULL) {
			printf("发件方: %s\n", msi.mail.opt_mail_from);
			printf("接收方: %s\n", msi.mail.opt_mail_to);
			//附件不为空
		   if(msi.mail.AttachFileNames.size()!=0 && msi.mail.AttachFileContent.size()!=0)
		   {	//遍历附件
			   ProcessSmtpAttachments(msi);
		   }
		   else
		   {
			   cout<<"No Attached File...! "<<endl;
		   }
	   } //判断邮件是否基本信息结束

	FreeMailServerInfo(&msi);
}


//检测附件
void DetectAttachments(MailServerInfo & msi)
{
	//遍历邮件附件
	for(int i=0;i<msi.mail.AttachFileNames.size();i++)
	{	//判断附件文件类型
		char *file_type = str_reverse(msi.mail.AttachFileNames[i], ".");
		if(file_type!=NULL)
		{
			cout<<"附带文件名称 : "<<msi.mail.AttachFileNames[i]<<endl;
            cout<<"附带文件类型 : "<<file_type<<endl;
            cout<<"附带文件大小 : "<<msi.mail.AttachFileLength[i]<<endl;

			if(strstr(FILTER_TYPE,file_type)!=NULL)							//如果是.jpg、png则存储附件
			{ //存储附件
				StoreAttachFiles(msi.mail.AttachFileNames[i],msi.mail.AttachFileLength[i],msi.mail.AttachFileContent[i]);
				cout<<"开始进行隐写分析..."<<endl;
				//int result=steganalysis(msi.mail.AttachFileContent[i],msi.mail.AttachFileLength[i], msi.mail.AttachFileNames[i],*setting);
				//用于暂时性规避,PNG图像只能写到硬盘,而非内存数据
				char filepath[100];
				strcpy(filepath,PICTURE_TO_STORE);
				strcat(filepath,msi.mail.AttachFileNames[i]);

				char *pic=msi.mail.AttachFileContent[i];
				int pic_len=msi.mail.AttachFileLength[i];

				if(strcmp(file_type,".jpg")==0)
				{
					if(  !(   (pic[0] == (char)0xff)   && (pic[1] == (char)0xd8)   &&  (pic[pic_len-2] == (char)0xff)    && (pic[pic_len-1] == (char)0xd9)  ) ){
						cout<<"Corrupt Imaget!"<<endl;
						msi.mail.AnalysisResults.push_back(-100);
						msi.mail.LevelResults.push_back(-1);
					}
					else
					{
				        int result= 0;
				        double deciVal = 0.0;

				        ////这个是分析过程
				        int errorCode = steganalysis(msi.mail.AttachFileContent[i], msi.mail.AttachFileLength[i],filepath, *setting, result, deciVal);
				        cout << "decision value: " << deciVal << endl;
						cout<<"初步分析结果: "<<result<<endl;
						msi.mail.AnalysisResults.push_back(result);
						msi.mail.LevelResults.push_back(deciVal);
					}
				}
				else if(strcmp(file_type,".png")==0)
				{
			        int result= 0;
			        double deciVal = 0.0;

			        ////这个是分析过程
			        int errorCode = steganalysis(msi.mail.AttachFileContent[i], msi.mail.AttachFileLength[i],filepath, *setting, result, deciVal);
			        cout << "decision value: " << deciVal << endl;
					cout<<"初步分析结果: "<<result<<endl;
					msi.mail.AnalysisResults.push_back(result);
					msi.mail.LevelResults.push_back(deciVal);
				}
				else{
					;//其它格式
				}
			}
			else                                                                                                       //其它文件类型
			{
				cout<<"其它文件类型附件..."<<endl;
				msi.mail.AnalysisResults.push_back(-100);
				msi.mail.LevelResults.push_back(-1);
			}
		}
		else{
			cout<<"无法解析该文件名称..."<<endl;
			msi.mail.AnalysisResults.push_back(-100);            //无法解析的文件名直接丢弃
			msi.mail.LevelResults.push_back(-1);
		}
	}
}

//分析结果写告警到数据库

bool hasAlertFiles(const MailServerInfo & msi)
{
	bool has=false;
	for(int index=0;index<msi.mail.AttachFileNames.size();index++)
	{
		if (msi.mail.AnalysisResults[index] == 1)
		{
			has=true;
			break;
		}
	}
	return has;
}

void  WritingAlerts(const MailServerInfo & msi)
{
	if(hasAlertFiles(msi))
	{
		string qt_file_ids;   //告警文件id集合
		//遍历邮件附件
		for(int index=0;index<msi.mail.AttachFileNames.size();index++)
		{
			if (msi.mail.AnalysisResults[index] == 1) {  //有隐写则写入file表
				if (msi.common.OPT_TUPLE4_LIST != NULL) {
					cout<<"检测到可疑文件!"<<endl;
					qt_file_ids += qt.set_qt_file(msi,index);
					qt_file_ids+=",";
					cout<<"set QT_FILE OK!"<<endl;
				}
			}
			else if(msi.mail.AnalysisResults[index]==-1) { 	//无隐写
				cout<<"检测正常!"<<endl<<endl;
			}
			else
			{
				cout<<"数据错误或未知错误..."<<endl;				//其它处理返回值，包括数据出错、其它文件类型等
			}
		}

		qt_file_ids.erase(qt_file_ids.end()-1);
		int qt_service_id=qt.set_qt_service(msi,qt_file_ids);
		cout<<"Inert Table QT_ALERT_SERVICE  OK! "<<endl;
		qt.set_qt_alert(msi, qt_service_id);
		cout<<"Inert Table QT_ALERT  OK! "<<endl;

		for (int i = 0; i < msi.mail.AttachFileNames.size(); i++) {
			if (msi.mail.AnalysisResults[i] != 1) {
				char file_to_delete[100];
				strcpy(file_to_delete, PICTURE_TO_STORE);
				strcat(file_to_delete, msi.mail.AttachFileNames[i]);
			//	remove(file_to_delete);
		    }
		}

		qt.Commit();
	}
}

void ProcessSmtpAttachments(MailServerInfo & msi)
{
	DetectAttachments(msi);
	WritingAlerts(msi);
}
