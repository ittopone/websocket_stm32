#include "websocket.h"

int main()
{     
	int connected = 0;
	while(1)
	{
		memset(g_ws_read_buf, 0, WS_MIN_LEN_READ);  
		//n = read(connfd, buf, REQUEST_LEN_MAX);   
		printf("---------------------\n");  


		if(0 == connected)  
		{  
			compute_accept_key();
			shake_hand();  
			connected=1;  
			continue;  
		}  
		
		response("12345678abcdefj");
	}
}


