#ifndef __BASE64_H__
#define __BASE64_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "ws_share.h"
	
const char base[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";  
	
void base64_encode(void);   
	  
void base64_encode(void)   
{    
	int tmp = 0, i = 0;	
	int prepare = 0;     
	int temp = strlen(g_ws_read_buf) % 3;     
	char * f = NULL;   	   
	char changed[4];   

	memset(g_ws_write_buf, 0, WS_MAX_LEN_WRITE);   
	f = g_ws_write_buf;
	while (tmp < strlen(g_ws_read_buf))   
	{   
		temp = 0;   
		prepare = 0;   
		memset(changed, '\0', 4);   
		while (temp < 3)   
		{      
			if (tmp >= strlen(g_ws_read_buf))   
			{   
				break;   
			}   
			prepare = ((prepare << 8) | (g_ws_read_buf[tmp] & 0xFF));   
			tmp++;   
			temp++;   
		}   
		prepare = (prepare << ((3 - temp) * 8));      
		for (i = 0; i < 4 ;i++ )   
		{   
			if (temp < i)   
			{   
				changed[i] = 0x40;   
			}   
			else   
			{   
				changed[i] = (prepare >> ((3 - i) * 6)) & 0x3F;   
			}
			*f = base[changed[i]];     
			f++;   
		}   
	}   
	*f = '\0';
}   

#ifdef __cplusplus	
}
#endif

#endif

