#include <iostream>
#include "server.hpp"

HttpServer::HttpServer(const std::string &PORT)
{
    M_PORT = PORT;
}

HttpServer::~HttpServer()
{
    // frees the linked list
    freeaddrinfo(m_serverInfo);
    close(m_sckt);
}

int HttpServer::setup()
{
    int status{};

    // resets hints making all values of zero
    memset(&m_hints, 0, sizeof m_hints);
    m_hints.ai_family = AF_INET; // IPV4
    m_hints.ai_socktype = SOCK_STREAM; // TCP
    m_hints.ai_flags = AI_PASSIVE; // Passing or own addr

    // creating the guard socket and binding it to port
        // NULL cause its our own addr
    // port
    // form
    // linked list where OS will write results
    // returns -1 if error
    status = getaddrinfo(NULL, M_PORT.c_str(), &m_hints, &m_serverInfo);
    if (status != 0)
    {
        std::cerr << "Error on getaddrinfo: " << gai_strerror(status) << std::endl;
        return EXIT_FAILURE;
    }

    // create socket: socket(domain, type, protocol) 0 = default protocol
    m_sckt = socket(m_serverInfo->ai_family, m_serverInfo->ai_socktype, m_serverInfo->ai_protocol);
    if (m_sckt < 0)
    {
        std::cerr << "Error while generating socket " << errno << std::endl;
        return EXIT_FAILURE;
    }

    // bind - binds a socket to a port
    // bind receives sckt, address info and adress lenght
    if (bind(m_sckt, m_serverInfo->ai_addr, m_serverInfo->ai_addrlen) == -1)
    {
        std::cerr << "Error while binding socket " << errno << std::endl;
        return EXIT_FAILURE;
    }

    // listen -- make socket hears from call
    // listen receives a socket and a backlog
    if (listen(m_sckt, M_BACKLOG) == -1)
    {  

        std::cerr << "Error on listening port " << errno << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void HttpServer::handleRequest(int clientSocket)
{
    ;
}

int HttpServer::run()
{
        // main loop
    while (true){

        // Someone will use connext to the port we are listening
        // accept will create a new socket to handle this new connection
        // accept receives socket, sockaddr address and socklen
        struct sockaddr_storage clientAddr; // stores clients IP
        socklen_t addrSize = sizeof(clientAddr);

        // receives sckt, only server address and a client address (where we write the new IP)
        // reinterpret_cast allows to remap bits, less safety, but useful in this situations 
        int newSckt = accept(m_sckt, reinterpret_cast<sockaddr *>(&clientAddr), &addrSize);
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
        std::string msg = "HTTP/1.1 200 OK \r\n" "Content-Type: text/plain\r\n" "Content-Length: 12\r\n" "\r\n" "Hello World!";
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
    return EXIT_SUCCESS;
}