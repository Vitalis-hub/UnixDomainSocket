#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>
#include "connection.h"

using std::endl; using std::string;
using namespace std;

void Manager(string targetWord){
    struct sockaddr_un addr;
    int ret;
    int data_socket;
    char buffer[BUFFER_SIZE];
    data_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if(data_socket == -1){
        perror("socket");
        exit(EXIT_FAILURE);
    }
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);
    ret = connect(data_socket, (const struct sockaddr *) &addr, sizeof(addr));
    if(ret == -1) {
        fprintf(stderr, "The server is down. \n");
        exit(EXIT_FAILURE);
    }

    vector<string> lines;

    ret = read(data_socket, buffer, sizeof(buffer));
           if (ret == -1) {
               perror("read");
               exit(EXIT_FAILURE);
           }

           /* Ensure buffer is 0-terminated. */
    buffer[sizeof(buffer) - 1] = 0;
    int res = 0;
    for(;;){
        memset(buffer, 0, sizeof(buffer));
        ret = read(data_socket, buffer, sizeof(buffer));
        if(ret == -1){
            perror("read");
            exit(EXIT_FAILURE);
        }
        buffer[sizeof(buffer) - 1] = 0;
        if(strncmp(buffer, "END", BUFFER_SIZE) == 0){
            for(int j = 0; j < lines.size(); j++){
                memset(buffer, 0, sizeof(buffer));
                for(int i =0; i < lines[j].length(); i++) {
                    if(lines[j].length() - targetWord.length() < 0) break;
                    string data = (string)lines[j].substr(i, targetWord.length());
                    data.erase (std::remove_if (data.begin (), data.end (), ::ispunct), data.end ());
                    if(strcasecmp(data.c_str(), targetWord.c_str()) == 0 && (i == lines[j].length() -1|| lines[j][i+ targetWord.length()] == ' ')){
                        //sprintf(buffer, "%s", lines[j]);
                        std::string str = lines[j];
                        const char *c = str.c_str();
                        strncpy(buffer, c, lines[j].length());
                        ret = write(data_socket, buffer, strlen(buffer) + 1);
                        if(ret == -1){
                            perror("write");
                            exit(EXIT_FAILURE);
                        }
                        break;
                    }
                }
                if(j + 1 == lines.size()){
                    memset(buffer, 0, sizeof(buffer));
                    string x = "END";
                    const char *c = x.c_str();
                    strncpy(buffer, c, x.length());
                    ret = write(data_socket, buffer, strlen(buffer)+ 1);
                    if(ret == -1){
                        perror("write");
                        exit(EXIT_FAILURE);
                    }
                }
            }
            break;
        }
        if(sizeof(buffer) == 0) continue;
        lines.push_back(buffer);
    }
    close (data_socket);
    exit(EXIT_SUCCESS);
}
struct CommandLineException {
    CommandLineException(int m,int a)
    {   cout << endl;
        cout << "Too many file names on the command line.";
        cout << endl;
        cout << "A maximum of " << m << " file names can appear on the command line.";
        cout << endl;
        cout << a << " file names were entered.";
        cout << endl;
        cout << "p01 (<input file name> (<output file name>))";
    }
};

int main(int argc, char* argv[])
{   try {
        char target[255];
        switch (argc) {
            case 1://no files on the command line
                cout << "Enter the target word: ";
                cin >> target;
                cout << endl;
            break;
            case 2://input file on the command line/prompt for output file
                strcpy(target,argv[1]);
            break;
            default:
                throw CommandLineException(1,argc-1);
                break;
        }
        string x;
        //ifstream file;
        std::string targetWord= std::string(target);
        Manager(targetWord);
        cout << "here i am down on my knees again" << endl;
    } catch ( ... ) {
        cout << endl;
        cout << "Program Terminated!";
        cout << endl;
        cout << "I won't be back!";
        cout << endl;
        exit(EXIT_FAILURE);
    }
    return 0;
}