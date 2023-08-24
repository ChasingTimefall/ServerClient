#pragma once

#include "../core.h"

#define VICEAPI __declspec(dllimport)

class CClientListener;
class CViceServer;

using FnServerOnMessageReceive = void(*)(CClientListener* Listener, PVOID BufferIn, size_t sizeOfBuffer);
using FnServerPreMessageSend = bool(*)(CClientListener* Listener, PVOID BufferOut, size_t* sizeOfBuffer);
using FnServerOnConnectionDrop = void(*)(CClientListener* Listener);
using FnServerOnConnect = void(*)(CClientListener* Listener);

VICEAPI CViceServer* CreateServer(size_t sizeOfBuffer, DWORD dwPort, FnServerOnMessageReceive _OnMessageReceive, FnServerPreMessageSend _PreMessageSend,
    FnServerOnConnectionDrop _OnConnectionDrop, FnServerOnConnect _OnConnect, DWORD dwTimeoutMilliseconds, DWORD _dwProcessingDelay, bool bSetupLocalServer);

VICEAPI VOID DestroyServer(CViceServer* ViceServer);

