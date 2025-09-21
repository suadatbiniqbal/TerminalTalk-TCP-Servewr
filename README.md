# TCP World Chat

A simple multi-threaded TCP-based world chat system written in C++. The server runs on your PC and accepts multiple clients that can connect from any device on the same network (including Termux on Android).

## Features

- **Multi-threaded TCP server** - Handles multiple clients simultaneously
- **Real-time messaging** - Messages are broadcast to all connected clients instantly
- **Cross-platform** - Works on Linux, macOS, and Android (Termux)
- **Network-wide** - Clients can connect from any device on the same network
- **Simple and lightweight** - Uses only standard C++ libraries
- **Graceful shutdown** - Handles Ctrl+C and client disconnections properly

## Files

- `server.cpp` - Multi-threaded TCP server code
- `client.cpp` - Terminal-based client code
- `Makefile` - Build configuration
- `README.md` - This documentation

## Quick Start

### 1. Installation

#### On Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install build-essential g++
```

#### On Termux (Android):
```bash
pkg update
pkg install clang make
```

### 2. Compilation

```bash
# Clone or download the project files
# Navigate to the project directory

# Build both server and client
make all

# Or build individually
make server
make client
```

### 3. Running the Server

```bash
# Start the server (runs on port 12345 by default)
./server
```

The server will display:
```
TCP World Chat Server
Server started on port 12345
Waiting for clients to connect...
Press Ctrl+C to stop the server

```

### 4. Running the Client

#### From the same PC (localhost):
```bash
./client 127.0.0.1
```

#### From another PC on the same network:
```bash
# Replace with your server PC's IP address
./client 192.168.1.100
```

#### From Termux (Android):
```bash
# First, compile the client on Termux
g++ -std=c++17 -pthread -o client client.cpp

# Then connect to your PC's IP
./client 192.168.1.100
```

## Finding Your PC's IP Address

### On Linux:
```bash
ip addr show | grep "inet " | grep -v 127.0.0.1
```

### On Windows:
```cmd
ipconfig | findstr IPv4
```

### On macOS:
```bash
ifconfig | grep "inet " | grep -v 127.0.0.1
```

## Usage Examples

### Starting a Chat Session

1. **Start the server** on your PC:
   ```bash
   ./server
   ```

2. **Connect clients** from various devices:
   ```bash
   # From another PC
   ./client 192.168.1.100

   # From Termux
   ./client 192.168.1.100
   ```

3. **Start chatting!** Type messages and press Enter. All connected clients will see the messages.

### Example Chat Session

**Server Output:**
```
=== TCP World Chat Server ===
Server started on port 12345
Waiting for clients to connect...
Press Ctrl+C to stop the server
=============================
[SERVER] New client connected from 192.168.1.101 (Total clients: 1)
[192.168.1.101] Hello everyone!
[SERVER] New client connected from 192.168.1.102 (Total clients: 2)
[192.168.1.102] Hi there!
[192.168.1.101] How's everyone doing?
```

**Client Output:**
```
=== TCP World Chat Client ===
Connected to server: 192.168.1.100:12345
Type messages and press Enter to send
Type 'quit' or press Ctrl+C to exit
=============================
Hello everyone!
[192.168.1.102] Hi there!
How's everyone doing?
[192.168.1.102] Good! Nice to meet you.
```

## Network Configuration

### Local Network (Wi-Fi)
- Server and clients must be on the same Wi-Fi network
- Use the server PC's local IP address (usually 192.168.x.x)
- No additional configuration needed

### Internet Access (Advanced)
To allow connections from outside your local network:

1. **Port Forwarding**: Configure your router to forward port 12345 to your PC
2. **Firewall**: Allow port 12345 through your PC's firewall
3. **Use Public IP**: Clients connect using your public IP address

**Example firewall commands (Ubuntu):**
```bash
sudo ufw allow 12345
sudo ufw reload
```

## Troubleshooting

### Common Issues

1. **"Connection refused"**
   - Make sure the server is running
   - Check that you're using the correct IP address
   - Verify firewall settings

2. **"Address already in use"**
   - Wait a few seconds and try again
   - Kill any existing server processes: `pkill server`

3. **Can't find IP address**
   - Use `127.0.0.1` for local testing
   - Check network connection
   - Try `hostname -I` on Linux

### Testing Locally

Always test locally first:
```bash
# Terminal 1: Start server
./server

# Terminal 2: Start client
./client 127.0.0.1
```

## Advanced Usage

### Custom Port
To use a different port, modify the port number in both `server.cpp` and `client.cpp`, then recompile.

### Multiple Servers
Run multiple servers on different ports:
```bash
# Modify code to use port 12346, then:
./server2
./client 127.0.0.1  # Will need to be modified to connect to 12346
```

## Code Structure

### Server (`server.cpp`)
- **Main thread**: Accepts new client connections
- **Client threads**: Handle individual client communication (one per client)
- **Broadcast system**: Forwards messages to all connected clients
- **Thread-safe**: Uses mutexes to protect shared data

### Client (`client.cpp`)
- **Main thread**: Handles user input and sending messages
- **Receive thread**: Listens for incoming messages from server
- **Simple interface**: Text-based terminal interaction

## Building from Source

### Manual Compilation
```bash
# Server
g++ -std=c++17 -pthread -o server server.cpp

# Client
g++ -std=c++17 -pthread -o client client.cpp
```

### Requirements
- C++17 compatible compiler (GCC 7+ or Clang 5+)
- POSIX-compliant system (Linux, macOS, Android/Termux)
- pthread library support

## License

This project is provided as-is for educational and personal use.

## Contributing

Feel free to modify and improve the code for your specific needs. Some potential enhancements:
- User nicknames/authentication
- Private messaging
- Message history
- GUI interface
- Encryption
- File transfer capabilities