#pragma once

#include "../core.h"

#define VICEAPI __declspec(dllimport)

class CViceClient;
class CClientObject;

using FnClientOnMessageReceive = void(*)(CClientObject* Client, PVOID BufferIn, size_t sizeOfBuffer);
using FnClientPreMessageSend = bool(*)(CClientObject* Client, PVOID BufferOut, size_t* sizeOfBuffer);
using FnClientOnConnectionDrop = void(*)(CClientObject* Client);
using FnClientOnConnect = void(*)(CClientObject* Client);

VICEAPI CViceClient* CreateClient(const char* szAddress, size_t sizeOfBuffer, DWORD dwPort, FnClientOnMessageReceive OnMessageReceive,
	FnClientPreMessageSend PreMessageSend, FnClientOnConnectionDrop OnConnectionDrop,
	FnClientOnConnect OnConnect, DWORD dwTimeoutMilliseconds, DWORD dwProcessInterval);

VICEAPI VOID DestroyClient(CViceClient* Client);
