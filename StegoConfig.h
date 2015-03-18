#include <iostream>
#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"

using namespace rapidxml;
using namespace std;

class StegoConfig
{
	public :
		StegoConfig(const char *xmlfile);
		virtual ~StegoConfig(){};
		int  GetValue(const char *field,const char  *key,char *data);
		int  GetValue(const char *field,const char  *key,const char  *attri, char *data);
		int  GetDBInfo(char *data);

	private:
		xml_node<>* root;
};
