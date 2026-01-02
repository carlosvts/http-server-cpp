# Multithreaded HTTP Server in C++

![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![Network](https://img.shields.io/badge/Network-Sockets-blue)

A low-level HTTP server developed in C++ following the principles of [**Beej's Guide to Network Programming**](https://beej.us/guide/bgnet/html/split/index.html). This project explores the inner workings of the HTTP protocol, POSIX socket management, and concurrency using multithreading.

## About the Project

This project implements a web server capable of handling real-time requests from a browser.  [cite_start]It supports serving static files (HTML, CSS, images) and processing user-submitted data via GET and POST methods. 

**Main Objectives:**
* Understand the lifecycle of a server socket: `getaddrinfo`, `socket`, `bind`, `listen`, and `accept`. 
* Implement real concurrency using `std::thread` to serve multiple devices (mobile, PC, etc.) simultaneously without blocking the main loop. 
* Manually parse HTTP headers and extract data from the request body. 

The architecture uses an `HttpServer` class to manage the port listener, while each accepted connection is delegated to an independent `worker` thread using `detach()`.

## Technologies

* **Language:** C++17 
* **Networking API:** POSIX Sockets (`sys/socket.h`) 
* **Concurrency:** ISO C++ Threads (`<thread>`) 
* **Compiler:** g++ 

## How to Build and Run

### Prerequisites

Ensure you are on a Unix-like environment (Linux or macOS) with **g++** installed. 

#### Linux Dependencies (Debian/Ubuntu)

```bash
sudo apt-get install g++
```

#### Compiling & Running
```bash
g++ main.cpp server.cpp -o httpserver
./httpserver <port>
```
example: ./httpserver 8080
* **Reminder:** Ports from 0 to 1023 may be blocked by the OS and need admin/root permissions.

* You can access the server at http://localhost:8080
### Static Files
* **Path:** `/` or `/index.html`
* **Description:** Serves the main HTML interface. It includes the CSS styling and the interactive form used to test POST requests.
* **Method:** `GET`

### Echo Service
* **Path:** `/api/echo`
* **Description:** A diagnostic tool that returns the **entire raw HTTP request** sent by the client.It is wrapped in `<pre>` tags to preserve formatting.
* **Method:** `GET`

### User-Agent Inspector
* **Path:** `/api/user-agent`
* **Description:** Parses the request headers and extracts the `User-Agent` string, identifying which browser or tool is accessing the server.
* **Method:** `GET`

### Personal Salute
* **Path:** `/api/salute`
* **Description:** Greets the user based on URL query parameters.
* **Example:** `/api/salute?name=John&age=38`
* **Output:** Displays a header with "Hello John! Your age is: 38".
* **Method:** `GET`

## Project Structure
``` Markdown
📁 Project
├── 📄 main.cpp        # Entry point: handles CLI arguments and starts the server loop
├── 📄 server.cpp      # Implementation of routing logic, GET/POST handling, and Sockets
├── 📄 server.hpp      # HttpServer class definition and data structures
├── 📄 index.html      # Frontend interface with a POST form for testing
```

## Reminder
This project is under development and was made only for learning porpouses, this code should not be used in any place that needs a real HTTP server since it doesnt have security measures yet.
I will revisit this project on and so on to improving it when i have the necessity to learn new things related to that.
