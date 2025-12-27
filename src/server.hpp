#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <vector>

class HttpServer
{
    private:
        int m_sckt;
        const int M_BACKLOG {10};
        std::string M_PORT;
        struct addrinfo m_hints;
        struct addrinfo *m_serverInfo;

    public:
        // Initializes a httpserver at port PORT
        HttpServer(const std::string &PORT);
        // Destructor (deallocate memory and close sockets)
        ~HttpServer();
        // We dont want a copy constructor
        HttpServer(const HttpServer& other) = delete;

        // while logic
        int run();

        // setup logic for socket -> bind -> listen
        int setup();
        // handles http connection
        int handleRequest(int clientSocket);
};
#endif