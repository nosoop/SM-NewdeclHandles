#if defined _socket_newdecls_included
	#endinput
#endif

#define _socket_newdecls_included
#pragma newdecls required

/**
 * Prevent a "function prototypes do not match" error from old-style include's funcenums by
 * defining them using the new "Socket" name before including the old include.  The new typedefs
 * will take priority as a result.  That also means that <socket> must not have been included 
 * prior to the inclusion of this file.
 * 
 * This allows users to use "Socket" instead of "Handle" in the function definition.
 */
#if defined _socket_included
	#error "Cannot #include <socket> before this include"
#endif

typedef SocketConnectCB = function void (Socket socket, any arg);
typedef SocketIncomingCB = function void (Socket socket, Socket newSocket,
		char[] remoteIP, int remotePort, any arg);
typedef SocketReceiveCB = function void (Socket socket, const char[] receiveData,
		int dataSize, any arg);
typedef SocketSendqueueEmptyCB = function void (Socket socket, any arg);
typedef SocketDisconnectCB = function void (Socket socket, any arg);
typedef SocketErrorCB = function void (Socket socket, int errorType, int errorNum, any arg);

#pragma newdecls optional
#include <socket>

#pragma newdecls required

methodmap Socket < Handle {
	public Socket(SocketType protocol = SOCKET_TCP, SocketErrorCB onError) {
		return view_as<Socket>(SocketCreate(protocol, onError));
	}
	
	public bool Bind(const char[] hostname, int port) {
		return SocketBind(this, hostname, port);
	}
	
	public void Connect(SocketConnectCB onConnect, SocketReceiveCB onReceive,
			SocketDisconnectCB onDisconnect, const char[] hostname, int port) {
		SocketConnect(this, onConnect, onReceive, onDisconnect, hostname, port);
	}
	
	public bool Disconnect() {
		return SocketDisconnect(this);
	}
	
	public bool Listen(SocketIncomingCB onIncoming) {
		return SocketListen(this, onIncoming);
	}
	
	public void Send(const char[] data, int size = -1) {
		SocketSend(this, data, size);
	}
	
	public void SendTo(const char[] data, int size = -1, const char[] hostname, int port) {
		SocketSendTo(this, data, size, hostname, port);
	}
	
	public bool SetOption(SocketOption option, any value) {
		return SocketSetOption(this, option, value) == 1;
	}
	
	property SocketReceiveCB ReceiveCallback {
		public set(SocketReceiveCB onReceive) {
			SocketSetReceiveCallback(this, onReceive);
		}
	}
	
	property SocketSendqueueEmptyCB SendqueueEmptyCallback {
		public set(SocketSendqueueEmptyCB onEmptySendqueue) {
			SocketSetSendqueueEmptyCallback(this, onEmptySendqueue);
		}
	}
	
	property SocketDisconnectCB DisconnectCallback {
		public set(SocketDisconnectCB onDisconnect) {
			SocketSetDisconnectCallback(this, onDisconnect);
		}
	}
	
	property SocketErrorCB ErrorCallback {
		public set(SocketErrorCB onError) {
			SocketSetErrorCallback(this, onError);
		}
	}
	
	property any CallbackArgument {
		public set(any value) {
			SocketSetArg(this, value);
		}
	}
	
	public static bool GetHostName(char[] buffer, int maxlength) {
		return SocketGetHostName(buffer, maxlength) == 1;
	}
}
