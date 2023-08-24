#include "serverapi.h"
#include "../server/server.h"

CViceServer* CreateServer(size_t sizeOfBuffer, DWORD dwPort, FnServerOnMessageReceive _OnMessageReceive, FnServerPreMessageSend _PreMessageSend,
    FnServerOnConnectionDrop _OnConnectionDrop, FnServerOnConnect _OnConnect, DWORD dwTimeoutMilliseconds, DWORD _dwProcessingDelay, bool bSetupLocalServer)
{
    CServerObject* ViceServer = new CServerObject;

    if (ViceServer->Setup(sizeOfBuffer, dwPort, _OnMessageReceive, _PreMessageSend,
        _OnConnectionDrop, _OnConnect, dwTimeoutMilliseconds, _dwProcessingDelay, bSetupLocalServer))
        return (CViceServer*)ViceServer;

    delete ViceServer;

    return nullptr;
}

VOID DestroyServer(CViceServer* ViceServer) {

    if (!ViceServer)
        return;

    ((CServerObject*)ViceServer)->Destroy();
    delete ((CServerObject*)ViceServer);
}