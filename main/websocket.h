#ifndef __WEBSOCKET_H__
#define __WEBSOCKET_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "base64.h"  
#include "sha1.h"  
#include "intLib.h"  

char shake_hand(void);  
	
char fetch_sec_key(void); 
	
char compute_accept_key(void); 
	
char analy_data(void); 

void pack_data(const char * message, unsigned long * len);  

char response(const char * message);
	

//if error, return 0, else return 1
char fetch_sec_key(void)  
{   
	const char * flag = "Sec-WebSocket-Key: "; 
	char * keyBegin = NULL; 
	int i = 0;	
 
	memset(g_ws_write_buf, 0, WS_MAX_LEN_WRITE);    

	keyBegin = strstr(g_ws_read_buf, flag);  
	if(! keyBegin)  
	{  
		#ifdef DEBUG_WEBSOCKET
		printf("%s-%d:get websocket client key error.\n", __func__, __LINE__);
		#endif
		return 0;  
	}
	
	keyBegin += strlen(flag);  

	for(i = 0; i < strlen(g_ws_read_buf); i++)  
	{  
		if((keyBegin[i] == 0x0A) || (keyBegin[i] == 0x0D))  
		{  
			break;  
		}  
		g_ws_write_buf[i] = keyBegin[i];  
	}  

	return 1;  
}  

//if error, return 0, else return 1
char compute_accept_key(void)  
{  
	const char * GUID="258EAFA5-E914-47DA-95CA-C5AB0DC85B11";  	  
	int i = 0, n = 0;  
 
	#ifdef DEBUG_WEBSOCKET
	printf("\nclient request header:\n%s\n", g_ws_read_buf);
	#endif
	
	//»ñÈ¡websocket clientµÄwebsocket key 
	if(! fetch_sec_key())
		return 0;

	strcat(g_ws_write_buf, GUID);  

	if(! sha1_hash())
		return 0;
 
	n = strlen(g_ws_write_buf); 
 
	memset(g_ws_read_buf, 0, WS_MIN_LEN_READ);  

	for(i = 0; i < n; i += 2)  
	{        
		g_ws_read_buf[i / 2] = htoi(g_ws_write_buf, i, 2);      
	}   

	base64_encode();   

	return 1;  
}  

//if error, return 0, else return 1
char shake_hand(void)  
{  
	memset(g_ws_read_buf, '\0', WS_MIN_LEN_READ);  

	sprintf(g_ws_read_buf, "HTTP/1.1 101 Switching Protocols\r\n");  
	sprintf(g_ws_read_buf, "%sUpgrade: websocket\r\n", g_ws_read_buf);  
	sprintf(g_ws_read_buf, "%sConnection: Upgrade\r\n", g_ws_read_buf);  
	sprintf(g_ws_read_buf, "%sSec-WebSocket-Accept: %s\r\n\r\n", g_ws_read_buf, g_ws_write_buf);  

	#ifdef DEBUG_WEBSOCKET
	printf("\nserver response header:\n%s\n", g_ws_read_buf);
	#endif
	
	#if EN_TEST_ON_LINUX
	if(write(g_connfd, g_ws_read_buf, strlen(g_ws_read_buf)) < 0)
		return 1;
	#endif
	
	return 0;
}

char analy_data(void)  
{   
	char fin = 0;
	char maskFlag = 0;
	char masks[4] = {0};  
	char temp[8];  
	unsigned long n = 0, payloadLen = 0;  
	int i = 0;   


	if (strlen(g_ws_read_buf) < 2)   
	{  
		#ifdef DEBUG_WEBSOCKET
		printf("%s-%d:data len error.\n", __func__, __LINE__);
		#endif
		return 0;  
	}  

	fin = (g_ws_read_buf[0] & 0x80) == 0x80; 
	if (!fin)  
	{  
		#ifdef DEBUG_WEBSOCKET
		printf("%s-%d:fin error.\n", __func__, __LINE__);
		#endif
		return 0;
	}  

	maskFlag = (g_ws_read_buf[1] & 0x80) == 0x80;   
	if (!maskFlag)  
	{  
		#ifdef DEBUG_WEBSOCKET
		printf("%s-%d:mask flag error.\n", __func__, __LINE__);
		#endif
		return 0;
	}  

	payloadLen = g_ws_read_buf[1] & 0x7F;  
	if (payloadLen == 126)  
	{        
		memcpy(masks, g_ws_read_buf + 4, 4);        
		payloadLen =(g_ws_read_buf[2] & 0xFF) << 8 | (g_ws_read_buf[3] & 0xFF);      
		memset(g_ws_write_buf, 0, WS_MAX_LEN_WRITE);  
		memcpy(g_ws_write_buf, g_ws_read_buf + 8, payloadLen);  
	}  
	else if (payloadLen == 127)  
	{  
		memcpy(masks, g_ws_read_buf + 10, 4);    
		for ( i = 0; i < 8; i++)  
		{  
			temp[i] = g_ws_read_buf[9 - i];  
		}   

		memcpy(&n,temp,8);      
		memset(g_ws_write_buf, 0, WS_MAX_LEN_WRITE);   
		memcpy(g_ws_write_buf, g_ws_read_buf + 14, n);//toggle error(core dumped) if data is too long.  
		payloadLen = n;      
	}  
	else  
	{     
		memcpy(masks, g_ws_read_buf + 2, 4);       
		memset(g_ws_write_buf, 0, WS_MAX_LEN_WRITE);  
		memcpy(g_ws_write_buf, g_ws_read_buf + 6, payloadLen);   
	}  

	for (i = 0; i < payloadLen; i++)  
	{  
		g_ws_write_buf[i] = (char)(g_ws_write_buf[i] ^ masks[i % 4]);  
	}  
	
	#ifdef DEBUG_WEBSOCKET
	printf("get data:%s, length:%lu\n", g_ws_write_buf, payloadLen);
	#endif	
	
	return 1; 
}  

void pack_data(const char * message, unsigned long * len)  
{   
	unsigned long n = 0;  

	n = strlen(message);  
	if (n < 126)  
	{   
		memset(g_ws_write_buf, 0, WS_MAX_LEN_WRITE);      
		g_ws_write_buf[0] = 0x81;  
		g_ws_write_buf[1] = n;  
		memcpy(g_ws_write_buf + 2, message, n);  
		*len = n + 2;  
	}  
	else if (n < 0xFFFF)  
	{    
		memset(g_ws_write_buf, 0, WS_MAX_LEN_WRITE);
		g_ws_write_buf[0] = 0x81;  
		g_ws_write_buf[1] = 126;  
		g_ws_write_buf[2] = ((n >> 8) & 0xFF);  
		g_ws_write_buf[3] = (n & 0xFF);  
		memcpy(g_ws_write_buf + 4, message, n);      
		*len = n + 4;  
	}  
	else  
	{  

		*len = 0;
	} 
}  

char response(const char * message)  
{  
	#if EN_TEST_ON_LINUX
	unsigned long n = 0; 
	
	if(! g_connfd)  
	{  
		return 0;  
	}  
	if(! message)  
	{  
		return 0;  
	}
	
	pack_data(message, &n);   

	if(n <= 0)  
	{   
		#ifdef DEBUG_WEBSOCKET
		printf("%s-%d:message is empty.\n", __func__, __LINE__);
		#endif
		return;  
	}   
	//send data to websocket client
	if(write(g_connfd, g_ws_write_buf, n) < 0)
		return 0;
	#endif
	
	return 1;
}

#ifdef __cplusplus	
}
#endif

#endif

