# MiniDocker

A lightweight, small-scale **Docker-like container runtime** implemented in C on Linux as part of an Operating Systems course project. The project demonstrates core concepts involved in containerization, including filesystem/root filesystem handling, container execution, image creation, and command-line interfaces.

## Features

- Run containers from locally available images
- Build images from a Dockerfile
- List available images
- Initialize a root filesystem
- Configure a container hostname
- Specify a custom root filesystem
- Run commands in interactive mode
- Custom command-line interface using `getopt_long`
- Separate modules for containers, filesystems, and images
- Makefile-based compilation

## Technologies Used

- **C**
- **GNU/Linux**
- **Make**
- **POSIX/Linux system calls**
- **getopt / getopt_long**

## Project Structure

```text
MiniDocker/
├── examples/
├── images/
├── src/
│   ├── images/
│   │   └── hello-world/bin/
│   ├── Makefile
│   ├── container.c
│   ├── container.h
│   ├── container.o
│   ├── filesystem.c
│   ├── filesystem.h
│   ├── filesystem.o
│   ├── image.c
│   ├── image.h
│   ├── image.o
│   ├── main.c
│   ├── main.o
│   └── minidocker
└── test/
```

The `examples/`, `images/`, and `test/` directories contain supporting examples, image-related files, and test material respectively.

The `src/` directory contains the main implementation. The `.o` files and `minidocker` executable are generated build artifacts.

## Architecture

The implementation is divided into three main components:

### Container Management

```text
container.c
container.h
```

Responsible for configuring and running containers, including options such as hostname, root filesystem, command, and interactive execution.

### Filesystem Management

```text
filesystem.c
filesystem.h
```

Provides filesystem/root filesystem-related functionality used by the container runtime.

### Image Management

```text
image.c
image.h
```

Handles image-related operations, including building images and running containers from images.

### Command-Line Interface

```text
main.c
```

Provides the main command-line interface and dispatches operations such as `run`, `build`, and `images`.

## Commands

### Run a Container

Run a container from an existing image:

```bash
./minidocker run IMAGE_NAME
```

### Build an Image

Build an image using a Dockerfile:

```bash
./minidocker build DOCKERFILE_PATH IMAGE_NAME
```

### List Images

List the available images:

```bash
./minidocker images
```

## Command-Line Options

The program also supports the following options:

```text
-h, --hostname HOSTNAME
-r, --rootfs PATH
-i, --interactive
    --init-rootfs PATH
    --help
```

Examples:

```bash
./minidocker --help
```

Initialize a root filesystem:

```bash
./minidocker --init-rootfs PATH
```

Run a command with a custom hostname:

```bash
./minidocker --hostname my-container COMMAND
```

Run with a custom root filesystem:

```bash
./minidocker --rootfs PATH COMMAND
```

Interactive execution can be enabled with:

```bash
./minidocker --interactive COMMAND
```

The exact behavior of these options depends on the implementation and the underlying Linux environment.

## Building

Clone the repository:

```bash
git clone <repository-url>
cd MiniDocker
```

Navigate to the source directory:

```bash
cd src
```

Compile the project using the provided Makefile:

```bash
make
```

This builds the `minidocker` executable and the required object files.

To remove generated build artifacts, if supported by the Makefile:

```bash
make clean
```

## Running

After compilation, the executable can be run from the `src/` directory:

```bash
./minidocker --help
```

For example:

```bash
./minidocker images
```

or:

```bash
./minidocker run hello-world
```

## Linux Environment

MiniDocker is designed around operating-system-level functionality and is intended to be used in a **Linux environment**. Some functionality may require appropriate permissions and Linux-specific system calls or filesystem features.

It is not intended to be a production replacement for Docker. Instead, it is a small-scale educational implementation for understanding how container runtimes can be constructed from OS-level primitives.

## Learning Objectives

The project demonstrates practical concepts related to:

- Operating-system process management
- Filesystem isolation and root filesystems
- Container configuration
- Image management
- Command execution
- System-level programming in C
- Linux command-line interfaces
- Build systems and Makefiles

## Project Status

This project was developed as an **Operating Systems course project** to implement a simplified Docker-like environment and explore the underlying concepts behind containerization.

## Course

**Operating Systems**

## Author

**Saransh**

## Purpose

The project was developed as an academic implementation of a lightweight container runtime, with the goal of understanding the operating-system concepts behind containerization rather than reproducing the complete functionality of Docker.
