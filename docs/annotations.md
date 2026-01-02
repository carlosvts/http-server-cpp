# Understanding some concepts about network
---

## Sockets
Working at transport layer on OSI Model.
- **What is**: A abstraction provided by OS that allows communication between different processes. Its like a bridge between two machines that are communicating via internet.
- **How**: It wraps a combination of a Protocol, a IP Address and a Port Number
Example: 192.168.1.255:3389 (IP + Port) via TCP/UDP
The OS uses this information wrapped by the socket to send and receive data.
The connection occours using a Socket 5-tuple
{protocol, local-ip, local-port, remote-ip, remote-port}
Example: {tcp, 130.254.2.11, 8080, 130.290.3.10, 1023}

Inside our computer, we use Unix Domain Sockets (UDS) to communicate between files and process inside or own machine, bypassing network layer

---
## TCP and UDP
### TCP (Transmission Control Protocol)
- Three Way Handshake client -> server -> client -> server
- Flow control, Connection-oriented
- Reliable, ordered, error-checked
- Ensures that packaged will arrive in order
- Usage: email, web, file transfer 
### UDP (User Datagram Protocol)
- Connectionless and unreliable
- Sends information really fast to the server without any guarantee
- Usage: Video calls, multiplayer games, streaming (In this situations, loosing a frame doesnt matter too much)

## How does a Http connection work?
A http connection works like a conversation where sockets "guards" the place that you are trying to go and talk to
First you need a outer socket that will `listen` to any requests of talking. When he listens, he them creates another socket (i will call `newsckt`) to estabilish the communication between the request and the proccess of the computer, like a httpResponse. The outer socket will still be there waiting for other people that wants to start a conversation (that in TCP is a three way handshake). If any other exists: he creates another `newsckt` to intermediate the conversation. The guard socket will keep adding people to the `Backlog`  only when its ready that a new instance of socket is created by the OS, when its ready it will `accepts` the connection.

# Another topics

## Multithreading
Is a technicque that allows the computer to perform multiple tasks at the same time (concurrently). In the context of a HTTP Server, we can use it to manage multiple inner sockets, allowing more connections and issues with laging.

In C++ content, we need to include <threads>
