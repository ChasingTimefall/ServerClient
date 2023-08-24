#include "client.h"


DWORD WINAPI ClientThread(CClientObject* Client)
{
    while (Client->HandleClient())
    {
        Sleep(Client->dwProcessInterval);  // Should never be the case
    }

    return EXIT_SUCCESS;
}

void CClientObject::Destroy() {

    if (hClientThread == INVALID_HANDLE_VALUE)
        return;

    bTerminationCall = true;

    Sleep((dwTimeOut * 2) + (dwProcessInterval * 2));

    TerminateThread(hClientThread,EXIT_SUCCESS);

    if (SocketAddress) {
        freeaddrinfo(SocketAddress);
        SocketAddress = nullptr;
    }
}

bool CClientObject::HandleClient() 
{

    int iResult = 0;
    const char* DefaultMessage = (const char*)CryptString("Null Message\n");

    PVOID Buffer = malloc(dwMaximumBufferSize);

    if (!Buffer)
        return true;

    SOCKET ConnectSocket = socket(SocketAddress->ai_family, SocketAddress->ai_socktype,
        SocketAddress->ai_protocol);

    if (ConnectSocket == INVALID_SOCKET) {
        free(Buffer);
        return false;
    }

    iResult = connect(ConnectSocket, SocketAddress->ai_addr, (int)SocketAddress->ai_addrlen);

    if (iResult == SOCKET_ERROR) {
        free(Buffer);
        closesocket(ConnectSocket);
        return false;
    }

    setsockopt(ConnectSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&dwTimeOut, sizeof(dwTimeOut));
    setsockopt(ConnectSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&dwTimeOut, sizeof(dwTimeOut));
   
    OnConnect(this);

    do {

        if (bTerminationCall)
            return false;

        Sleep(dwProcessInterval);

        RtlZeroMemory(Buffer, dwMaximumBufferSize);

        size_t sizeBytesToSend = 0;
        bool bDropConnection = PreMessageSend(this,Buffer,&sizeBytesToSend);

        if (bDropConnection)
            goto DROP_CONNECTION;

        if (sizeBytesToSend>0)
            iResult = send(ConnectSocket, (const char*)Buffer, sizeBytesToSend, 0);
        else
            iResult = send(ConnectSocket, DefaultMessage, strlen(DefaultMessage), 0);
      
        if (iResult == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAETIMEDOUT)
                goto DROP_CONNECTION_TIMEOUT;
        }

        RtlZeroMemory(Buffer, dwMaximumBufferSize);
        iResult = recv(ConnectSocket, (char*)Buffer, dwMaximumBufferSize, 0);

        if (iResult == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAETIMEDOUT)
                goto DROP_CONNECTION_TIMEOUT;
        }

        if (iResult > 0)
            OnMessageReceive(this, Buffer, dwMaximumBufferSize);
        else   
            goto DROP_CONNECTION;

    } while (iResult > 0);

DROP_CONNECTION:

    closesocket(ConnectSocket);
    OnConnectionDrop(this);
    free(Buffer);
    return false;

DROP_CONNECTION_TIMEOUT:
    closesocket(ConnectSocket);
    OnConnectionDrop(this);
    free(Buffer);

    return false;
}

bool CClientObject::Setup(const char* szAddress, size_t sizeOfBuffer, DWORD dwPort, FnClientOnMessageReceive _OnMessageReceive,FnClientPreMessageSend _PreMessageSend, FnClientOnConnectionDrop _OnConnectionDrop,FnClientOnConnect _OnConnect, DWORD dwTimeoutMilliseconds, DWORD _dwProcessInterval) {

    bTerminationCall = false;
    dwTimeOut = dwTimeoutMilliseconds;
    OnMessageReceive = _OnMessageReceive;
    PreMessageSend = _PreMessageSend;
    OnConnectionDrop = _OnConnectionDrop;
    OnConnect = _OnConnect;
    dwMaximumBufferSize = sizeOfBuffer;
    dwProcessInterval = _dwProcessInterval;

    struct addrinfo* result = NULL,
        * ptr = NULL,
        hints;

    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (iResult != 0) 
        return false;
    
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    iResult = getaddrinfo(szAddress, std::to_string(dwPort).c_str(), &hints, &SocketAddress);

    if (iResult != 0) 
        return false;
    
    hClientThread =  CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&ClientThread, this, NULL, NULL);

   if (hClientThread == INVALID_HANDLE_VALUE) {
       freeaddrinfo(SocketAddress);
       WSACleanup();
       return false;
   }

    return true;
}