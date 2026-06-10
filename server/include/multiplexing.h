#pragma once

#include <winsock2.h>
#include <map>
#include <iostream>
#include <charconv>
#include <string>
#include <array>
#include <vector>
#include <queue>
#include <functional>
#include <cstring>
#include "../src/source.h"

void connectSockets();
order interperetInput(char buffer[1024]);
bool createOrder();
int marketSell(int asset, int amt);