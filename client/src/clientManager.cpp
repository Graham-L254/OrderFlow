#include <ctime>
#include <cstdlib>
#include <cmath>
#include <chrono>
#include "../include/client.h"

static bool print(false);

void client_w_accuracy(int id, int accuracy, int range);

const int num_runs {100'000};

int main(){
    int instance_num;
    std::cout << "Num Instances: ";
    std::cin >> instance_num;
    std::vector<int> accuracies (instance_num);
    for (int i {}; i < instance_num; ++i){
        std::cout << "accuracy for " <<  i;
        std:: cin >> accuracies[i];
    }
    std::vector<std::thread> thread_vector (instance_num);
    
    auto start = std::chrono::steady_clock::now();

    for (int i {}; i < instance_num; ++i){
        std::thread newThread(client_w_accuracy, i, accuracies[i], 5);
        thread_vector[i] = (move(newThread));
    }

    for (int i {}; i < instance_num; ++i){
        thread_vector[i].join();
    }

    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Elapsed: " << elapsed.count() << " ms\n";

    

}

void client_w_accuracy(int id, int accuracy, int range){

    sockaddr_in clientSocket;
    SOCKET s;

    WSADATA wsaData; 
    WSAStartup(MAKEWORD(2,2), &wsaData);
    
    
    hostent* localHost = gethostbyname("");
    char* localIP = inet_ntoa (*(struct in_addr *)*localHost->h_addr_list);

    clientSocket.sin_family = AF_INET;
    clientSocket.sin_port = htons(5501 + id);
    clientSocket.sin_addr.s_addr = inet_addr(localIP);
    
    const sockaddr_in mySocket = clientSocket;

    SOCKET connectedSocket;


    s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    connectedSocket = connect(s, (SOCKADDR*)&clientSocket, sizeof(clientSocket));

    for(int i {}; i < num_runs; ++i){

        int expectedPrice = 100 + std::sin(std::time(nullptr) % 20) * 20;
        int error = (std::rand() % 2) * 2 * accuracy - accuracy;

        bool buy {std::rand() % 100 >= 50};

        int input[3];
        input[0] = 1 - 2* buy;
        input[1] = id;
        input[2] = expectedPrice + error + range - 2 * buy * range;


        if (print){
            std::cout << "connected \n";
        }

        long long fullInput {};

        char bitInput[12];

        std::memcpy(bitInput, &(input), 12 );

        if (print){
            std::cout << send(s, bitInput, 12, 0) << "\n";

            std::cout << "sent" << fullInput << "\n";
        }else{
            send(s, bitInput, 12, 0);
        }




    }
    
    WSASendDisconnect(s, NULL);
    closesocket(s);

    WSACleanup();

}