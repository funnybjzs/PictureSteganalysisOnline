/*
 * system.cpp
 *
 *  Created on: 2014年8月16日
 *      Author: zhaoshuo
 */

#include "systemall.h"

//模板信息
 sys_setting  *setting;

 //工程部
 QTDatabase qt("192.168.1.60","1521","meta","qt","qt");

 //QTDatabase qt("192.168.119.128","1521","orcl","scott","tiger");

void InitSystemEnv()
{
	//载入隐写模版
	string jpeg_model("/home/zhaoshuo/lab/platform/model/jpeg_model/");
	string spatial_model("/home/zhaoshuo/lab/platform/model/spatial_model/");
	setting =new sys_setting(jpeg_model, spatial_model, "");
	cout<<"------分类器模板载入OK ! ------"<<endl;

	//链接数据库，准备相关资源
	if(qt.Init())
	{
		 cout<<"----------系统环境准备OK !-----------"<<endl;
	}
}

//释放数据库环境
void FreeSystemEnv(){
	if(qt.Release())	//释放数据库资源
	{
		cout<<"----------System Environment Free Finish !-----------"<<endl;
	}
}




