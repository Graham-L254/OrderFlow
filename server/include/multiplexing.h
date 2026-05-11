#include <winsock2.h>

#include <map>
#include <iostream>
#include <charconv>
#include <string>
#include <array>
#include <vector>
#include <queue>
#include "../src/source.cpp"


void connectSockets();
std::vector<std::string> interperetInput(char buffer[1024]);
bool createOrder();
int marketSell(int asset, int amt);