#include "../core.h"

#define VICEAPI __declspec(dllexport)

class CViceClient;
class CClientObject;

using FnClientOnMessageReceive = void(*)(CClientObject* Client, PVOID BufferIn, size_t sizeOfBuffer);
using FnClientPreMessageSend = bool(*)(CClientObject* Client, PVOID BufferOut, size_t* sizeOfBuffer);
using FnClientOnConnectionDrop = void(*)(CClientObject* Client);
using FnClientOnConnect = void(*)(CClientObject* Client);

//Dont replace CClientObject by mistake while making new class for data
VICEAPI CViceClient* CreateClient(const char* szAddress, size_t sizeOfBuffer, DWORD dwPort, FnClientOnMessageReceive OnMessageReceive,
	FnClientPreMessageSend PreMessageSend, FnClientOnConnectionDrop OnConnectionDrop,
	FnClientOnConnect OnConnect, DWORD dwTimeoutMilliseconds , DWORD dwProcessInterval);

VICEAPI VOID DestroyClient(CViceClient* Client);

