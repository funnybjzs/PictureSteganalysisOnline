/*
 * ParseAux.cpp
 *
 *  Created on: 2014年10月14日
 *      Author: zhaoshuo
 */
#include "Parse.h"
#include "QTDatabase.h"
#include <string>

char PICTURE_TO_STORE[50]="/home/zhaoshuo/storepicture/";

extern sys_setting  *setting;
extern QTDatabase qt;

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

//分析结果写告警到数据库
string  WritingAlerts(const MailServerInfo & msi,int index,char *type,char path[])
{
		if (msi.mail.AnalysisResults[index] == 1) {  //有隐写则写入file表
			if (msi.common.OPT_TUPLE4_LIST != NULL) {
				cout<<"Alarm detected!"<<endl;
				return qt.set_qt_file(msi.mail.AttachFileNames[index],msi.mail.AttachFileLength[index],type,path);
			}
		}
		else if(msi.mail.AnalysisResults[index]==0) { 	//无隐写
			cout<<"OK dectected !"<<endl;
			return "";
		}
		else
		{
			cout<<"Other Types !..."<<endl;						//其它处理返回值，包括出错、其它文件类型等
			return "";
		}
}

void HttpParse(char *data,int opt_num)
{
			HttpServerInfo http;
			InitHttpServerInfo(&http);
			GetHttpOption(data, opt_num, &http);
			if(http.common.OPT_FLOW_ID_LIST!=NULL)
			{
				cout<<http.common.OPT_FLOW_ID_LIST<<endl;
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
			printf("Flow Data Id: %s\n", msi.common.OPT_FLOW_ID_LIST);
	   }
       //邮件基本信息是否为空
	   if (msi.mail.opt_mail_from!=NULL&&msi.mail.opt_mail_to!=NULL) {
			printf("Mail From: %s\n", msi.mail.opt_mail_from);
			printf("Mail To: %s\n", msi.mail.opt_mail_to);
			//附件不为空

			string qt_file_ids;   //文件id集合

		   if(msi.mail.AttachFileNames.size()!=0 && msi.mail.AttachFileContent.size()!=0)
		   {	//遍历附件
				for(int i=0;i<msi.mail.AttachFileNames.size();i++)
				{	//判断附件文件类型
					char *file_type = str_reverse(msi.mail.AttachFileNames[i], ".");
					if(file_type!=NULL)
					{
						cout<<"Attached File Name : "<<msi.mail.AttachFileNames[i]<<endl;
	                    cout<<"Attached File Type : "<<file_type<<endl;
	                    cout<<"Attached File Size : "<<msi.mail.AttachFileLength[i]<<endl;

						if(strstr(FILTER_TYPE,file_type)!=NULL)
						{
							//如果是.jpg、png则存储附件
							StoreAttachFiles(msi.mail.AttachFileNames[i],msi.mail.AttachFileLength[i],msi.mail.AttachFileContent[i]);
							cout<<"Start Picture Analysis..."<<endl;
							//int result=steganalysis(msi.mail.AttachFileContent[i],msi.mail.AttachFileLength[i], msi.mail.AttachFileNames[i],*setting);
							//用于暂时性规避,PNG图像只能写到硬盘,而非内存数据
							char filepath[100];
							strcpy(filepath,PICTURE_TO_STORE);
							strcat(filepath,msi.mail.AttachFileNames[i]);
							int result=steganalysis(msi.mail.AttachFileContent[i],msi.mail.AttachFileLength[i],filepath ,*setting);

							cout<<"the result is : "<<result<<endl;
							msi.mail.AnalysisResults.push_back(result);
						}
						else//其它文件类型
						{
							msi.mail.AnalysisResults.push_back(-100);
						}

						string  id=WritingAlerts(msi,i,file_type,PICTURE_TO_STORE);
						if(id!="")
						{
							qt_file_ids+=id;
							qt_file_ids+=",";
						}
					}
				}

				if(qt_file_ids.size()!=0)
				{
					int qt_service_id =qt.set_qt_service(msi,qt_file_ids);

					for(int i=0;i<msi.mail.AttachFileNames.size();i++)
					{
						if (msi.mail.AnalysisResults[i] == 1)
						{
							qt.set_qt_alert(msi,qt_service_id,msi.mail.AnalysisResults[i]);
						}
					}
				}

				qt.Commit();

		   }
		   else
		   {
			   cout<<"No Attached File...! "<<endl;
		   }
	   } //判断是否有邮件基本信息结束

	FreeMailServerInfo(&msi);

}

