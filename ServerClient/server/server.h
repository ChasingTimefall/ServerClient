#pragma once
#include "../core.h"
#include "../api/serverapi.h"


class CServerObject;
class CClientListener {
public:

	CClientListener(CServerObject* _OwningServer,SOCKET _ClientSocket,DWORD _dwMaximumBufferSize, 
		FnServerOnMessageReceive _OnMessageReceive,FnServerPreMessageSend _PreMessageSend,
		FnServerOnConnectionDrop _OnConnectionDrop, FnServerOnConnect _OnConnect, DWORD dwTimeoutMilliseconds = 10000,DWORD _dwProcessingDelay = 10) {
		ClientSocket = _ClientSocket;
		OwningServer = _OwningServer;
		OnMessageReceive = _OnMessageReceive;
		PreMessageSend = _PreMessageSend;
		OnConnectionDrop = _OnConnectionDrop;
		OnConnect = _OnConnect;
		dwMaximumBufferSize = _dwMaximumBufferSize;
		dwTimeOut = dwTimeoutMilliseconds;
		bTerminationCall = false;
		dwProcessingDelay = _dwProcessingDelay;
	}

	bool HandleClient();
public:
	HANDLE hClientThread = INVALID_HANDLE_VALUE;
	bool bTerminationCall = false;
	CServerObject* OwningServer = nullptr;
	SOCKET ClientSocket = INVALID_SOCKET;
private:
	FnServerOnMessageReceive OnMessageReceive = NULL;
	FnServerPreMessageSend PreMessageSend = NULL;
	FnServerOnConnectionDrop OnConnectionDrop = NULL;
	FnServerOnConnect OnConnect = NULL;
	DWORD dwMaximumBufferSize = 0;
	DWORD dwTimeOut = 0;
	DWORD dwProcessingDelay = 0;
};

class CServerObject {
public:
	bool Setup(size_t sizeOfBuffer, DWORD dwPort, FnServerOnMessageReceive _OnMessageReceive, FnServerPreMessageSend _PreMessageSend,
		FnServerOnConnectionDrop _OnConnectionDrop, FnServerOnConnect _OnConnect,DWORD dwTimeoutMilliseconds = 10000,DWORD _dwProcessingDelay = 10, bool bSetupLocalServer = false);
	void Accept();
	void Destroy();
	void RemoveClient(CClientListener* ClientInfo);
public:
	WSADATA wsaData;
	SOCKET ListenSocket = INVALID_SOCKET;
	DWORD dwPort = 0;
	DWORD dwMaximumBufferSize = 0;
	DWORD dwTimeOut = 0;
	DWORD dwProcessingDelay = 0;
private:
	FnServerOnMessageReceive OnMessageReceive = NULL;
	FnServerPreMessageSend PreMessageSend = NULL;
	FnServerOnConnectionDrop OnConnectionDrop = NULL;
	FnServerOnConnect OnConnect = NULL;
	HANDLE hServerThread = INVALID_HANDLE_VALUE;
	std::deque<CClientListener*> vecClients;
	std::shared_mutex Mutex;
};