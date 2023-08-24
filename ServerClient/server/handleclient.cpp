#include "server.h"

bool CClientListener::HandleClient() {

    int iResult = 0;
    int iSendResult = 0;

    const char* DefaultMessage = (const char*)CryptString("Null Message\n");

    PVOID Buffer = malloc(dwMaximumBufferSize);

    if (!Buffer)
        return true;

    setsockopt(ClientSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&dwTimeOut, sizeof(dwTimeOut));
    setsockopt(ClientSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&dwTimeOut, sizeof(dwTimeOut));

    OnConnect(this);

    do {

        if (bTerminationCall)
            return false;

        Sleep(dwProcessingDelay);

        RtlZeroMemory(Buffer, dwMaximumBufferSize);
        iResult = recv(ClientSocket, (char*)Buffer, dwMaximumBufferSize, 0);
       
        if (iResult == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAETIMEDOUT)
                goto DROP_CONNECTION_TIMEOUT;
        }

        if (iResult > 0) {

            OnMessageReceive(this, Buffer, iResult);

            size_t sizeBytesToSend = 0;

            RtlZeroMemory(Buffer, dwMaximumBufferSize);

            bool bShouldDropConnection = PreMessageSend(this, Buffer, &sizeBytesToSend);

            if (bShouldDropConnection)
                goto DROP_CONNECTION;

            if (sizeBytesToSend > 0)
                iSendResult = send(ClientSocket, (const char*)Buffer, sizeBytesToSend, 0);
            else
                iSendResult = send(ClientSocket, (const char*)DefaultMessage, strlen(DefaultMessage), 0);

            if (iSendResult == SOCKET_ERROR)
            {
                if (WSAGetLastError() == WSAETIMEDOUT)
                    goto DROP_CONNECTION_TIMEOUT;
                else
                    goto DROP_CONNECTION;
            }

        }
        else 
            goto DROP_CONNECTION;

    } while (iResult > 0);

DROP_CONNECTION:
    closesocket(ClientSocket);
    OnConnectionDrop(this);
    free(Buffer);
    return false;

DROP_CONNECTION_TIMEOUT:
    closesocket(ClientSocket);
    OnConnectionDrop(this);
    free(Buffer);
	return false;
}