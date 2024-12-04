#include <iostream>
#include <cstring>
#include <bits/stdc++.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;

#define PORT 4400
#define BUFFER_SIZE 16384

void sendMessageToServer(int socket) {
    int p = 0;
    send(socket, &p, sizeof(p), 0);

    std::string name, message;
    std::cout << "Enter your name: ";
    std::cin.ignore();
    std::getline(std::cin, name);

    std::cout << "Enter your message: ";
    std::getline(std::cin, message);

    std::string formattedMessage = name + ": " + message;
    send(socket, formattedMessage.c_str(), formattedMessage.size(), 0);
}

void sendNumbersToServer(int socket) {
    int p = 1;
    send(socket, &p, sizeof(p), 0);

    int operation;
        std::string a, b;
        std::cout << "Choose operation: 1 for addition, 2 for subtraction, 3 for multiplication  , 4 for history(-1 to exit): ";
        std::cin >> operation;

        

        if(operation == 4 ){
            string s="4 0 0";
            // cout<<s<<endl;
            send(socket,s.c_str(),s.length(),0);
            char p[1024]={0};
            if(read(socket ,p,1024)>0){
                cout<<p<<endl;
            }
            return ;
        }

        if (operation < 1 || operation > 3) {
            std::cout << "Invalid operation. Please try again.\n";
            return;
        }

        std::cout << "Enter the first number: ";
        std::cin >> a;
        std::cout << "Enter the second number: ";
        std::cin >> b;

        std::ostringstream messageStream;
        messageStream << operation << " " << a << " " << b;

        std::string finalMessage = messageStream.str();
        send(socket, finalMessage.c_str(), finalMessage.length(), 0);
        char buffer[BUFFER_SIZE] = {0};
        int bytesRead = read(socket, buffer, BUFFER_SIZE);
        if (bytesRead > 0) {
            std::cout << "Server response:\n" << buffer << std::endl;
        }
        memset(buffer, 0, BUFFER_SIZE);

}

int main() {
    int clientSocket;
    struct sockaddr_in serverAddress;

    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr) <= 0) {
        perror("Invalid address");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Connection failed");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    std::cout << "Connected to server.\n";

    while (true) {
        int p;
        std::cout << "Enter value for p (-1 to exit): ";
        std::cin >> p;

        if (p == -1) {
            send(clientSocket, &p, sizeof(p), 0);
            std::cout << "Exiting...\n";
            break;
        } else if (p == 0) {
            sendMessageToServer(clientSocket);
        } else if (p == 1) {
            sendNumbersToServer(clientSocket);
        } else {
            std::cout << "Invalid input. Please enter 0, 1, or -1.\n";
        }
    }

    close(clientSocket);
    return 0;
}