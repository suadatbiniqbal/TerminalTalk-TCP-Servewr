/*
 * TCP World Chat Client
 * Connects to the chat server and allows sending/receiving messages
 * Compile: g++ -std=c++17 -pthread -o client client.cpp
 * Run: ./client <server_ip>
 * Example: ./client 192.168.1.100
 */
//star the repo
//github https://github.com/suadatbiniqbal
#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 5555
#define BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"

std::atomic<bool> running(true);
int client_socket;
std::string username;

void receive_messages() {
    char buffer[BUFFER_SIZE];
    
    while (running) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        
        if (bytes_received <= 0) {
            if (running) {
                std::cout << "\n[ERROR] Connection lost" << std::endl;
            }
            running = false;
            break;
        }
        
        std::string message(buffer, bytes_received);
        
        if (message == "USERNAME") {
            // Server requesting username
            send(client_socket, username.c_str(), username.length(), 0);
        } else {
            // Display received message
            std::cout << "\r" << message;
            std::cout.flush();
            if (!message.empty() && message.back() != '\n') {
                std::cout << std::endl;
            }
            std::cout << username << ": ";
            std::cout.flush();
        }
    }
}

void send_messages() {
    std::string message;
    
    while (running) {
        std::cout << username << ": ";
        std::getline(std::cin, message);
        
        if (!running) break;
        
        if (message == "/quit" || message == "/exit") {
            std::cout << "[CLIENT] Disconnecting..." << std::endl;
            running = false;
            break;
        }
        
        if (!message.empty()) {
            message += "\n";
            int bytes_sent = send(client_socket, message.c_str(), message.length(), 0);
            
            if (bytes_sent < 0) {
                std::cout << "[ERROR] Could not send message" << std::endl;
                running = false;
                break;
            }
        }
    }
}

int main() {
    // Get username from user
    std::cout << "Enter your username: ";
    std::getline(std::cin, username);
    
    while (username.empty() || username.length() < 2) {
        std::cout << "[ERROR] Username must be at least 2 characters long" << std::endl;
        std::cout << "Enter your username: ";
        std::getline(std::cin, username);
    }
    
    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    if (client_socket == -1) {
        std::cerr << "[ERROR] Could not create socket" << std::endl;
        return 1;
    }
    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        std::cerr << "[ERROR] Invalid address" << std::endl;
        close(client_socket);
        return 1;
    }
    
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "[ERROR] Connection failed. Make sure the server is running." << std::endl;
        close(client_socket);
        return 1;
    }
    
    std::cout << "[CLIENT] Connected to server at " << SERVER_IP << ":" << PORT << std::endl;
    std::cout << "[CLIENT] Type /quit or /exit to disconnect\n" << std::endl;
    
    // Start threads
    std::thread receive_thread(receive_messages);
    std::thread send_thread(send_messages);
    
    // Wait for threads
    send_thread.join();
    running = false;
    
    shutdown(client_socket, SHUT_RDWR);
    close(client_socket);
    
    receive_thread.join();
    
    std::cout << "[CLIENT] Disconnected" << std::endl;
    
    return 0;
}
