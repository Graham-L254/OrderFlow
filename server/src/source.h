#pragma once
#include "../include/multiplexing.h"

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
    std::map<int, double> assets;
    std::map<int, double> orders;
    id ID {};
    static int numPeople;

    person(){
        starting_amt: 1000; assets: {};
        ID = id(numPeople);
        cash = starting_amt;
        ++numPeople;
    }

    person(int start_value){
        starting_amt: start_value; assets: {};
        cash = starting_amt;
        ID = id(numPeople);
    }

    bool buyAsset(int asset, double price, float units){
        if (cash > price * units){
            cash = cash - price * units;
            //createOrder();
            return true;
        }else{
            return false;
        }
        return true;
    };

    bool sellAssetMarket(int assetNum, float amt){
        cash = cash;
        // + marketSell(assetNum, amt);
        return true;
    }


};



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

    void add(std::pair<int,id> value){
        node* temp = new node(value);
        if (start == nullptr){
            start = temp;
            end = start;
        }else{
            end->next = temp;
        }
    }

    int subtract(int amount){
        while (start != nullptr || amount == 0){
            amount -= start->value.first;
            if (amount > 0){
                start->value.first = -amount;
            }else{
                start = start-> next;
            }
            
        }

        //if (start == nullptr){
        //    this->doubleEndedLinkedListDestructor();
        //}

        return amount;
    }

    //void doubleEndedLinkedListDestructor(std::map<double, doubleEndedLinkedList>* market, double price){
    //    market->erase(price);
        //~doubleEndedLinkedList();
    //}

};



struct Asset{
    // map of each price and the peaple and amount of orders there
    std::map<double, doubleEndedLinkedList, std::greater<>> buyOrders;
    std::map<double, doubleEndedLinkedList> sellOrders;

    int AssetID;

    Asset(int ID): buyOrders {},sellOrders {}, AssetID {ID}{}

    void addOrder(bool isBuy, int amount, double price, id ID){
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
    }

    void printMarket(){
        
        std::cout << "\n \n buyOrder: \n \n";
        for (const auto& value : buyOrders){
            auto* ptr = value.second.start;
            while (ptr != nullptr){
                std::cout << "contracts: " << ptr->value.first << " ID: " << ptr->value.second.IDNum;
                ptr = ptr->next;
            }
            std::cout << "\n";
        }

        std::cout << "\n \n sellOrders: \n \n";
        for (const auto& value : sellOrders){
            auto* ptr = value.second.start;
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
        // checks if orders will execute
        if (bestBuy > bestSell){
            return false; 
        }
        else if (buy){
            while(buyOrders.begin()->first > buyOrders.begin()->first){

            }
        }else if (!buy){

        }
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


