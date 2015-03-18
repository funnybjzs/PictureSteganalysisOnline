#include "StegoConfig.h"
#include <string.h>

//为什么要这个声明
//int  GetValue(const char *field,const char  *key,char *data);

StegoConfig::StegoConfig(const char *xmlfile)
{
	file<> fdoc(xmlfile);
	xml_document<> doc;
	doc.parse<0>(fdoc.data());
	this->root=doc.first_node()->first_node();		//获取根节点
}

int  StegoConfig::GetValue(const char *field,const char  *key,char *data){
	xml_node<>* node_choose=root->first_node(field);
	int hasvalue=0;
	if(!node_choose)
	{
		cout<<"specified field not found"<<endl;
		return hasvalue;
	}
	for(xml_node<>* node=node_choose->first_node();node!=NULL;node=node->next_sibling()){
		if(!strcmp(node->name(),key))
		{
			hasvalue=(strcpy(data,node->value())!=NULL)?1:0;
		}
	}
	if(!hasvalue)
	{
		cout<<"specified key not found"<<endl;
	}
	return hasvalue;
}

int  StegoConfig::GetValue(const char *field,const char  *key,const char  *attri,char *data){
	int hasvalue=0;
	xml_node<>* node_choose=root->first_node(field);
	if(!node_choose)
	{
		cout<<"specified field not found"<<endl;
		return hasvalue;
	}
	for(xml_node<>* node=node_choose->first_node();node!=NULL;node=node->next_sibling()){
		if(!strcmp(node->name(),key))
		{
			xml_attribute<>* node_by_attri=node->first_attribute();
			if(!node_by_attri)
			{
				cout<<"specified attri not found"<<endl;
				hasvalue=-1;
			}
			else
			{
				if(!strcmp(node_by_attri->value(),attri))
				{
					hasvalue=(strcpy(data,node->value())!=NULL)?1:0;
				}
			}
		}
	}
	if(!hasvalue)
	{
		cout<<"specified key not found"<<endl;
	}
	return hasvalue;
}

int StegoConfig:: GetDBInfo(char *data)
{
	char ip[16]={0},port[8]={0},sid[8]={0};
	if(GetValue("Database","Ip",ip)&&GetValue("Database","Port",port)&&GetValue("Database","Sid",sid))
	{
		strcat(data,ip);strcat(data,":");strcat(data,port);strcat(data,"/");strcat(data,sid);
		return 1;
	}
	else
	{
		return 0;
	}
}
