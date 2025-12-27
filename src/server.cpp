#include <iostream>
#include <sstream>
#include <fstream>

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

int HttpServer::handleRequest(int clientSocket)
{
    
    // thinking of sckt as a external guard and newSckt (clientSocket) as the bartender inside the show
    // now we use recv and send functions (they work with array buffers
    std::vector<char> buffer;
    buffer.resize(1024);

    // recv takes a socket, a buffer to put data into, the maximum lenght of buffer and flags
    // flag 0 is default
    // receives in bytes the client request
    // recv always receive the same format [METHOD] [SPACE] [PATH] [SPACE] [VERSION]
    // then header files like Host: localhost:8080, User-Agent: firefox
    // finally some data (if exists)
    int bytesRead = recv(clientSocket, buffer.data(), buffer.size(), 0);
    if (bytesRead == -1)
    {
        std::cerr << "Error while writing to buffer " << errno << std::endl;
        return EXIT_FAILURE;
    }
    
    // after receving, we sent a response
    // we send in this pattern (three lines)
    // 1. HTTP/1.1 [STATUS_CODE] [TEXT]
    // 2. headers like Content-Type, Content-Lenght
    // 3. text (data, bytes, video, etc)
    std::string request {buffer.begin(), (buffer.begin() + bytesRead)};
    
    // Parsing client request
    std::stringstream ss(request);
    std::string method, path, protocol;
    //  operator>> stops on spaces, so this work
    ss >> method >> path >> protocol;
    
    // PATHS
    if (path == "/") { path = "/index.html"; }
    if (path == "/api/user-agent") 
    {
        std::string searchKey = "User-Agent: ";
        std::string userAgent;
        size_t startPos = request.find(searchKey); // returns pointer to first occurance
        if(startPos != std::string::npos)
        {
            startPos += searchKey.length();
            size_t endPos = request.find("\r");
            userAgent = request.substr(startPos, endPos - startPos);
        }
        // now we prepare and send a response
        std::string response = "User-Agent: " + userAgent;
        std::string header = "HTTP/1.1 200 OK\r\n";
        header += "Content-Type: text/plain; charset=utf-8\r\n";
        header += "Content-Length: " + std::to_string(response.size()) + "\r\n";
        header += "\r\n";

        // send response
        int headerBytesSent = send(clientSocket, header.c_str(), header.size(), 0);
        if (headerBytesSent == -1)
        {
            std::cerr << "Error while returning response " << errno << std::endl;
            return EXIT_FAILURE;
        }

        int dataBytesSent = send(clientSocket, response.data(), response.size(), 0);    
        if (dataBytesSent == -1)
        {
            std::cerr << "Error while returning response " << errno << std::endl;
            return EXIT_FAILURE;    
        }
        return EXIT_SUCCESS;
    }
    std::string localPath = path.substr(1); // Gets a substring starting at index 1 (i.e, eliminates the backslash)
    


    // Reading binary            read binary         point to the end of the file
    std::ifstream file(localPath, std::ios::binary | std::ios::ate);
    
    if (!file.is_open())
    {
        // basic not found
        std::string response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
        send(clientSocket, response.c_str(), response.size(), 0);
        return EXIT_FAILURE;
    }
    
    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    
    // creating a out buffer
    std::vector<char> fileBuffer;
    fileBuffer.resize(fileSize);

    // populating buffer with data
    if (!file.read(fileBuffer.data(), fileSize))
    {
        return EXIT_FAILURE;
    }
    // Preparing the MIME type
    std::string header = "HTTP/1.1 200 OK\r\n";
    std::string extension = localPath.substr(localPath.find_last_of(".")); // last occuerence of "."
    std::string mimeType = "text/plain";

    if (extension == ".html") { mimeType = "text/html"; }
    else if (extension == ".css") { mimeType = "text/css"; }
    else if (extension == ".png") { mimeType = "image/png"; }
    else if (extension == ".jpg" || extension == ".jpeg") { mimeType = "image/jpeg"; }

    // now that MIME is parsed, we send the response
    header += "Content-Type: " + mimeType + "\r\n";
    header += "Content-Length: " + std::to_string(fileSize) + "\r\n";
    header += "\r\n";

    // sending header and data
    int headerBytesSent = send(clientSocket, header.c_str(), header.size(), 0);
    if (headerBytesSent == -1)
    {
        std::cerr << "Error while returning response " << errno << std::endl;
        return EXIT_FAILURE;
    }

    int dataBytesSent = send(clientSocket, fileBuffer.data(), fileBuffer.size(), 0);    
    if (dataBytesSent == -1)
    {
        std::cerr << "Error while returning response " << errno << std::endl;
        return EXIT_FAILURE;    
    }
    return EXIT_SUCCESS;
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

        handleRequest(newSckt);

        // closes the files descriptors
        close(newSckt);
        }
    return EXIT_SUCCESS;
}