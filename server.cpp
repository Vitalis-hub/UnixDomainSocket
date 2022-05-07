#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <cctype>
#include <string>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/un.h>
#include <unistd.h>
#include "connection.h"
using namespace std;
vector<string> sendPacketData(vector<string> data){
    struct sockaddr_un name;
    int down_flag = 0;
    int ret;
    int connection_socket;
    int data_socket;
    string buffer[BUFFER_SIZE];
    char buffer2[BUFFER_SIZE];
    connection_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if(connection_socket == -1){
        perror("socket");
        exit(EXIT_FAILURE);
    }
    memset(&name, 0, sizeof(name));
    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);
    ret = bind(connection_socket, (const struct sockaddr *) &name,
    sizeof(name));
    if(ret == -1){
        perror("bind");
        exit(EXIT_FAILURE);
    }
    ret = listen(connection_socket, 20);
    if(ret == -1){
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    data_socket = accept(connection_socket, NULL, NULL);
    if(data_socket == -1){
        perror("accept");
        exit(EXIT_FAILURE);
    }

    int wordSearchLength = data.size();
    sprintf(buffer2, "%d", wordSearchLength);
    ret = write(data_socket, buffer, sizeof(buffer));
    if (ret == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    vector<string> res;
    for(int i =0; i < data.size(); i++){
        //sprintf(buffer2, "%s", data[i]);
        memset(buffer2, 0, sizeof(buffer2));
        std::string str = data[i];
        const char *c = str.c_str();
        strncpy(buffer2, c, data[i].length());
        ret = write(data_socket, buffer2, sizeof(buffer2));
        if(ret == -1){
            perror("write");
            exit(EXIT_FAILURE);
        }
        if(i + 1 == data.size()){
            memset(buffer2, 0, sizeof(buffer2));
            string x = "END";
            // sprintf(buffer2, "%s", x);
            const char *c = x.c_str();
            strncpy(buffer2, c, x.length());
            ret = write(data_socket, buffer2, sizeof(buffer2));
            if(ret == -1){
                perror("write");
                exit(EXIT_FAILURE);
            }
            break;
        }
    }
    
    for(;;){
        ret = read(data_socket, buffer2, sizeof(buffer2));
        if(ret == -1){
            perror("read");
            exit(EXIT_FAILURE);
        }
        buffer2[sizeof(buffer2) - 1] = 0;
        if(strncmp(buffer2, "END", sizeof(buffer2)) == 0){
            down_flag = 1;
            break;
        }
        res.push_back(buffer2);
    }
    wait(NULL);
    close(connection_socket);
    unlink(SOCKET_NAME);

    return res;
}