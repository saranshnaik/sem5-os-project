#!/bin/bash

echo "=== Minidocker Test ==="

# Build minidocker
echo "Building minidocker..."
make clean
make

# Create directory structure
echo "Creating directory structure..."
mkdir -p images/hello-world examples

# Create example hello.c file
cat > examples/hello.c << 'EOF'
#include <stdio.h>
#include <unistd.h>

int main() {
    printf("🚀 Hello World from Minidocker Container!\n");
    printf("📦 This is running from a custom container image\n");
    printf("🆔 Container PID: %d\n", getpid());
    
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        printf("🏠 Container Hostname: %s\n", hostname);
    }
    
    printf("✨ Demonstration successful!\n");
    return 0;
}
EOF

# Create Dockerfile for hello-world image
cat > images/hello-world/Dockerfile.minidocker << 'EOF'
COPY hello.c
RUN gcc -static -o /bin/app hello.c
ENTRYPOINT /bin/app
EOF

# Copy hello.c to image directory
cp examples/hello.c images/hello-world/

# Build the hello-world image
echo -e "\nBuilding hello-world image..."
./minidocker build images/hello-world/Dockerfile.minidocker hello-world

# List images
echo -e "\nListing available images..."
./minidocker images

# Test 1: Run hello-world image
echo -e "\nTest 1: Running hello-world image"
sudo ./minidocker run hello-world

echo -e "\n=== Tests completed ==="
