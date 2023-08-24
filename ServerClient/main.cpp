#include "core.h"
#include <iostream>
#include "server/server.h"
#include "client/client.h"


void MessageReceive(CClientListener* Listener,PVOID BufferIn, size_t sizeOfBuffer)
{
    std::cout << "[Server] Message Received : " << (const char*)BufferIn << "\n";
}

bool PreSendMessage(CClientListener* Listener,PVOID BufferOut, size_t* sizeOfBuffer)
{
    char BufferTest[] = "Sending from Vice Server";

    *sizeOfBuffer = sizeof(BufferTest);

    RtlCopyMemory(BufferOut, BufferTest, sizeof(BufferTest));

    std::cout << "[Server] Sending Message : "<< *sizeOfBuffer << "\n";

    return false;
}

void OnConnectionDrop(CClientListener* Listener)
{
    std::cout << "[Server] Connection Dropped\n";
}

void OnConnect(CClientListener* Listener)
{
    std::cout << "[Server] Connected\n";
}

void ClientMessageReceive(CClientObject* Client, PVOID BufferIn, size_t sizeOfBuffer)
{
    std::cout << "[Client] Message Received : " << (const char*)BufferIn << "\n";
}

bool ClientPreSendMessage(CClientObject* Client, PVOID BufferOut, size_t* sizeOfBuffer)
{
    char BufferTest[] = "Sending from Vice Client";

    *sizeOfBuffer = sizeof(BufferTest);

    RtlCopyMemory(BufferOut, BufferTest, sizeof(BufferTest));

    std::cout << "[Client] Sending Message : " << *sizeOfBuffer << "\n";

    return false;
}

void ClientOnConnectionDrop(CClientObject* Client)
{
    std::cout << "[Client] Connection Dropped\n";
}

void ClientOnConnect(CClientObject* Client)
{
    std::cout << "[Client] Connected\n";
}

/*
int DllMain()
{
AGAIN:
    CServerObject* ServerObject = new CServerObject;
   
    bool bSetup = false;
     bSetup = ServerObject->Setup( 0x1000, 27015, &MessageReceive, &PreSendMessage, &OnConnectionDrop, &OnConnect, 1000,1);

    std::cout << "Server Setup : " << bSetup << "\n";

    CClientObject* ClientObject = new CClientObject;

    std::cout << "Client Setup : " << ClientObject->Setup("127.0.0.1",0x1000, 27015, &ClientMessageReceive, &ClientPreSendMessage, &ClientOnConnectionDrop, &ClientOnConnect,1000,1) << "\n";
    
   // std::cout << "Destroying Server\n";
    //ServerObject->Destroy();
   
  //  std::cout << "Destroying Client\n";
   // ClientObject->Destroy();

  //  delete ServerObject;
   // delete ClientObject;

    std::cout << "Finished\n";

    while (true)
    {
        Sleep(INT_MAX);
    }


    system("pause");


  //  std::cout << ClientObject.Setup(0x1000, 27015) << "\n";

    /*
    while (true)
    {
        std::cout << "Sending\n";
        char Bytes[25] = {0};
        int iResult = 0;
        iResult = send(ServerObject.ListenSocket, Bytes, 2, 0);
       
        if (iResult == SOCKET_ERROR) {
            printf("send failed with error: %d\n", WSAGetLastError());
        }

        Sleep(1000);
    }
    */


    /*
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
  
    struct addrinfo* result = NULL,
        * ptr = NULL,
        hints;

    const char* sendbuf = "this is a test";
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    // Send an initial buffer
    iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    printf("Bytes Sent: %ld\n", iResult);

    // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // Receive until the peer closes the connection
    do {

        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0)
            printf("Bytes received: %d\n", iResult);
        else if (iResult == 0)
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while (iResult > 0);

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();


    /*
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo* result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    closesocket(ListenSocket);

    do {

        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);

            iSendResult = send(ClientSocket, recvbuf, iResult, 0);
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
            }
            printf("Bytes sent: %d\n", iSendResult);
        }
        else if (iResult == 0)
            printf("Connection closing...\n");
        else {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }

    } while (iResult > 0);

    std::cout << "7\n";

    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }

    closesocket(ClientSocket);
    WSACleanup();
    

    return 0;
}*/