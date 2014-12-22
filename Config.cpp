#include "Config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//以下是两个读取config.ini文件的函数
UINT GetPrivateProfileInt(
			LPCTSTR lpAppName, //   address   of   section   name   
            LPCTSTR lpKeyName,  //   address   of   key   name   
            INT nDefault,  //   return   value   if   key   name   is   not   found   
            LPCTSTR lpFileName  //   address   of   initialization   filename   
)
{

    UINT ret = nDefault;
    FILE *file = NULL;

    file = fopen(lpFileName, "r");
    if (file == NULL)
    {
        return ret;
    }
    char srcbuf[256];
    char desbuf[256];
    int size = 256;
    char *_srcbuf = srcbuf;
    char *_desbuf = desbuf;
    char target = 'S';
    //'S'   find   section   name,'K'   find   key   name   
    while (fgets(_srcbuf, size, file) != NULL)
    {
        int flag = 1;
        if (target == 'S')
        {
            if (_srcbuf[0] != '[')
            {
                continue;
            }
            strcpy(_desbuf, lpAppName);
            int len = strlen(_desbuf);
            int i;
            for (i = 0; i < len; i++)
            {
                if (_desbuf[i] != _srcbuf[i + 1])
                {
                    flag = 0;
                    break;
                }
            }
            if (flag == 0)
            {
                continue;
            }
            if (_srcbuf[i + 1] != ']')
            {
                continue;
            }
            target = 'K';
        }
        else
        {
            if (_srcbuf[0] == '[')
            {
                break;
            }
            strcpy(_desbuf, lpKeyName);
            int len = strlen(_desbuf);
            int i;
            for (i = 0; i < len; i++)
            {
                if (_desbuf[i] != _srcbuf[i])
                {
                    flag = 0;
                    break;
                }
            }
            if (flag == 0)
            {
                continue;
            }

            int _flag = 0;
            while (_srcbuf[i] != '\0')
            {
                if (_srcbuf[i] == ' ')
                {
                    i++;
                    continue;
                }
                if (_srcbuf[i] == '=')
                {
                    _flag = 1;
                    i++;
                    continue;
                }
                if (_flag == 0)
                {
                    break;
                }
                char *lpresult = &_srcbuf[i];
                if (strcmp(lpresult,"0")==0)
                {
                    fclose(file);
                    file = NULL;
                    return 0;
                }
                UINT value = strtoul(lpresult, NULL, 0);
                if (value != 0)
                {
                    ret = value;
                }
                fclose(file);
                file = NULL;
                return ret;
            }
        } //end   of   if   
    } //   end   of   while   
    fclose(file);
    file = NULL;
    return ret;

} //end   of   GetPrivateProfileInt   


