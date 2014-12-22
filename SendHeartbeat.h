#ifndef _SENDHEARTBEAT_H_
#define _SENDHEARTBEAT_H_
#include <stdint.h>
uint32_t GetHeartbeatInfo(uint8_t **buf);
bool SendHeartbeat(char *serverip,uint32_t serverport,uint32_t interval);
bool SendHeartbeatOnce(char *serverip,uint32_t serverport);



#endif
