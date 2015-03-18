/*
 * system.cpp
 *
 *  Created on: 2014年8月16日
 *      Author: zhaoshuo
 */

#include "systemall.h"

//模板信息
 sys_setting  *setting;

 QTDatabase qt("./QTStego.xml");


void InitSystemEnv(char  *xmlfile)
{
	StegoConfig scfg(xmlfile);
	//载入隐写模版
	char jpgml[128]={0},pngml[128]={0};
	if(scfg.GetValue("Classifier","ModelPath","jpg",jpgml)&&scfg.GetValue("Classifier","ModelPath","png",pngml))
	{
		string jpeg_model(jpgml);
		string spatial_model(pngml);
		setting =new sys_setting(jpeg_model, spatial_model, "");
		cout<<"------分类器模板载入OK ! ------"<<endl;
	}
	else
	{
		cout<<"------分类器模板信息出错，请检查配置文件内容是否正确-------"<<endl;
	}

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




