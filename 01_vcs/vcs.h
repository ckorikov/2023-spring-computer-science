#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <arpa/inet.h>
#include <filesystem>

#define PORT 8050
#define CHUNK_SIZE 1024
using std::string;
using std::endl;
using std::cout;
using std::vector;