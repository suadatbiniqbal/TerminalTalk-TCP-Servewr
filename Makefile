# Makefile for TCP World Chat
# Usage: make all (builds both server and client)
#        make server (builds only server)
#        make client (builds only client)
#        make clean (removes executables)
# // follow mw on instagram @suadatbiniqbal
# //star the repo
# //github https://github.com/suadatbiniqbal


CXX = g++
CXXFLAGS = -std=c++17 -pthread -Wall -Wextra
SERVER_TARGET = server
CLIENT_TARGET = client
SERVER_SRC = server.cpp
CLIENT_SRC = client.cpp

# Default target
all: $(SERVER_TARGET) $(CLIENT_TARGET)

# Build server
$(SERVER_TARGET): $(SERVER_SRC)
	$(CXX) $(CXXFLAGS) -o $(SERVER_TARGET) $(SERVER_SRC)
	@echo "Server compiled successfully!"

# Build client
$(CLIENT_TARGET): $(CLIENT_SRC)
	$(CXX) $(CXXFLAGS) -o $(CLIENT_TARGET) $(CLIENT_SRC)
	@echo "Client compiled successfully!"

# Clean executables
clean:
	rm -f $(SERVER_TARGET) $(CLIENT_TARGET)
	@echo "Cleaned executables!"

# Install dependencies (for Ubuntu/Debian)
install-deps:
	sudo apt-get update
	sudo apt-get install -y build-essential g++

# Help
help:
	@echo "Available targets:"
	@echo "  all          - Build both server and client"
	@echo "  server       - Build only the server"
	@echo "  client       - Build only the client"
	@echo "  clean        - Remove executables"
	@echo "  install-deps - Install required dependencies"
	@echo "  help         - Show this help"

.PHONY: all clean install-deps help