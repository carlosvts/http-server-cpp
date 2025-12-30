/*
    HTTP Server in C++ folowwing Beej`s Guide
*/

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <string>

#include "server.hpp"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <port>";
        return 1;
    }
    std::string port = argv[1];

    HttpServer httpServer(port);
    if (httpServer.setup() == 1) { return 1; }
    std::cout << "Server running at port " << port << std::endl;
    if (httpServer.run() == 1 ) { return 1; }  

    return 0;
} 
