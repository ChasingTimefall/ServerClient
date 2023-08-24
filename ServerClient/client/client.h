#pragma once
#include "../core.h"
#include "../api/clientapi.h"


class CClientObject {
public:

	bool Setup(const char* szAddress , size_t sizeOfBuffer, DWORD dwPort, FnClientOnMessageReceive OnMessageReceive,
		FnClientPreMessageSend PreMessageSend, FnClientOnConnectionDrop OnConnectionDrop, 
		FnClientOnConnect OnConnect, DWORD dwTimeoutMilliseconds= 10000,DWORD dwProcessInterval = 10);

	bool HandleClient();
	void Destroy();

public:
	DWORD dwPort = 0;
	WSADATA wsaData;
	addrinfo* SocketAddress = nullptr;
	DWORD dwTimeOut = 0;
	DWORD dwMaximumBufferSize = 0;
	DWORD dwProcessInterval = 0;
	HANDLE hClientThread = INVALID_HANDLE_VALUE;
	bool bTerminationCall = false;
private:
	FnClientOnMessageReceive OnMessageReceive;
	FnClientPreMessageSend PreMessageSend; 
	FnClientOnConnectionDrop OnConnectionDrop;
	FnClientOnConnect OnConnect;
private:
	BYTE* Buffer = nullptr;
};