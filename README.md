# Custom Memory Allocator  

**Efficient and customizable memory allocation library**  

## Overview  
This project implements a custom memory allocator in C++ that provides efficient dynamic memory management for applications. The allocator supports advanced features like tracking memory statistics, custom replacement strategies, and debugging tools.  

## Features  
- **Dynamic memory allocation and deallocation**  
- **Memory statistics tracking (`mstats`)**  
- **Custom memory replacement strategy (`mreplace`)**  
- **Compatibility with standard C++ programs**  
- **Docker environment for easy setup and testing**  

## Files  
- **`alloc.c`**: Core implementation of the custom memory allocator.  
- **`alloc.so`**: Compiled shared object of the memory allocator for runtime use.  
- **`mreplace`**: Tool to replace libc allocation functions with the custom allocator.  
- **`mstats`**: Tracks and prints memory usage statistics for the custom allocator.  
- **`mstats-libc`**: Memory statistics tool for libc allocation for comparison.  
- **`mstats.c`**: Source file for memory statistics tools.  
- **`test`**: A directory containing test cases to validate allocator performance.  
- **`tay-small.gif` & `tay-small-illinify.gif`**: Visual aids or animations related to the allocator (if applicable).  

## Setup and Installation  

### Prerequisites  
- **C++ Compiler**: GCC or Clang recommended.  
- **Docker** (optional): For containerized testing and development.  

### Compilation  
Use the provided `Makefile` for building the project:  

```bash  
make  

Usage
Running the Custom Allocator
Compile your application using the custom allocator:

bash
Copy code
gcc -o app app.c -L. -lalloc  
Use the mreplace tool to load the custom allocator dynamically:

bash
Copy code
LD_PRELOAD=./alloc.so ./app  
Memory Statistics
Run the mstats tool to monitor and print memory usage:

bash
Copy code
./mstats ./app  
Docker Environment
Build the Docker image:

bash
Copy code
docker build -t custom-allocator .  
Run the Docker container:

bash
Copy code
docker run -it custom-allocator  
Tests
Navigate to the tests directory to find test cases:

bash
Copy code
cd tests  
make test  
Run individual tests:

bash
Copy code
./test_case_name  
Contributing
Feel free to fork this repository and submit pull requests. Make sure to follow the coding style guidelines and provide appropriate test cases for new features.

License
This project is licensed under the MIT License. See the LICENSE file for details.
