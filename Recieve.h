/*
 * Recieve.h
 *
 *  Created on: 2014年7月9日
 *      Author: zhaoshuo
 */

#ifndef RECIEVE_H_
#define RECIEVE_H_

#include "Parse.h"
#include <sys/socket.h>

static char *left_data =NULL;
static int left_length = 0;
static int isHeadStart = true;
static AppData_Head head;

int RecieveLoop(char *data,int left,int sck_fd);
void RecieveData(char *data,int len,int sck_fd);
void SendConfirmMsg(AppData_Head *h,int sck_fd);

#endif /* RECIEVE_H_ */
