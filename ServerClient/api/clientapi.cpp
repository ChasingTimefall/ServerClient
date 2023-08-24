#include "clientapi.h"
#include "../client/client.h"

CViceClient* CreateClient(const char* szAddress, size_t sizeOfBuffer, DWORD dwPort, FnClientOnMessageReceive OnMessageReceive,
	FnClientPreMessageSend PreMessageSend, FnClientOnConnectionDrop OnConnectionDrop,
	FnClientOnConnect OnConnect, DWORD dwTimeoutMilliseconds , DWORD dwProcessInterval ) 
{
    CClientObject* ViceClient = new CClientObject;

    if (ViceClient->Setup(szAddress, sizeOfBuffer, dwPort, OnMessageReceive,
		 PreMessageSend,  OnConnectionDrop,
		 OnConnect,  dwTimeoutMilliseconds,  dwProcessInterval))
        return (CViceClient*)ViceClient;

    delete ViceClient;

	return nullptr;
}

VOID DestroyClient(CViceClient* Client) 
{
    if (!Client)
        return;

    ((CClientObject*)Client)->Destroy();
    delete ((CClientObject*)Client);
}