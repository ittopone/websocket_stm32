使用说明：

1. 当wifi模块转换到ap（即开启tcp server）模式时，
   需要初始化一个标志来表示此时app的websocket client未连接到服务器，
   如：bool g_is_app_connected = false
   
2. 当app的websocket client连接到服务器时，
   需要做如下处理：
   if(! g_is_app_connected)
   {
		if(! compute_accept_key())
			return error;
		if(! shake_hand())
			return error;
		g_is_app_connected = true;
		return true;
   }
   上面代码如果返回为true表示app的websocket client与服务器握手成功，
   已经成功连接上服务器，接下来就可以互相传输数据了。
   
3. 当服务器接收到app的websocket client发送过来的数据时，
   需要做如下处理：
   if(g_is_app_connected)
   {
		if(! analy_data())
			return error;
		//得到app传输过来的数据，数据存放在g_ws_read_buf中
   }
   
4. 当服务器需要发送数据给app的websocket client时，
   需要做如下处理：
   if(g_is_app_connected)
   {
		if(! response("here fill the data you want to send to app"))
			return error;
		//发送数据给app成功
   }

5. 当app异常或者服务器异常断开时，
   需要做如下处理：
   g_is_app_connected = false
   
注：
websocket文件夹下包含文件有：
base64.h intLib.h sha1.h ws_share.h websocket.h
使用时，仅需要包含头文件websocket.h即可，因为websocket.h已经包含了其他头文件。