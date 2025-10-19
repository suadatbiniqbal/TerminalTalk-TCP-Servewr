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
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5555
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

struct Client {
    int socket;
    std::string username;
    std::thread thread;
};

std::vector<Client*> clients;
std::mutex clients_mutex;

void broadcast_message(const std::string& message, int sender_socket) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    for (auto client : clients) {
        if (client->socket != sender_socket) {
            send(client->socket, message.c_str(), message.length(), 0);
        }
    }
}

void remove_client(int socket) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    auto it = std::find_if(clients.begin(), clients.end(),
        [socket](Client* c) { return c->socket == socket; });
    
    if (it != clients.end()) {
        Client* client = *it;
        std::string leave_msg = "[SERVER] " + client->username + " has left the chat.\n";
        std::cout << client->username << " disconnected." << std::endl;
        
        clients.erase(it);
        close(client->socket);
        
        // Broadcast after removing client
        for (auto c : clients) {
            send(c->socket, leave_msg.c_str(), leave_msg.length(), 0);
        }
        
        delete client;
    }
}

void handle_client(Client* client) {
    char buffer[BUFFER_SIZE];
    
    // Request username
    std::string username_prompt = "USERNAME";
    send(client->socket, username_prompt.c_str(), username_prompt.length(), 0);
    
    // Receive username
    memset(buffer, 0, BUFFER_SIZE);
    int bytes_received = recv(client->socket, buffer, BUFFER_SIZE, 0);
    
    if (bytes_received <= 0) {
        remove_client(client->socket);
        return;
    }
    
    client->username = std::string(buffer, bytes_received);
    
    // Welcome message to new user
    std::string welcome = "[SERVER] Welcome " + client->username + "! You are now connected.\n";
    send(client->socket, welcome.c_str(), welcome.length(), 0);
    
    // Notify all other clients
    std::string join_msg = "[SERVER] " + client->username + " has joined the chat!\n";
    std::cout << client->username << " connected." << std::endl;
    broadcast_message(join_msg, client->socket);
    
    // Handle messages
    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        bytes_received = recv(client->socket, buffer, BUFFER_SIZE, 0);
        
        if (bytes_received <= 0) {
            break;
        }
        
        std::string message = client->username + ": " + std::string(buffer, bytes_received);
        std::cout << message;
        broadcast_message(message, client->socket);
    }
    
    remove_client(client->socket);
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    if (server_socket == -1) {
        std::cerr << "[ERROR] Could not create socket" << std::endl;
        return 1;
    }
    
    // Allow socket reuse
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "[ERROR] Bind failed" << std::endl;
        close(server_socket);
        return 1;
    }
    
    if (listen(server_socket, MAX_CLIENTS) < 0) {
        std::cerr << "[ERROR] Listen failed" << std::endl;
        close(server_socket);
        return 1;
    }
    
    std::cout << "[SERVER] Server is listening on port " << PORT << std::endl;
    std::cout << "[SERVER] Waiting for connections..." << std::endl;
    
    while (true) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        
        int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        
        if (client_socket < 0) {
            std::cerr << "[ERROR] Accept failed" << std::endl;
            continue;
        }
        
        Client* new_client = new Client();
        new_client->socket = client_socket;
        
        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            clients.push_back(new_client);
        }
        
        new_client->thread = std::thread(handle_client, new_client);
        new_client->thread.detach();
        
        std::cout << "[SERVER] Active connections: " << clients.size() << std::endl;
    }
    
    close(server_socket);
    return 0;
}
