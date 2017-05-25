#ifndef __INTLIB_H__
#define __INTLIB_H__

#ifdef __cplusplus
extern "C"{
#endif
	
#include "ws_share.h"

int tolower(int c);

int htoi(const char s[],int start,int len);
	
	
int tolower(int c)   
{   
	if (c >= 'A' && c <= 'Z')   
	{   
		return c + 'a' - 'A';   
	}   
	else   
	{   
		return c;   
	}   
}   

int htoi(const char s[],int start,int len)   
{   
	int i,j;   
	int n = 0;
	if (s[0] == '0' && (s[1]=='x' || s[1]=='X'))
	{   
		i = 2;   
	}   
	else   
	{   
		i = 0;   
	}   
	i+=start;  
	j=0;  
	for (; (s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'f') || (s[i] >='A' && s[i] <= 'F');++i)   
	{     
		if(j>=len)  
		{  
			break;  
		}  
		if (tolower(s[i]) > '9')   
		{   
			n = 16 * n + (10 + tolower(s[i]) - 'a');   
		}   
		else   
		{   
			n = 16 * n + (tolower(s[i]) - '0');   
		}   
		j++;  
	}   
	return n;   
}   	
	
#ifdef __cplusplus	
}
#endif

#endif

