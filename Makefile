# Makefile for TCP World Chat
# Usage: make all (builds both server and client)
#        make server (builds only server)
#        make client (builds only client)
#        make clean (removes executables)
# //star the repo
# //github https://github.com/suadatbiniqbal

# Compiler settings
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pthread
LDFLAGS = -pthread

# Project settings
SERVER_TARGET = server
CLIENT_TARGET = client
SERVER_SRC = server.cpp
CLIENT_SRC = client.cpp

# Build directory (optional)
BUILD_DIR = build

# Default target - builds both server and client
all: $(SERVER_TARGET) $(CLIENT_TARGET)
	@echo "==================================="
	@echo "Build completed successfully!"
	@echo "Run './server' to start the server"
	@echo "Run './client' to connect a client"
	@echo "==================================="

# Build server
$(SERVER_TARGET): $(SERVER_SRC)
	@echo "Compiling server..."
	$(CXX) $(CXXFLAGS) -o $(SERVER_TARGET) $(SERVER_SRC) $(LDFLAGS)
	@echo "Server compiled successfully!"

# Build client
$(CLIENT_TARGET): $(CLIENT_SRC)
	@echo "Compiling client..."
	$(CXX) $(CXXFLAGS) -o $(CLIENT_TARGET) $(CLIENT_SRC) $(LDFLAGS)
	@echo "Client compiled successfully!"

# Build only server
server: $(SERVER_TARGET)

# Build only client
client: $(CLIENT_TARGET)

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -f $(SERVER_TARGET) $(CLIENT_TARGET)
	rm -f *.o
	@echo "Clean completed!"

# Run server
run-server: $(SERVER_TARGET)
	@echo "Starting server..."
	./$(SERVER_TARGET)

# Run client
run-client: $(CLIENT_TARGET)
	@echo "Starting client..."
	./$(CLIENT_TARGET)

# Rebuild everything
rebuild: clean all

# Install (copy to /usr/local/bin - requires sudo)
install: all
	@echo "Installing to /usr/local/bin..."
	sudo cp $(SERVER_TARGET) /usr/local/bin/
	sudo cp $(CLIENT_TARGET) /usr/local/bin/
	@echo "Installation completed!"

# Uninstall
uninstall:
	@echo "Uninstalling..."
	sudo rm -f /usr/local/bin/$(SERVER_TARGET)
	sudo rm -f /usr/local/bin/$(CLIENT_TARGET)
	@echo "Uninstall completed!"

# Help target
help:
	@echo "TerminalTalk TCP Server - Makefile"
	@echo "==================================="
	@echo "Available targets:"
	@echo "  make           - Build both server and client"
	@echo "  make all       - Build both server and client"
	@echo "  make server    - Build only server"
	@echo "  make client    - Build only client"
	@echo "  make clean     - Remove build artifacts"
	@echo "  make rebuild   - Clean and build everything"
	@echo "  make run-server - Build and run server"
	@echo "  make run-client - Build and run client"
	@echo "  make install   - Install to system (requires sudo)"
	@echo "  make uninstall - Uninstall from system"
	@echo "  make help      - Show this help message"

# Declare phony targets
.PHONY: all server client clean run-server run-client rebuild install uninstall help
