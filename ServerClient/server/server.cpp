#include "server.h"

#define DEFAULT_BUFLEN 512

DWORD WINAPI ListenerThread(CServerObject* ServerObject)
{
    while (true)
    {
        ServerObject->Accept();
    }
}

bool CServerObject::Setup( size_t sizeOfBuffer, DWORD dwPort, FnServerOnMessageReceive _OnMessageReceive, FnServerPreMessageSend _PreMessageSend,
    FnServerOnConnectionDrop _OnConnectionDrop, FnServerOnConnect _OnConnect, DWORD dwTimeoutMilliseconds, DWORD _dwProcessingDelay,bool bSetupLocalServer) {

    OnMessageReceive = _OnMessageReceive;
    PreMessageSend = _PreMessageSend;
    OnConnectionDrop = _OnConnectionDrop;
    OnConnect = _OnConnect;
    dwMaximumBufferSize = sizeOfBuffer;
    dwTimeOut = dwTimeoutMilliseconds;
    dwProcessingDelay = _dwProcessingDelay;

    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (iResult != 0)
        return false;


    struct addrinfo* result = NULL;
    struct addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    if (bSetupLocalServer)
    {
        iResult = getaddrinfo(NULL, std::to_string(dwPort).c_str(), &hints, &result);

        if (iResult != 0)
            return false;

        ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

        if (ListenSocket == INVALID_SOCKET) {
            freeaddrinfo(result);
            return false;
        }

          iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);

        if (iResult == SOCKET_ERROR) {
              freeaddrinfo(result);
            closesocket(ListenSocket);
            return false;
        }

         freeaddrinfo(result);
    }
    else {

        ListenSocket = socket(AF_INET, SOCK_STREAM, 0);

        if (ListenSocket == INVALID_SOCKET)
            return false;
        
        sockaddr_in hint;
        hint.sin_family = AF_INET;
        hint.sin_port = dwPort;
        hint.sin_addr.S_un.S_addr = INADDR_ANY;

        iResult = bind(ListenSocket, (sockaddr*)&hint, sizeof(hint));

        if (iResult == SOCKET_ERROR) {
            closesocket(ListenSocket);
            return false;
        }
    }

    iResult = listen(ListenSocket, SOMAXCONN);

    if (iResult == SOCKET_ERROR) {
        closesocket(ListenSocket);
        return false;
    }

    hServerThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&ListenerThread, this, NULL, NULL);

    if (hServerThread == INVALID_HANDLE_VALUE)
        return false;

    return true;
}

DWORD WINAPI ClientListner(CClientListener* Listener)
{
    while (Listener->HandleClient())
    {
        Sleep(10);
    }
    
    Listener->OwningServer->RemoveClient(Listener);

    delete Listener;

    return EXIT_SUCCESS;
}

void CServerObject::Destroy() {

    WSACleanup();

    if (hServerThread == INVALID_HANDLE_VALUE)
        return;

    if (!vecClients.empty())
    {
        for (auto Current : vecClients)
            Current->bTerminationCall = true;
        
        vecClients.clear();
    }

    Sleep((dwTimeOut * 2) + (dwProcessingDelay * 2));

    TerminateThread(hServerThread,EXIT_SUCCESS);

    if (ListenSocket != INVALID_SOCKET)
        closesocket(ListenSocket);
}

void CServerObject::RemoveClient(CClientListener* ClientInfo) {
   
    std::unique_lock<std::shared_mutex> Lock(Mutex);

    if (vecClients.empty())
        return;

    for (auto It = vecClients.begin(); It != vecClients.end(); It = next(It))
    {
        CClientListener* Current = *It;

        if (ClientInfo == Current) {
            vecClients.erase(It);
            break;
        }

    }

}
void CServerObject::Accept()
{

    SOCKET ClientSocket = accept(ListenSocket, NULL, NULL);

    if (ClientSocket == INVALID_SOCKET) 
        return;

    CClientListener* Listner = new CClientListener(this, ClientSocket, dwMaximumBufferSize, OnMessageReceive, PreMessageSend, OnConnectionDrop , OnConnect, dwTimeOut,dwProcessingDelay);
    HANDLE hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&ClientListner, Listner, NULL, NULL);
  
    if (hThread == INVALID_HANDLE_VALUE)
        return;

    Listner->hClientThread = hThread;

    vecClients.emplace_back(Listner);
}