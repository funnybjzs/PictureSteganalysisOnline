#ifndef _CONFIG_H_
#define _CONFIG_H_


typedef unsigned int    UINT;
typedef int             INT;
typedef const char*     LPCTSTR;
typedef char*           LPCSTR;
typedef unsigned int   DWORD;

UINT GetPrivateProfileInt(
	        LPCTSTR lpAppName,    //   address   of   section   name   
            LPCTSTR lpKeyName,    //   address   of   key   name   
            INT nDefault,         //   return   value   if   key   name   is   not   found   
            LPCTSTR lpFileName    //   address   of   initialization   filename   
);

DWORD GetPrivateProfileString(
			LPCTSTR lpAppName,          //   points   to   section   name   
            LPCTSTR lpKeyName,          //   points   to   key   name   
            LPCTSTR lpDefault,         //   points   to   default   string   
            LPCSTR lpReturnedString,   //   points   to   destination   buffer   
            DWORD nSize,               //   size   of   destination   buffer   
            LPCTSTR lpFileName        //   points   to   initialization   filename   
);

UINT GetPrivateProfileInt(LPCTSTR lpAppName,LPCTSTR lpKeyName,INT nDefault,LPCTSTR lpFileName);
DWORD GetPrivateProfileString(LPCTSTR lpAppName,LPCTSTR lpKeyName,LPCTSTR lpDefault,LPCSTR lpReturnedString,DWORD nSize,LPCTSTR lpFileName);


#endif
