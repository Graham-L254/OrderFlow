#include "header.h"

#include <vector>

std::map<double, int> buyOrders {};
std::map<double, int> sellOrders {};

//sockaddr_in sockAddress;

SOCKET s;
SOCKET clientSocket;
int i {};

const long long int numSockets {5};

std::vector<SOCKET> sockets (numSockets);
fd_set sockets_set;


int w {};

timeval waitTime {1,1};

int main(){
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        std::cout << "WSAStartup failed: " << WSAGetLastError() << "\n";
        return 1;
    }
    for (int i {}; i < numSockets; ++i){
        sockets[i] = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP);
        if (sockets[i] == INVALID_SOCKET) {
            std::cout << "Socket creation failed: " << WSAGetLastError() << "\n";
            continue;
        }

        sockaddr_in sockAddress{};
        sockAddress.sin_family = AF_INET;
        sockAddress.sin_port = htons(5501+i);
        sockAddress.sin_addr.s_addr = INADDR_ANY;
        

        int opt = 1;
        if (setsockopt(sockets[i], SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) != 0){
            std::cout << "setsockopt failed on port " << 5501+i << ": " << WSAGetLastError() << "\n";
        }

        if (bind(sockets[i], (SOCKADDR*)&sockAddress, sizeof(sockAddress)) != 0){
            std::cout << "Bind failed on port " << 5501+i << ": " << WSAGetLastError() << "\n";
            continue;
        }

        if (listen(sockets[i], SOMAXCONN) != 0){
            std::cout << "Listen failed on port " << 5501+i << ": " << WSAGetLastError() << "\n";
            continue;
        }


        std::cout << "Listening on port " << 5501 + i << "\n";
    }
    try{
        while (true){
            
            ++w;
            w = w % 5;

            //w = 0;

            waitTime = {0, 10};

            //std::cout << "w is " << w << "\n";

            FD_ZERO(&sockets_set);

            FD_SET(sockets[w],&sockets_set);
            if (select(0,&sockets_set,nullptr,nullptr,&waitTime) > 0){

                

                clientSocket = accept(sockets_set.fd_array[0], NULL, NULL);
                

                char buffer[1024] = {0};

                recv(clientSocket, buffer, sizeof(buffer), 0);
                
                std::cout << buffer << "\n";
            }

        }
    }catch(...){
        WSACleanup();
        std::cout << "ConnFailed";
    }

    
    closesocket(clientSocket);
    WSACleanup();
    return 1;
}


bool createBuyOrder(double price, int PersonID){
    return true;
}

int marketSell(int asset, int amt){
    return 1;
}
