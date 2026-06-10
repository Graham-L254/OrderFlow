#include "../include/client.h"

sockaddr_in clientSocket;
SOCKET s;

int main(){
    std::string id;
    std::cout << "ID:";
    std::cin >> id;

    WSADATA wsaData; 
    WSAStartup(MAKEWORD(2,2), &wsaData);
    
    
    hostent* localHost = gethostbyname("");
    char* localIP = inet_ntoa (*(struct in_addr *)*localHost->h_addr_list);

    clientSocket.sin_family = AF_INET;
    clientSocket.sin_port = htons(5501 + stoi(id));
    clientSocket.sin_addr.s_addr = inet_addr(localIP);
    
    const sockaddr_in mySocket = clientSocket;

    SOCKET connectedSocket;

    //connect(s, (SOCKADDR*)&clientSocket, sizeof(clientSocket));


    while(true){
        std::cout << "input params: (amt, assetNum, price)";
        int input[3];
        std::cin >> input[0];
        std::cin >> input[1];
        std::cin >> input[2];

        s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        connectedSocket = connect(s, (SOCKADDR*)&clientSocket, sizeof(clientSocket));

        std::cout << "connected \n";

        long long fullInput {};

        char bitInput[12];

        std::memcpy(bitInput, &(input), 12 );

        std::cout << send(s, bitInput, 12, 0) << "\n";

        //std::cout << WSAGetLastError();

        std::cout << "sent" << fullInput << "\n";

        WSASendDisconnect(s, NULL);
    }
    
    WSACleanup();
    return 1;
}
