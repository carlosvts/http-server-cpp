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
Example: {tcp, 130.254.2.11, 8080, 130.290.3.10, 1029}

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
