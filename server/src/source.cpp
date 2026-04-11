#include "../include/multiplexing.h"

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
    std::map<double, std::queue<std::pair<int, id>>&> buyOrders, sellOrders;


    int AssetID;

    Asset(): buyOrders {},sellOrders {}, AssetID {}{}

    void takerOrder(){

    }

    void makerOrder(double Price, bool buy){
        try{
            buyOrders[Price];
        }catch(const std::out_of_range& e){

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

struct id{
    int IDNum;
    person* person();

    id(){
        IDNum = -1;
    }

    id(int num){
        IDNum = num;
    }
};

struct personList{
    
};