#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <thread>

#pragma comment(lib, "ws2_32.lib") 

void handleClient(SOCKET client_socket) {
    std::cout << "Client connected\n";
    char buffer[512];
    int bytesReceived;


    while ((bytesReceived = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
        send(client_socket, buffer, bytesReceived, 0); 
    }

    closesocket(client_socket);
    std::cout << "Client disconnected\n";
}

void startServer(int portnum) {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "WSAStartup failed\n";
        return;
    }

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        WSACleanup();
        return;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(portnum);

    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed\n";
        closesocket(server_socket);
        WSACleanup();
        return;
    }

    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed\n";
        closesocket(server_socket);
        WSACleanup();
        return;
    }

    std::cout << "Server listening on port " << portnum << "...\n";

    while (true) {
        SOCKET client_socket = accept(server_socket, nullptr, nullptr);
        if (client_socket == INVALID_SOCKET) {
            std::cerr << "Accept failed\n";
            continue; 
        }

        std::thread(handleClient, client_socket).detach();
    }

    closesocket(server_socket);
    WSACleanup();
}

int main() {
    startServer(6379);
    return 0;
}
