#pragma once

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <shared_mutex>
#include <mutex>
#pragma comment (lib, "Ws2_32.lib")
#include "dependencies/crypt.h"

#define SERVER
#define CLIENT