# TerminalTalk TCP Server 💬

<div align="center">

![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Socket](https://img.shields.io/badge/Socket-Programming-orange?style=for-the-badge)
![TCP](https://img.shields.io/badge/TCP-Protocol-blue?style=for-the-badge)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![Make](https://img.shields.io/badge/Make-Build_System-green?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge)

**A lightweight, multi-threaded terminal chat application built with C++ and TCP sockets**

[Features](#-features) • [Quick Start](#-quick-start) • [Installation](#-installation) • [Usage](#-usage) • [Documentation](#-documentation)

![Demo](https://img.shields.io/badge/Status-Active-success?style=flat-square)
![Version](https://img.shields.io/badge/Version-1.0.0-blue?style=flat-square)

</div>

---

## 🎯 About

**TerminalTalk** is a real-time, multi-client chat application that runs entirely in your terminal. Built from scratch using C++ and POSIX sockets, it demonstrates advanced networking concepts including multi-threading, socket programming, and concurrent client handling[web:11][web:20].

### 🚀 Why TerminalTalk?

- ⚡ **Blazing Fast** - Pure C++ implementation with minimal overhead
- 🧵 **Multi-Threaded** - Handle unlimited clients simultaneously
- 🔒 **Thread-Safe** - Mutex-protected shared resources
- 📦 **Zero Dependencies** - Only standard C++ libraries
- 🎓 **Educational** - Clean, well-documented code for learning
- 🛠️ **Easy Build** - One command compilation with Makefile

---

## ✨ Features

### Core Functionality
- ✅ **Real-Time Messaging** - Instant message delivery via TCP
- ✅ **Multi-Client Support** - Connect unlimited users simultaneously
- ✅ **Username System** - Unique usernames for each participant
- ✅ **Broadcast Messages** - All messages sent to every connected client
- ✅ **Join/Leave Notifications** - Server announces user activity
- ✅ **Graceful Disconnect** - Clean exit with `/quit` or `/exit` commands
- ✅ **Automatic Cleanup** - Handles disconnections and crashes elegantly

### Technical Features
- Multi-threaded server architecture
- POSIX socket programming
- Mutex-based synchronization
- Thread-safe client management
- Non-blocking I/O operations
- Automatic port reuse

---

## ⚡ Quick Start

NOTE: Final Update From Me!

Clone the repository

git clone https://github.com/suadatbiniqbal/TerminalTalk-TCP-Servewr.git
cd TerminalTalk-TCP-Servewr
Build everything

make
Terminal 1: Start server

./server
Terminal 2: Connect first client

./client
Terminal 3: Connect second client

./client

text

That's it! Start chatting 🎉

---

## 🔧 Prerequisites

- **OS**: Linux, macOS, or WSL2
- **Compiler**: GCC/G++ 4.8.1+ with C++11 support
- **Build Tool**: Make (optional but recommended)

### Verify Installation

g++ --version # Should show 4.8.1 or higher
make --version # Check if Make is installed

text

---

## 📥 Installation

### Method 1: Using Makefile (Recommended)

Clone repository

git clone https://github.com/suadatbiniqbal/TerminalTalk-TCP-Servewr.git
cd TerminalTalk-TCP-Servewr
Build both server and client

make

text

### Method 2: Manual Compilation

Compile server

g++ -std=c++11 -pthread server.cpp -o server
Compile client

g++ -std=c++11 -pthread client.cpp -o client

text

### Method 3: System-Wide Installation

Build and install to /usr/local/bin

make install
Now run from anywhere

server # Start server
client # Connect client

text

---

## 🚀 Usage

### Starting the Server

make run-server
OR

./server

text

**Output:**

[SERVER] Server is listening on port 5555
[SERVER] Waiting for connections...

text

### Connecting Clients

**Terminal 2:**

make run-client
OR

./client

text

**You'll see:**

Enter your username: Alice
[CLIENT] Connected to server at 127.0.0.1:5555
[CLIENT] Type /quit or /exit to disconnect

[SERVER] Welcome Alice! You are now connected.
Alice:

text

### Example Chat Session

**Alice:**

Alice: Hey everyone! 👋
[SERVER] Bob has joined the chat!
Bob: Hi Alice! How's it going?
Alice: Great! Welcome Bob!

text

**Bob:**

Enter your username: Bob
[SERVER] Welcome Bob! You are now connected.
Alice: Hey everyone! 👋
Bob: Hi Alice! How's it going?
Alice: Great! Welcome Bob!

text

---

## 📝 Makefile Commands

| Command | Description |
|---------|-------------|
| `make` | Build both server and client |
| `make all` | Same as `make` |
| `make server` | Build server only |
| `make client` | Build client only |
| `make clean` | Remove all build files |
| `make rebuild` | Clean and rebuild everything |
| `make run-server` | Build and run server |
| `make run-client` | Build and run client |
| `make install` | Install system-wide (requires sudo) |
| `make uninstall` | Remove system installation |
| `make help` | Show all available commands |

---

## 🎮 Chat Commands

| Command | Description | Example |
|---------|-------------|---------|
| `/quit` | Disconnect from server | `/quit` |
| `/exit` | Disconnect (alternative) | `/exit` |
| `Ctrl+C` | Force disconnect | - |

---

## 🏗️ Architecture

### System Design

text
    ┌─────────────────────┐
    │   TCP Server        │
    │   Port: 5555        │
    │   Multi-threaded    │
    └──────────┬──────────┘
               │
   ┌───────────┼───────────┬───────────┐
   │           │           │           │

┌───▼───┐ ┌──▼───┐ ┌───▼───┐ ┌──▼───┐
│Client1│ │Client2│ │Client3│ │Client4│
│ Alice │ │ Bob │ │ Charlie│ │ David│
└───────┘ └───────┘ └────────┘ └───────┘

text

### Data Flow

1. **Connection**: Client connects to server (port 5555)
2. **Authentication**: Server requests username
3. **Registration**: Client sends username, added to active list
4. **Broadcasting**: Server announces new user to all clients
5. **Messaging**: User sends message → Server broadcasts to all
6. **Disconnection**: User leaves → Server notifies everyone

### File Structure

TerminalTalk-TCP-Servewr/
├── server.cpp # Server implementation
├── client.cpp # Client implementation
├── Makefile # Build automation
├── README.md # This file
└── LICENSE # MIT License

text

---

## ⚙️ Configuration

### Change Server Port

Edit `server.cpp` and `client.cpp`:

#define PORT 8080 // Change to your preferred port

text

### Change Server IP (Remote Connection)

Edit `client.cpp`:

#define SERVER_IP "192.168.1.100" // Your server's IP address

text

### Adjust Maximum Clients

Edit `server.cpp`:

#define MAX_CLIENTS 50 // Increase for more users

text

### Adjust Buffer Size

Edit both files:

#define BUFFER_SIZE 2048 // For longer messages

text

Then rebuild:

make rebuild

text

---

## 🔍 Troubleshooting

### ❌ Connection Refused

**Problem:** `[ERROR] Connection failed. Make sure the server is running.`

**Solution:**

Ensure server is running first

./server
Check if port is available

netstat -tuln | grep 5555

text

### ❌ Port Already in Use

**Problem:** `[ERROR] Bind failed`

**Solution:**

Find process using port 5555

lsof -i :5555
Kill the process

kill -9 <PID>
Or use make to rebuild

make rebuild

text

### ❌ Compilation Errors

**Problem:** `error: 'thread' is not a member of 'std'`

**Solution:**

Ensure C++11 support

make clean
make

text

### ❌ Username Too Short

**Problem:** `[ERROR] Username must be at least 2 characters long`

**Solution:** Enter a username with 2+ characters

### ❌ Permission Denied (Install)

**Problem:** Permission errors during `make install`

**Solution:**

sudo make install

text

---

## 🤝 Contributing

Contributions make the open-source community an amazing place to learn and create. Any contributions are **greatly appreciated**!

### How to Contribute

1. **Fork** the project
2. **Create** your feature branch (`git checkout -b feature/AmazingFeature`)
3. **Commit** your changes (`git commit -m 'Add AmazingFeature'`)
4. **Push** to the branch (`git push origin feature/AmazingFeature`)
5. **Open** a Pull Request

### Feature Ideas 💡

- [ ] Private messaging (`/msg username message`)
- [ ] Chat rooms/channels
- [ ] Message encryption (TLS/SSL)
- [ ] File transfer support
- [ ] Message history/logging
- [ ] User authentication system
- [ ] Admin commands (kick, ban, mute)
- [ ] Emoji support
- [ ] Message timestamps
- [ ] Color-coded messages
- [ ] GUI version (Qt/GTK)
- [ ] Windows support (Winsock)

---

## 📚 Documentation



**Server Functions:**
- `broadcast_message()` - Send message to all clients except sender
- `handle_client()` - Manage individual client connections
- `remove_client()` - Clean up disconnected clients

**Client Functions:**
- `receive_messages()` - Listen for incoming messages (thread)
- `send_messages()` - Handle user input and send (thread)
- `connect_to_server()` - Establish connection with server

### Learning Resources

- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)
- [POSIX Threads Programming](https://hpc-tutorials.llnl.gov/posix/)
- [C++ Socket Programming Tutorial](https://www.geeksforgeeks.org/socket-programming-cc/)

---

## 📄 License
Free Open Source 

---

## 👤 Author

**Suadat Bin Iqbal**

- GitHub: [@suadatbiniqbal](https://github.com/suadatbiniqbal)
- Project: [TerminalTalk-TCP-Servewr](https://github.com/suadatbiniqbal/TerminalTalk-TCP-Servewr)

---

## 🙏 Acknowledgments

- [GeeksforGeeks](https://www.geeksforgeeks.org/) - Socket programming tutorials
- [Beej's Guide](https://beej.us/guide/bgnet/) - Network programming guide
- [POSIX Threads](https://hpc-tutorials.llnl.gov/posix/) - Threading tutorials
- [Shields.io](https://shields.io/) - README badges
- [Best-README-Template](https://github.com/othneildrew/Best-README-Template) - README inspiration

---

## 📊 Project Stats

![GitHub repo size](https://img.shields.io/github/repo-size/suadatbiniqbal/TerminalTalk-TCP-Servewr?style=flat-square)
![GitHub stars](https://img.shields.io/github/stars/suadatbiniqbal/TerminalTalk-TCP-Servewr?style=flat-square)
![GitHub forks](https://img.shields.io/github/forks/suadatbiniqbal/TerminalTalk-TCP-Servewr?style=flat-square)
![GitHub issues](https://img.shields.io/github/issues/suadatbiniqbal/TerminalTalk-TCP-Servewr?style=flat-square)

---

<div align="center">

**Made with ❤️ and C++**

⭐ **Star this repo if you found it helpful!** ⭐

[Report Bug](https://github.com/suadatbiniqbal/TerminalTalk-TCP-Servewr/issues) • [Request Feature](https://github.com/suadatbiniqbal/TerminalTalk-TCP-Servewr/issues)

</div>