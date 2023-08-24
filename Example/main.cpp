#include "netapi/serverapi.h"
#include "netapi/clientapi.h"
#include <iostream>

void MessageReceive(CClientListener* Listener, PVOID BufferIn, size_t sizeOfBuffer)
{
    std::cout << "[Server] Message Received : " << (const char*)BufferIn << "\n";
}

bool PreSendMessage(CClientListener* Listener, PVOID BufferOut, size_t* sizeOfBuffer)
{
    char BufferTest[] = "Sending from Vice Server";

    *sizeOfBuffer = sizeof(BufferTest);

    RtlCopyMemory(BufferOut, BufferTest, sizeof(BufferTest));

    std::cout << "[Server] Sending Message : " << *sizeOfBuffer << "\n";

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


int main() {

    CViceServer* ViceServer = CreateServer(0x1000, 27025, &MessageReceive, &PreSendMessage, &OnConnectionDrop, &OnConnect, 1000, 0,true);

    std::cout << "Server Setup : " << ViceServer << "\n";

    CViceClient* ViceClient = CreateClient("127.0.0.1", 0x1000, 27025, &ClientMessageReceive, &ClientPreSendMessage, &ClientOnConnectionDrop, &ClientOnConnect, 1000, 0);

    std::cout << "Client Setup : " << ViceClient << "\n";

    //Sleep(50);

    //DestroyServer(ViceServer);
   // DestroyClient(ViceClient);


    while (true)
    {
        Sleep(INT_MAX);
    }

    return 0;
}