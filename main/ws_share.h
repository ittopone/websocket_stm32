#ifndef __WS_SHARE_H__
#define __WS_SHARE_H__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>  
#include <string.h>  

//调试开关
#define DEBUG_WEBSOCKET

//是否允许在linux上调试
#define EN_TEST_ON_LINUX 0

#if EN_TEST_ON_LINUX
int g_connfd = 0;
#endif
	
#define WS_MIN_LEN_READ 544

#define WS_MAX_LEN_WRITE 256

char g_ws_read_buf[WS_MIN_LEN_READ] = {0};

char g_ws_write_buf[WS_MAX_LEN_WRITE] = {0};

#ifdef __cplusplus	
}
#endif

#endif

