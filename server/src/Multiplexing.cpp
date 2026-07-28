#include "../include/multiplexing.h"

SOCKET listenSockets[5];
const long int numSockets {5};

Asset market = Asset(0);
std::mutex marketMutex;



HANDLE g_iocp;

struct PerIoContext {
    OVERLAPPED overlapped;
    WSABUF wsabuf;
    char buffer[12];
};

struct PerConnectionContext {
    SOCKET socket;
    PerIoContext io;
    int totalReceived {0};
};

void processOrder(char* buffer){
    int amt{}, idNum{}, price{};
    std::memcpy(&amt, buffer, 4);
    std::memcpy(&idNum, buffer + 4, 4);
    std::memcpy(&price, buffer + 8, 4);

    if (print){
        std::cout << "amount:" << amt << "\n";
        std::cout << "idNum:" << idNum << "\n";
        std::cout << "price:" << price << "\n";
    }

    if (!amt){
        std::cout << "invalid order, 0 size\n";
        return;
    }

    std::lock_guard<std::mutex> lock(marketMutex);
    market.addOrder(amt > 0, abs(amt), price / 100.0, idNum);
}

void postRecv(PerConnectionContext* ctx){
    ZeroMemory(&ctx->io.overlapped, sizeof(OVERLAPPED));
    ctx->io.wsabuf.buf = ctx->io.buffer + ctx->totalReceived;
    ctx->io.wsabuf.len = 12 - ctx->totalReceived;

    DWORD flags = 0;
    DWORD bytesRecvd = 0;

    int result = WSARecv(ctx->socket, &ctx->io.wsabuf, 1, &bytesRecvd, &flags, &ctx->io.overlapped, NULL);
    if (result == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING){
        std::cout << "WSARecv failed: " << WSAGetLastError() << "\n";
        closesocket(ctx->socket);
        delete ctx;
    }
}

void workerThread(){
    while (true){
        DWORD bytesTransferred = 0;
        ULONG_PTR completionKey = 0;
        LPOVERLAPPED overlapped = nullptr;

        BOOL ok = GetQueuedCompletionStatus(g_iocp, &bytesTransferred, &completionKey, &overlapped, INFINITE);

        if (overlapped == nullptr){
            // the wait call itself failed, not a specific connection's I/O
            std::cout << "GetQueuedCompletionStatus failed: " << GetLastError() << "\n";
            continue;
        }

        auto* ctx = reinterpret_cast<PerConnectionContext*>(completionKey);

        if (!ok || bytesTransferred == 0){
            closesocket(ctx->socket);
            delete ctx;
            continue;
        }

        ctx->totalReceived += bytesTransferred;

        if (ctx->totalReceived < 12){
            postRecv(ctx);
        } else {
            processOrder(ctx->io.buffer);
            ctx->totalReceived = 0;
            postRecv(ctx);
        }
    }
}

void acceptLoop(SOCKET listenSocket){
    while (true){
        SOCKET clientSocket = accept(listenSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET){
            std::cout << "accept failed: " << WSAGetLastError() << "\n";
            continue;
        }

        auto* ctx = new PerConnectionContext();
        ctx->socket = clientSocket;

        if (CreateIoCompletionPort((HANDLE)clientSocket, g_iocp, (ULONG_PTR)ctx, 0) == NULL){
            std::cout << "CreateIoCompletionPort (per-socket) failed: " << GetLastError() << "\n";
            closesocket(clientSocket);
            delete ctx;
            continue;
        }

        postRecv(ctx);
    }
}

void connectSockets(){
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        std::cout << "WSAStartup failed: " << WSAGetLastError() << "\n";
        return;
    }
    for (int i {}; i < numSockets; ++i){
        listenSockets[i] = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (listenSockets[i] == INVALID_SOCKET) {
            std::cout << "Socket creation failed: " << WSAGetLastError() << "\n";
            continue;
        }

        sockaddr_in sockAddress{};
        sockAddress.sin_family = AF_INET;
        sockAddress.sin_port = htons(5501 + i);
        sockAddress.sin_addr.s_addr = INADDR_ANY;

        int opt = 1;
        setsockopt(listenSockets[i], SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

        if (bind(listenSockets[i], (SOCKADDR*)&sockAddress, sizeof(sockAddress)) != 0){
            std::cout << "Bind failed on port " << 5501 + i << ": " << WSAGetLastError() << "\n";
            continue;
        }

        if (listen(listenSockets[i], SOMAXCONN) != 0){
            std::cout << "Listen failed on port " << 5501 + i << ": " << WSAGetLastError() << "\n";
            continue;
        }

        std::cout << "Listening on port " << 5501 + i << "\n";
    }
}

int main(){
    connectSockets();

    g_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if (g_iocp == NULL){
        std::cout << "CreateIoCompletionPort failed: " << GetLastError() << "\n";
        return 1;
    }

    unsigned int numWorkers = std::thread::hardware_concurrency();
    if (numWorkers == 0) numWorkers = 4;

    std::vector<std::thread> workers;
    for (unsigned int i {}; i < numWorkers; ++i){
        workers.emplace_back(workerThread);
    }

    std::vector<std::thread> acceptThreads;
    for (int i {}; i < numSockets; ++i){
        acceptThreads.emplace_back(acceptLoop, listenSockets[i]);
    }

    for (auto& t : acceptThreads) t.join();  // blocks forever; these loops never return
    for (auto& t : workers) t.join();

    WSACleanup();
    return 0;
}