#include "../include/multiplexing.h"

struct id{
    int IDNum;
    //person* person();

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
            createOrder();
            return true;
        }else{
            return false;
        }
        return true;
    };

    bool sellAssetMarket(int assetNum, float amt){
        cash = cash + marketSell(assetNum, amt);
        return true;
    }


};

struct Asset{
    // map of each price and the peaple and amount of orders there
    std::map<double, std::queue<std::pair<int, id>>*, std::greater<double>> buyOrders;
    std::map<double, std::queue<std::pair<int, id>>*, std::greater<double>> sellOrders;

    int AssetID;

    Asset(): buyOrders {},sellOrders {}, AssetID {}{}

    void addOrder(bool isBuy, int amount, double price, id ID){
        if (isBuy){
            try
            {
                (buyOrders.at(price))->push({amount,ID});
            }
            catch(const std::out_of_range& e)
            {
                std::queue<std::pair<int, id>> newQueue;
                newQueue.push({price, ID});
                buyOrders[price] = &newQueue;
            };
        }else{

        }
    }



};

struct Future : public Asset{
    timeval timeExecuted;
    Future(timeval executionTime, int ID){
        AssetID = ID;
        timeExecuted = executionTime;
    }
};





struct order{

};



struct personList{

};