DWORD GetPrivateProfileString(LPCTSTR lpAppName, 
//   points   to   section   name   
LPCTSTR lpKeyName,  //   points   to   key   name   
LPCTSTR lpDefault,  //   points   to   default   string   
LPCSTR lpReturnedString,  //   points   to   destination   buffer   
DWORD nSize,  //   size   of   destination   buffer   
LPCTSTR lpFileName  //   points   to   initialization   filename   
)
{

    DWORD ret = 0;
    FILE *file = NULL;
	int i;

    ret = strlen(lpDefault);
    if (ret < nSize)
    {
        strcpy(lpReturnedString, lpDefault);
    }
    else
    {
        unsigned int i;
        for (i = 0; i < nSize - 1; i++)
        {
            lpReturnedString[i] = lpDefault[i];
        }
        lpReturnedString[i] = '\0';
        ret = nSize - 1;
    }

    file = fopen(lpFileName, "r");
    if (file == NULL)
    {
        return ret;
    }

    if (strcmp(lpAppName,"")==0)
    {
        char buf[256];
        int size = 256;
        char *_buf = buf;
        char *lpret = lpReturnedString;
        DWORD count = 0;

        while (fgets(_buf, size, file) != NULL && (count < nSize - 2))
        {
            if (_buf[0] != '[')
            {
                continue;
            }
            for (i = 1; count < nSize - 2 && _buf[i] != ']' && _buf[i] != '\r'
                 && _buf[i] != '\n'; i++)
            {
                *lpret = _buf[i];
                lpret++;
                count++;
            }
            *lpret = '\0';
            lpret++;
            count++;
        }
        *lpret = '\0';
        if (count > 0)
        {
            count--;
        }
        fclose(file);
        file = NULL;
        return ret = count;
    } //   end   of   lpAppName   ==   ""   

    if (strcmp(lpKeyName,"")==0)
    {
        char srcbuf[256];
        char desbuf[256];
        int size = 256;
        char *_srcbuf = srcbuf;
        char *_desbuf = desbuf;
        char target = 'S';
        //'S'   find   section   name,'K'   find   key   name   
        char *lpret = lpReturnedString;
        DWORD count = 0;
        while (fgets(_srcbuf, size, file) != NULL && (count < nSize - 2))
        {
            int flag = 1;
            if (target == 'S')
            {
                if (_srcbuf[0] != '[')
                {
                    continue;
                }
                strcpy(_desbuf, lpAppName);
                int len = strlen(_desbuf);
                int i;
                for (i = 0; i < len; i++)
                {
                    if (_desbuf[i] != _srcbuf[i + 1])
                    {
                        flag = 0;
                        break;
                    }
                }
                if (flag == 0)
                {
                    continue;
                }
                if (_srcbuf[i + 1] != ']')
                {
                    continue;
                }
                target = 'K';
            }
            else
            {
                if (_srcbuf[0] == '[')
                {
                    break;
                }
                for (i = 0; count < nSize - 2 && _srcbuf[i] != ' ' &&
                     _srcbuf[i] != '=' && _srcbuf[i] != '\r' && _srcbuf[i]
                     != '\n'; i++)
                {
                    *lpret = _srcbuf[i];
                    lpret++;
                    count++;

                }
                *lpret = '\0';
                lpret++;
                count++;
            } //   end   of   if   
        } //   end   of   while   
        *lpret = '\0';
        if (count > 0)
        {
            count--;
        }
        fclose(file);
        file = NULL;
        return ret = count;

    } //   end   of   lpKeyName   ==   ""   

    char srcbuf[256];
    char desbuf[256];
    int size = 256;
    char *_srcbuf = srcbuf;
    char *_desbuf = desbuf;
    char target = 'S';
    //'S'   find   section   name,'K'   find   key   name   
    while (fgets(_srcbuf, size, file) != NULL)
    {
        int flag = 1;
        if (target == 'S')
        {
            if (_srcbuf[0] != '[')
            {
                continue;
            }
            strcpy(_desbuf, lpAppName);
            int len = strlen(_desbuf);
            for (i = 0; i < len; i++)
            {
                if (_desbuf[i] != _srcbuf[i + 1])
                {
                    flag = 0;
                    break;
                }
            }
            if (flag == 0)
            {
                continue;
            }
            if (_srcbuf[i + 1] != ']')
            {
                continue;
            }
            target = 'K';
        }
        else
        {
            if (_srcbuf[0] == '[')
            {
                break;
            }
            strcpy(_desbuf, lpKeyName);
            int len = strlen(_desbuf);
            int i;
            for (i = 0; i < len; i++)
            {
                if (_desbuf[i] != _srcbuf[i])
                {
                    flag = 0;
                    break;
                }
            }
            if (flag == 0)
            {
                continue;
            }

            int _flag = 0;
            while (_srcbuf[i] != '\0')
            {
                if (_srcbuf[i] == ' ')
                {
                    i++;
                    continue;
                }
                if (_srcbuf[i] == '=')
                {
                    _flag = 1;
                    i++;
                    continue;
                }
                if (_flag == 0)
                {
                    break;
                }
                char *lpresult = &_srcbuf[i];
                char *lpret = lpReturnedString;
                int count = 0;
                while (*lpresult != '\r' &&  *lpresult != '\n' &&  *lpresult !=
                       '\0')
                {
                    *lpret =  *lpresult;
                    lpret++;
                    lpresult++;
                    count++;
                }
                *lpret = '\0';
                fclose(file);
                file = NULL;
                return ret = count;
            } //   end   of   while   
        } //   end   of   if   
    } //   end   of   while   
    fclose(file);
    file = NULL;
    return ret;


} //end   of   GetPrivateProfileString
/*
	GetPrivateProfileString("Console", "ip", "", THEIR_IP, 30, "./config.ini");
	THEIR_PORT=GetPrivateProfileInt("Console", "port", 0, "./config.ini"); 
	//GetPrivateProfileString("Myself", "ip", "", MY_IP, 30, "./config.ini");
	if (!get_local_ip(MY_IP))
		exit(-1);
	SpaceOfTime = GetPrivateProfileInt("Other", "spaceoftime", 0, "./config.ini");

	GetPrivateProfileString("Stat","ip","",STAT_IP,30,"./config.ini");
	STAT_PORT=GetPrivateProfileInt("Stat", "port", 0, "./config.ini"); 
	stattime = GetPrivateProfileInt("Stat", "stattime", 0, "./config.ini"); 
	statlength = GetPrivateProfileInt("Stat", "statlength", 0, "./config.ini"); 
	*/
