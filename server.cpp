#include <iostream>
#include <bits/stdc++.h>
#include <cstring>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <mutex>
#include "calculator.h"

using namespace std;

#define PORT 4400
#define BUFFER_SIZE 16384

std::mutex coutMutex;

void printClientMessage(int clientSocket) {
    char buffer[BUFFER_SIZE] = {0};
    read(clientSocket, buffer, BUFFER_SIZE);
    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout << "Client message: " << buffer << std::endl;
}

void calculate(int clientSocket) {
    char buffer[BUFFER_SIZE] = {0};
    while (true) {
        int bytesRead = read(clientSocket, buffer, BUFFER_SIZE);
        if (bytesRead <= 0) break;

        std::istringstream iss(buffer);
        int operation;
        std::string a, b;
        iss >> operation >> a >> b;

        Root rootA(a), rootB(b);
        SuperCalculator calc;
        Node* result = nullptr;
        std::string operationName;

        if (operation == 1) {
            result = calc.add(rootA, rootB ,1);
            operationName = "Addition";
        } else if (operation == 2) {
            result = calc.subtract(rootA, rootB,1);
            operationName = "Subtraction";
        } else if (operation == 3) {
            result = calc.multiply(rootA, rootB ,1);
            operationName = "Multiplication";
        } 
        else if(operation == 4){
            ifstream obj;
            obj.open("history.txt");
            string s;
            while(obj){
                getline(obj,s);
                send(clientSocket,s.c_str(),s.length(),0);
            }
            obj.close();
        }
        else {
            std::string errorMessage = "Invalid operation selected. Please choose 1 for addition, 2 for subtraction, or 3 for multiplication.";
            send(clientSocket, errorMessage.c_str(), errorMessage.length(), 0);
            memset(buffer, 0, BUFFER_SIZE);
            continue;
        }
        // Convert the result to a string
        std::ostringstream resultStream;
        resultStream << operationName << " Result: ";
        
        // Traverse the linked list and print values to the result stream
        Node* currentNode = result;
        while (currentNode != nullptr) {
            resultStream << currentNode->value;
            currentNode = currentNode->next;
        }

        std::string finalResult = resultStream.str();
        std::cout << "Server calculated:\n" << finalResult << std::endl;

        send(clientSocket, finalResult.c_str(), finalResult.length(), 0);
        memset(buffer, 0, BUFFER_SIZE);
    }
    close(clientSocket);
}

void handleClient(int clientSocket) {
    while (true) {
        int p;
        if (read(clientSocket, &p, sizeof(p)) <= 0) {
            std::lock_guard<std::mutex> lock(coutMutex);
            std::cout << "Client disconnected.\n";
            break;
        }

        if (p == 0) {
            printClientMessage(clientSocket);
        } else if (p == 1) {
            calculate(clientSocket);
        } else if (p == -1) {
            std::lock_guard<std::mutex> lock(coutMutex);
            std::cout << "Client requested to terminate the connection.\n";
            break;
        } else {
            std::lock_guard<std::mutex> lock(coutMutex);
            std::cout << "Invalid value for p.\n";
        }
    }

    close(clientSocket);
}

int main() {
    int serverFd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(serverFd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(serverFd);
        exit(EXIT_FAILURE);
    }

    if (listen(serverFd, 3) < 0) {
        perror("Listen failed");
        close(serverFd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is running and waiting for connections...\n";

    while (true) {
        int clientSocket = accept(serverFd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (clientSocket < 0) {
            perror("Accept failed");
            continue;
        }

        std::cout << "Client connected. Spawning thread to handle connection...\n";
        std::thread clientThread(handleClient, clientSocket);
        clientThread.detach(); // Detach the thread to allow independent execution
    }

    close(serverFd);
    return 0;
}
