/*
 * TerminalTalk TCP Client
 * Connects to the chat server and allows sending/receiving messages
 * Compile: g++ -std=c++17 -pthread -o client client.cpp
 * Run: ./client
 */

#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <chrono>

#define PORT 5555
#define BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"

// ANSI Color Codes
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

std::atomic<bool> running(true);
int client_socket;
std::string username;

void show_banner() {
    std::cout << CYAN << BOLD;
    std::cout << "===========================================" << std::endl;
    std::cout << "         TERMINAL TALK CLIENT              " << std::endl;
    std::cout << "===========================================" << RESET << std::endl;
}

void loading_animation(const std::string& message) {
    const char spinner[] = {'|', '/', '-', '\\'};
    for (int i = 0; i < 10; ++i) {
        std::cout << "\r" << YELLOW << message << " " << spinner[i % 4] << RESET << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "\r" << GREEN << message << " [OK]     " << RESET << std::endl;
}

void receive_messages() {
    char buffer[BUFFER_SIZE];
    
    while (running) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        
        if (bytes_received <= 0) {
            if (running) {
                std::cout << "\n" << RED << "[ERROR] Connection lost" << RESET << std::endl;
            }
            running = false;
            break;
        }
        
        std::string message(buffer, bytes_received);
        
        if (message == "USERNAME") {
            send(client_socket, username.c_str(), username.length(), 0);
        } else {
            // Clear current line and display received message
            std::cout << "\r\033[2K" << message;
            if (!message.empty() && message.back() != '\n') {
                std::cout << std::endl;
            }
            std::cout << CYAN << username << ": " << RESET;
            std::cout.flush();
        }
    }
}

void send_messages() {
    std::string message;
    
    while (running) {
        std::cout << CYAN << username << ": " << RESET;
        if (!std::getline(std::cin, message)) break;
        
        if (!running) break;
        
        if (message == "/quit" || message == "/exit") {
            std::cout << YELLOW << "[CLIENT] Disconnecting..." << RESET << std::endl;
            running = false;
            break;
        }
        
        if (!message.empty()) {
            message += "\n";
            int bytes_sent = send(client_socket, message.c_str(), message.length(), 0);
            
            if (bytes_sent < 0) {
                std::cout << RED << "[ERROR] Could not send message" << RESET << std::endl;
                running = false;
                break;
            }
        }
    }
}

int main() {
    show_banner();

    std::cout << "Enter your username: ";
    std::getline(std::cin, username);
    
    while (username.empty() || username.length() < 2) {
        std::cout << RED << "[ERROR] Username must be at least 2 characters long" << RESET << std::endl;
        std::cout << "Enter your username: ";
        std::getline(std::cin, username);
    }
    
    loading_animation("Initializing socket");
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    if (client_socket == -1) {
        std::cerr << RED << "[ERROR] Could not create socket" << RESET << std::endl;
        return 1;
    }
    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        std::cerr << RED << "[ERROR] Invalid address" << RESET << std::endl;
        close(client_socket);
        return 1;
    }
    
    loading_animation("Connecting to server");
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << RED << "[ERROR] Connection failed. Make sure the server is running." << RESET << std::endl;
        close(client_socket);
        return 1;
    }
    
    std::cout << GREEN << "[SUCCESS] Connected to server at " << SERVER_IP << ":" << PORT << RESET << std::endl;
    std::cout << YELLOW << "[INFO] Type /quit or /exit to disconnect\n" << RESET << std::endl;
    
    std::thread receive_thread(receive_messages);
    std::thread send_thread(send_messages);
    
    send_thread.join();
    running = false;
    
    shutdown(client_socket, SHUT_RDWR);
    close(client_socket);
    
    if (receive_thread.joinable()) {
        receive_thread.join();
    }
    
    std::cout << YELLOW << "[CLIENT] Disconnected" << RESET << std::endl;
    
    return 0;
}
