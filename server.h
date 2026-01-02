#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include "Storage.h"

// Declaration of handleClient so other files can use it
void handleClient(SOCKET client_socket, Storage& storage);

// Declaration of startServer
void startServer(int portnum);

#endif // SERVER_H
