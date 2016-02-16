#pragma once

#include <stdio.h>
#include <string.h>
#include <assert.h>

#ifdef WIN32
#include <WinSock2.h>
#include <io.h>
#else //WIN32
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#endif //WIN32

#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

using namespace std;

#include "CommonTypes.h"
