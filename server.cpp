/*
 * TCP World Chat Server
 * Multi-threaded server that accepts multiple clients and broadcasts messages
 * Compile: g++ -std=c++17 -pthread -o server server.cpp
 * Run: ./server
 */
// follow mw on instagram @suadatbiniqbal
//star the repo
//github https://github.com/suadatbiniqbal

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <string>
#include <algorithm>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <signal.h>

class ChatServer {
private:
    int server_socket;
    int port;
    std::vector<int> client_sockets;
    std::mutex clients_mutex;
    bool running;

public:
    ChatServer(int port = 12345) : port(port), running(false) {
        // Ignore SIGPIPE to prevent crashes when clients disconnect
        signal(SIGPIPE, SIG_IGN);
    }

    ~ChatServer() {
        stop();
    }

    bool start() {
        // Create socket
        server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket < 0) {
            std::cerr << "Error creating socket" << std::endl;
            return false;
        }

        // Set socket options to reuse address
        int opt = 1;
        if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            std::cerr << "Error setting socket options" << std::endl;
            close(server_socket);
            return false;
        }

        // Setup server address
        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(port);

        // Bind socket
        if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            std::cerr << "Error binding socket to port " << port << std::endl;
            close(server_socket);
            return false;
        }

        // Listen for connections
        if (listen(server_socket, 10) < 0) {
            std::cerr << "Error listening on socket" << std::endl;
            close(server_socket);
            return false;
        }

        running = true;
        std::cout << "=== TCP World Chat Server ===" << std::endl;
        std::cout << "Server started on port " << port << std::endl;
        std::cout << "Waiting for clients to connect..." << std::endl;
        std::cout << "Press Ctrl+C to stop the server" << std::endl;
        std::cout << "=============================" << std::endl;

        return true;
    }

    void run() {
        if (!running) return;

        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        while (running) {
            // Accept new client connection
            int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);

            if (client_socket < 0) {
                if (running) {
                    std::cerr << "Error accepting client connection" << std::endl;
                }
                continue;
            }

            // Get client IP address
            char client_ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);

            // Add client to list
            {
                std::lock_guard<std::mutex> lock(clients_mutex);
                client_sockets.push_back(client_socket);
            }

            std::cout << "[SERVER] New client connected from " << client_ip 
                     << " (Total clients: " << client_sockets.size() << ")" << std::endl;

            // Create thread to handle this client
            std::thread client_thread(&ChatServer::handle_client, this, client_socket, std::string(client_ip));
            client_thread.detach();
        }
    }

    void handle_client(int client_socket, std::string client_ip) {
        char buffer[1024];

        while (running) {
            // Receive message from client
            int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);

            if (bytes_received <= 0) {
                // Client disconnected
                break;
            }

            buffer[bytes_received] = '\0';
            std::string message(buffer);

            // Remove newline if present
            if (!message.empty() && message.back() == '\n') {
                message.pop_back();
            }

            if (!message.empty()) {
                // Print message on server
                std::cout << "[" << client_ip << "] " << message << std::endl;

                // Broadcast message to all clients
                broadcast_message("[" + client_ip + "] " + message, client_socket);
            }
        }

        // Remove client from list and close socket
        remove_client(client_socket);
        std::cout << "[SERVER] Client " << client_ip << " disconnected (Total clients: " 
                 << client_sockets.size() << ")" << std::endl;
    }

    void broadcast_message(const std::string& message, int sender_socket) {
        std::lock_guard<std::mutex> lock(clients_mutex);

        std::string full_message = message + "\n";

        // Send to all clients except the sender
        for (auto it = client_sockets.begin(); it != client_sockets.end();) {
            if (*it != sender_socket) {
                int sent = send(*it, full_message.c_str(), full_message.length(), 0);
                if (sent < 0) {
                    // Client disconnected, remove from list
                    close(*it);
                    it = client_sockets.erase(it);
                    continue;
                }
            }
            ++it;
        }
    }

    void remove_client(int client_socket) {
        std::lock_guard<std::mutex> lock(clients_mutex);

        auto it = std::find(client_sockets.begin(), client_sockets.end(), client_socket);
        if (it != client_sockets.end()) {
            close(*it);
            client_sockets.erase(it);
        }
    }

    void stop() {
        if (!running) return;

        running = false;

        // Close all client sockets
        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            for (int socket : client_sockets) {
                close(socket);
            }
            client_sockets.clear();
        }

        // Close server socket
        if (server_socket >= 0) {
            close(server_socket);
        }

        std::cout << "\n[SERVER] Server stopped." << std::endl;
    }
};

// Signal handler for graceful shutdown
ChatServer* global_server = nullptr;

void signal_handler(int signal) {
    if (global_server) {
        global_server->stop();
    }
    exit(0);
}

int main() {
    // Setup signal handlers for graceful shutdown
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    ChatServer server(12345);
    global_server = &server;

    if (!server.start()) {
        return 1;
    }

    server.run();
    return 0;
}