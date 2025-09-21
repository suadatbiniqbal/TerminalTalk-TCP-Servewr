/*
 * TCP World Chat Client
 * Connects to the chat server and allows sending/receiving messages
 * Compile: g++ -std=c++17 -pthread -o client client.cpp
 * Run: ./client <server_ip>
 * Example: ./client 192.168.1.100
 */
// follow mw on instagram @suadatbiniqbal
//star the repo
//github https://github.com/suadatbiniqbal

#include <iostream>
#include <thread>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <signal.h>

class ChatClient {
private:
    int client_socket;
    std::string server_ip;
    int server_port;
    bool connected;
    std::thread receive_thread;

public:
    ChatClient(const std::string& ip, int port = 12345) 
        : server_ip(ip), server_port(port), connected(false) {
        // Ignore SIGPIPE
        signal(SIGPIPE, SIG_IGN);
    }

    ~ChatClient() {
        disconnect();
    }

    bool connect_to_server() {
        // Create socket
        client_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (client_socket < 0) {
            std::cerr << "Error creating socket" << std::endl;
            return false;
        }

        // Setup server address
        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(server_port);

        if (inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr) <= 0) {
            std::cerr << "Invalid server IP address: " << server_ip << std::endl;
            close(client_socket);
            return false;
        }

        // Connect to server
        if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            std::cerr << "Failed to connect to server " << server_ip << ":" << server_port << std::endl;
            std::cerr << "Make sure the server is running and reachable." << std::endl;
            close(client_socket);
            return false;
        }

        connected = true;
        std::cout << "=== TCP World Chat Client ===" << std::endl;
        std::cout << "Connected to server: " << server_ip << ":" << server_port << std::endl;
        std::cout << "Type messages and press Enter to send" << std::endl;
        std::cout << "Type 'quit' or press Ctrl+C to exit" << std::endl;
        std::cout << "=============================" << std::endl;

        // Start receiving thread
        receive_thread = std::thread(&ChatClient::receive_messages, this);

        return true;
    }

    void receive_messages() {
        char buffer[1024];

        while (connected) {
            int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);

            if (bytes_received <= 0) {
                if (connected) {
                    std::cout << "\n[CLIENT] Connection to server lost." << std::endl;
                    connected = false;
                }
                break;
            }

            buffer[bytes_received] = '\0';
            std::string message(buffer);

            // Remove newline if present
            if (!message.empty() && message.back() == '\n') {
                message.pop_back();
            }

            if (!message.empty()) {
                std::cout << message << std::endl;
            }
        }
    }

    void send_messages() {
        std::string message;

        while (connected) {
            std::getline(std::cin, message);

            if (!connected) break;

            if (message == "quit" || message == "exit") {
                break;
            }

            if (!message.empty()) {
                message += "\n";
                int sent = send(client_socket, message.c_str(), message.length(), 0);

                if (sent < 0) {
                    std::cout << "Failed to send message. Connection may be lost." << std::endl;
                    break;
                }
            }
        }

        connected = false;
    }

    void disconnect() {
        if (!connected) return;

        connected = false;

        if (client_socket >= 0) {
            close(client_socket);
        }

        if (receive_thread.joinable()) {
            receive_thread.join();
        }

        std::cout << "[CLIENT] Disconnected from server." << std::endl;
    }

    void run() {
        send_messages();
        disconnect();
    }
};

// Global client for signal handler
ChatClient* global_client = nullptr;

void signal_handler(int signal) {
    if (global_client) {
        global_client->disconnect();
    }
    exit(0);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <server_ip>" << std::endl;
        std::cout << "Example: " << argv[0] << " 192.168.1.100" << std::endl;
        std::cout << "Example: " << argv[0] << " 127.0.0.1 (for localhost)" << std::endl;
        return 1;
    }

    // Setup signal handlers
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    std::string server_ip = argv[1];
    ChatClient client(server_ip);
    global_client = &client;

    if (!client.connect_to_server()) {
        return 1;
    }

    client.run();
    return 0;
}