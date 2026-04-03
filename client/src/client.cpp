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
        std::cout << "input params: (amt, assetNum, atMarket?)";
        std::string input;
        std::cin >> input;

        s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        connectedSocket = connect(s, (SOCKADDR*)&clientSocket, sizeof(clientSocket));

        std::cout << "connected \n";

        const char* message = (input.c_str());
        std::cout << send(s, message, strlen(message), 0);

        std::cout << WSAGetLastError();

        std::cout << "sent\n";

        WSASendDisconnect(s, NULL);
    }
    
    WSACleanup();
    return 1;
}
