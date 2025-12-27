/*
    HTTP Server in C++ folowwing Beej`s Guide
*/

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <vector>

std::string PORT = "8080";
constexpr int BACKLOG = 10;

int main()
{
    int status {};
    // gives restrictions (hints) for the OS)
    struct addrinfo hints; 
    struct addrinfo *serverInfo;

    // resets hints making all values of zero
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // IPV4
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE; // Passing or own addr
    
    // NULL cause its our own addr
    // port
    // form
    // linked list where OS will write results
    // returns -1 if error
    status = getaddrinfo(NULL, PORT.c_str(), &hints, &serverInfo);
    if (status != 0)
    {
        std::cerr << "Error on getaddrinfo: " << gai_strerror(status) << std::endl;
        return EXIT_FAILURE;
    }

    // create socket: socket(domain, type, protocol) 0 = default protocol
    int sckt = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);
    if (sckt < 0)
    {
        std::cerr << "Error while generating socket " << errno << std::endl;
        return EXIT_FAILURE;
    }

    // bind - binds a socket to a port
    // bind receives sckt, address info and adress lenght
    if (bind(sckt, serverInfo->ai_addr, serverInfo->ai_addrlen) == -1)
    {
        std::cerr << "Error while binding socket " << errno << std::endl;
        return EXIT_FAILURE;
    }

    // main loop
    while (true){

        // listen -- make socket hears from call
        // listen receives a socket and a backlog
        if (listen(sckt, BACKLOG) == -1)
        {  

            std::cerr << "Error on listening port " << errno << std::endl;
            return EXIT_FAILURE;
        }

        // Someone will use connext to the port we are listening
        // accept will create a new socket to handle this new connection
        // accept receives socket, sockaddr address and socklen
        struct sockaddr_storage clientAddr; // stores clients IP
        socklen_t addrSize = sizeof(clientAddr);

        // receives sckt, only server address and a client address (where we write the new IP)
        // reinterpret_cast allows to remap bits, less safety, but useful in this situations 
        int newSckt = accept(sckt, reinterpret_cast<sockaddr *>(&clientAddr), &addrSize);
        if (newSckt == -1)
        {
            std::cerr << "Error while accepting connection " << errno << std::endl;
            return EXIT_FAILURE;
        }

        // thinking of sckt as a external guard and newSckt as the bartender inside the show
        // now we use recv and send functions (they work with array buffers
        std::vector<char> buffer;
        buffer.resize(1024);
        // recv takes a socket, a buffer to put data into, the maximum lenght of buffer and flags
        // flag 0 is default
        int bytesRead = recv(newSckt, buffer.data(), buffer.size(), 0);
        if (bytesRead == -1)
        {
            std::cerr << "Error while writing to buffer " << errno << std::endl;
            return EXIT_FAILURE;
        }

        // after receving, we sent a response
        std::string msg = "HTTP/1.1 200 OK \r\n" "Content-Type: text/plain\r\n" "Content-Lenght: 12\r\n" "\r\n" "Hello World!";
        int bytesSent {};
        bytesSent = send(newSckt, msg.c_str(), msg.size(), 0);
        if (bytesSent == -1)
        {
            std::cerr << "Error while returning response " << errno << std::endl;
            return EXIT_FAILURE;
        }
        // closes the files descriptors
        close(newSckt);
        }
    close(sckt);
    
    return 0;
}
