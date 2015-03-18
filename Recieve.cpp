/*
 * Recieve.c
 *
 *  Created on: 2014年7月9日
 *      Author: zhaoshuo
 */
#include "Recieve.h"

char *left_data =NULL;
int left_length = 0;
bool isHeadStart = true;
AppData_Head head;

void RecieveData(char *data, int len, int sckfd) {
	//组装数据
	if (left_data != NULL) {
		left_data = (char *) realloc(left_data, left_length + len + 1);
		memset(left_data + left_length + len, 0, 1);
		memcpy(left_data + left_length, data, len);

	} else {
		left_data = (char *) malloc(len + 1);
		memset(left_data, 0, len + 1);
		memcpy(left_data, data, len);
	}
	left_length += len;

	if (isHeadStart) {
		if (strstr(left_data, "\r\n\r\n")) //判断数据是否完整
				{    //调试打印
//			printf("head...\n");
//			int i;
//			for (i = 0; i < 32; ++i) {
//				printf("%c", left_data[i]);
//			}
//			printf("\n......\n");
			left_length = RecieveLoop(left_data, left_length, sckfd);
		} else {
			;
		}
	} else {
		if (atoi(head.option[CONT_LENGTH]) <= left_length) {
			//调试打印
//			printf("[recv]:body length: %d...\n", atoi(head.option[CONT_LENGTH]));
//			int i;
//			for (i = 0; i <32; ++i) {
//				printf("%c", left_data[i]);
//			}
//			printf("\n......\n");
			ParseBody(left_data, atoi(head.option[OPT_NUM]));
			left_length -= atoi(head.option[CONT_LENGTH]);

			if (left_length == 0) {
				FreeAppHead(&head);
				free(left_data);
				left_data = NULL;
				isHeadStart = true;
			} else {
				char *left = left_data;
				left = left + atoi(head.option[CONT_LENGTH]);
				FreeAppHead(&head);
				left_length = RecieveLoop(left, left_length, sckfd);
			}
		} else {
			;
		}
	}
}

int RecieveLoop(char *data, int left_len, int sckfd) {
	while (left_len > 0) {
		if (strstr(data, "\r\n\r\n")) //长度够数据头
		{
//						printf("head...\n");
//						int i;
//						for (i = 0; i < 32; ++i) {
//							printf("%c", left_data[i]);
//						}
			int head_len = ParseHead(data, &head);
//			printf("\nhead length : %d......\n",head_len);
//			int m;
//			for (m = 0; m < head_len; ++m) {
//				printf("%c", left_data[m]);
//			}
//			printf("\n....\n");
			//发送确认消息
			SendConfirmMsg(&head, sckfd);
			left_len = left_len-head_len;
			int body_len = atoi(head.option[CONT_LENGTH]);
			if(body_len==0)	            //数据体为空
			{
				FreeAppHead(&head);
				if(left_len==0)	          //恰好够数据头解析完
				{
					isHeadStart = true;
					free(left_data);
					left_data = NULL;
				}
				else
				{
					data = data + head_len;
					continue;
				}
			}
			else   							//数据体不为空
			{
				data = data + head_len;
				//剩余长度够数据体
				if (left_len >= body_len) {
//					printf("[Loop]:body length: %d...\n", atoi(head.option[CONT_LENGTH]));
//								printf("head...\n");
//								int j;
//								for (j = 0; j < 32; ++j) {
//									printf("%c", left_data[j]);
//								}
//								printf("\n......\n");
					ParseBody(data, atoi(head.option[OPT_NUM]));
					left_len = left_len - body_len;

					//剩余长度刚好够数据体
					if (left_len == 0) {
						isHeadStart = true;
						free(left_data);
						left_data = NULL;
						FreeAppHead(&head);
//						break;
					} else //继续解析下一个
					{
						data = data + body_len;
						FreeAppHead(&head);
					}
				} else //剩余长度不够数据体
				{
					isHeadStart = false;
					if (left_len == 0) //刚好够数据头
							{
						free(left_data);
						left_data = NULL;
						break;
					} else {
						char *tmp = (char *)malloc(left_len + 1);
						memset(tmp, 0, left_len + 1);
						memcpy(tmp, data, left_len);
						free(left_data);
						left_data = NULL;
						left_data = tmp;
						tmp = NULL;
						break;
					}
				}
			}
		} else      //不够数据头
		{
			char *tmp = (char *)malloc(left_len + 1);
			memset(tmp, 0, left_len + 1);
			memcpy(tmp, data, left_len);
			free(left_data);
			left_data = NULL;
			left_data = tmp;
			tmp = NULL;
			isHeadStart = true;
			break;
		}
	}

	return left_len;
}

void SendConfirmMsg(AppData_Head *h, int sckfd) {
	char msg[200] = { 0 };
	strcat(msg, h->option[Start_Version]);
	strcat(msg, " ");
	strcat(msg, RESOPONSE_MSG[0][0]);
	strcat(msg, " ");
	strcat(msg, RESOPONSE_MSG[1][0]);
	strcat(msg, "\r\n");

	strcat(msg, "X-Data-Seq:");
	strcat(msg, h->option[Data_Seq]);
	strcat(msg, "\r\n");

	strcat(msg, "X-Capture-Ip:");
	strcat(msg, h->option[CAP_IP]);
	strcat(msg, "\r\n\r\n");

//	printf("response:\n");
//	printf("%s", msg);

	send(sckfd, msg, strlen(msg), 0);
}

void ClearAllTmpData()
{
	if(left_data!=NULL)
	{
		free(left_data);
		left_data=NULL;
	}
	left_length=0;
	isHeadStart=true;
	FreeAppHead(&head);
}
