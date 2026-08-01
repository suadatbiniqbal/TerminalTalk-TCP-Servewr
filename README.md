# TerminalTalk TCP Server

TerminalTalk is a high-performance, multi-threaded TCP chat server and client application implemented in C++. It facilitates real-time communication between multiple users through a terminal-based interface.

## Description

TerminalTalk TCP Server leverages POSIX sockets and C++ threads to manage multiple concurrent client connections. It provides a robust architecture for message broadcasting and client-server interaction, making it an ideal project for understanding network programming and concurrency in a Unix-like environment.

## Key Features

- **Multi-threaded Architecture**: Handles multiple clients simultaneously using dedicated threads.
- **Real-time Broadcasting**: Messages sent by one client are instantly distributed to all other connected participants.
- **Enhanced Terminal Interface**: Utilizes ANSI color codes and stylized banners for an improved user experience.
- **Connection Animations**: Includes terminal-based loading animations during initialization and connection phases.
- **Username Identification**: Each participant is identified by a unique username.
- **Lightweight and Efficient**: Minimal overhead, using standard C++ and system-level socket APIs.

## Technical Specifications

- **Language**: C++11 or higher
- **Protocol**: TCP/IP
- **API**: POSIX Sockets
- **Threading**: C++ Standard Thread Library
- **OS Support**: Linux, macOS, and other Unix-based systems

## Installation and Build

### Prerequisites

Ensure you have the following installed on your system:
- GNU Compiler Collection (g++)
- Make build automation tool

### Cloning the Repository

```bash
git clone https://github.com/suadatbiniqbal/TerminalTalk-TCP-Server.git
cd TerminalTalk-TCP-Server
```

### Compiling the Project

The project includes a comprehensive Makefile for easy compilation.

```bash
make all
```

This will generate two executable files: `server` and `client`.

## Usage

### 1. Start the Server

First, initiate the server to listen for incoming connections:

```bash
./server
```

The server will start listening on port 5555 by default.

### 2. Connect Clients

In separate terminal windows, start as many clients as needed:

```bash
./client
```

Upon starting, each client will:
1. Display a startup banner.
- Undergo a brief connection initialization animation.
2. Prompt for a username.
3. Establish a connection with the server.

### 3. Communication

Once connected, simply type your message and press Enter to broadcast it to all other users. To disconnect, type `/quit` or `/exit`.

## Project Structure

- `server.cpp`: Implementation of the multi-threaded TCP server.
- `client.cpp`: Implementation of the TCP client with an interactive interface.
- `Makefile`: Build instructions for the project.
- `README.md`: Project documentation.

## License

This project is open-source and available under the MIT License.
