#pragma once
#include "../include/multiplexing.h"


// prints values every numTradesPrint 
int numTradesPrint {5000};
long numTrades {};
static bool print {false};

struct id{
    int IDNum;

    id(){
        IDNum = -1;
    }

    id(int num){
        IDNum = num;
    }
};



struct person{
    double starting_amt;
    double cash;
    int assets;
    id ID {};
    static int numPeople;

    person(){
        starting_amt = 1000; assets = 0;;
        ID = id(numPeople);
        cash = starting_amt;
        ++numPeople;
    }

    person(int start_value){
        starting_amt: start_value; assets: {};
        cash = starting_amt;
        ID = id(numPeople);
    }


};
int person::numPeople = 0;
std::vector<person> peopleList (5);

class node{
public:
    std::pair<int,id> value;
    node* next {nullptr};

    node(std::pair<int, id> values) : value{values} {
        
    }
};


class doubleEndedLinkedList{

public:
    double price;
    node* start;
    node* end;
    std::map<double, doubleEndedLinkedList>* parentMarket;

    doubleEndedLinkedList(std::pair<int,id> value) : start{nullptr},end{nullptr}{
        add(value);
    }

    doubleEndedLinkedList() : start{nullptr},end{nullptr}{}

    int getSize(){
        int count {};
        auto* ptr = start;
        while (ptr != nullptr){
            count += ptr->value.first;
            ptr = ptr->next;
        }
        return count;
    }

    void add(std::pair<int,id> value){
        node* temp = new node(value);
        if (start == nullptr){
            start = temp;
            end = start;
        }else{
            end->next = temp;
            end = temp;
        }
    }

    //sell subtract
    int subtract(int amount, std::map<double, doubleEndedLinkedList, std::greater<>>* market, double price){

        while (start != nullptr && amount > 0){
            int filled;
            node* consumedNode = nullptr;

            if (start->value.first > amount){
                filled = amount;
                start->value.first -= amount;
                amount = 0;
            } else {
                filled = start->value.first;
                amount -= filled;
                consumedNode = start;
                start = start->next;
            }

            int idNum = (consumedNode ? consumedNode : start)->value.second.IDNum;
            peopleList[idNum].cash -= filled * price;
            peopleList[idNum].assets += filled;
        }
        if (start == nullptr){
            market->erase(price);
        }
        return amount;
    }

    //buy subtract
    int subtract(int amount, std::map<double, doubleEndedLinkedList>* market, double price){

        while (start != nullptr && amount > 0){
            int filled;
            node* consumedNode = nullptr;

            if (start->value.first > amount){
                filled = amount;
                start->value.first -= amount;
                amount = 0;
            } else {
                filled = start->value.first;
                amount -= filled;
                consumedNode = start;
                start = start->next;
            }

            int idNum = (consumedNode ? consumedNode : start)->value.second.IDNum;
            peopleList[idNum].cash += filled * price;
            peopleList[idNum].assets -= filled;
        }
        if (start == nullptr){
            market->erase(price);
        }
        return amount;
    }

};



struct Asset{
    // map of each price and the peaple and amount of orders there
    std::map<double, doubleEndedLinkedList, std::greater<>> buyOrders;
    std::map<double, doubleEndedLinkedList> sellOrders;

    int AssetID;

    Asset(int ID): buyOrders {}, sellOrders {}, AssetID {ID}{}

    void addOrder(bool isBuy, int amount, double price, id ID){
        ++numTrades;
        if(numTrades % numTradesPrint == 0){
            for (int i {}; i < peopleList.size(); ++i){
                std::cout << "\nperson: " << i << ", cash: " << peopleList[i].cash << ", assets: " << peopleList[i].assets << "\n";
                std::cout << "total min: " << peopleList[i].cash + peopleList[i].assets * buyOrders.begin()->first << " total max: " << peopleList[i].cash + peopleList[i].assets * sellOrders.begin()->first;
            }
            std::cout << "\n" << "-----------------------------" << "\n";
        }
        if (isBuy){
            try
            {
                std::pair<int, id> placeHolder = {amount,ID}; 
                buyOrders[price].add(placeHolder);
            }
            catch(const std::out_of_range& e)
            {
                std::pair<int, id> temp = {amount, ID};
                buyOrders[price] = doubleEndedLinkedList({amount, ID});
            };
        }else{
            try
            {
                sellOrders[price].add({amount,ID});
            }
            catch(const std::out_of_range& e)
            {
                sellOrders[price] = doubleEndedLinkedList({amount, ID});
            };
        }

        if (print){
            printMarket();

            if(orderMatch(isBuy)){
                std::cout << "\n" << "---------" << "\n";
                printMarket();
            }
        }else{
            orderMatch(isBuy);
        }
        
    }

    void printMarket(){
        
        std::cout << "\n \n sellOrders: \n \n";
        for (const auto& value : sellOrders){
            auto* ptr = value.second.start;

            std::cout << value.first << " || ";

            while (ptr != nullptr){
                std::cout << "contracts: " << ptr->value.first << " ID: " << ptr->value.second.IDNum;
                ptr = ptr->next;
            }
            std::cout << "\n";
        }

        std::cout << "\n \n buyOrder: \n \n";
        for (const auto& value : buyOrders){
            auto* ptr = value.second.start;

            std::cout << value.first << " || ";

            while (ptr != nullptr){
                std::cout << "contracts: " << ptr->value.first << " ID: " << ptr->value.second.IDNum;
                ptr = ptr->next;
            }
            std::cout << "\n";
        }


    }

    bool orderMatch(bool buy){
        
        double bestBuy {buyOrders.begin()->first};
        double bestSell {sellOrders.begin()->first};
        if (print){
            std::cout << bestBuy;
            std::cout << bestSell;
        }
        // checks if orders will execute and if there are orders
        while (true){
            if (buyOrders.empty() || sellOrders.empty()){
                return false;
            }
            bestBuy = buyOrders.begin()->first;
            bestSell = sellOrders.begin()->first;
            if (bestBuy < bestSell){
                return false; 
            }
            else if (buy){
                if (print){
                    std::cout << "buy condition";
                }
                int sizeOfBestPrice {buyOrders.begin()->second.getSize()};
                int tradedVolume = sizeOfBestPrice - sellOrders.begin()->second.subtract(sizeOfBestPrice, &sellOrders, bestSell);
                if (tradedVolume == 0){
                    break;
                }
                if (print){
                    std::cout << "traded volume: " << tradedVolume;
                }
                buyOrders.begin()->second.subtract(tradedVolume, &buyOrders, bestSell);
            }else if (!buy){
                if (print){
                    std::cout << "sell condition";
                }
                int sizeOfBestPrice {sellOrders.begin()->second.getSize()};
                int tradedVolume = sizeOfBestPrice - buyOrders.begin()->second.subtract(sizeOfBestPrice, &buyOrders, bestBuy);
                if (tradedVolume == 0){
                    break;
                }
                if (print){
                    std::cout << "traded volume: " << tradedVolume;
                }
                sellOrders.begin()->second.subtract(tradedVolume, &sellOrders, bestBuy);
            }
        }
        if (print){
            printMarket();
        }
        return true;
    }

    



};

struct Future : public Asset{
    timeval timeExecuted;
    Future(timeval executionTime, int ID) : Asset(ID){
        
        timeExecuted = executionTime;
    }
};




struct order{
    int amount;
    int IDNum;
    double price;

    order(int amount, int IDNum, double price) : amount{amount}, IDNum{IDNum}, price{price}{

    }
};



struct personList{

};


