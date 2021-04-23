#ifndef UNTITLED3_ORDER_H
#define UNTITLED3_ORDER_H

#include <iostream>
#include <utility>
#include <thread>
#include <fstream>
#include <chrono>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <queue>
#include <cmath>
#include "Address.h"
#include "Payment.h"

using namespace std;

class Order{
private:
    Address _address_from, _address_to;
    string _car_number, _passenger_name, _driver_name;
    Payment _method;
    int _sum{};

public:
    Order(Address address_from, Address address_to, string car_number = "", string passenger_name = "", string driver_name = "",Payment method = Payment(), int sum = 0){
        _address_from = std::move(address_from);
        _address_to = std::move(address_to);
        _car_number = std::move(car_number);
        _passenger_name = std::move(passenger_name);
        _driver_name = std::move(driver_name);
        _method = std::move(method);
        _sum = sum;
    }
    Order(){}
    static Order from_string(string data){
        string name, res, car_number_, passenger_name_, driver_name_;
        Address address_from_, address_to_;
        int sum_ = 0;
        bool tor = false;
        Payment method_;
        for(int i=1;i<data.size();i++){
            if(data[i] == '\''){
                if(!tor){
                    name = "";
                    res = "";
                }else{
                    i+=2;
                }
                tor = !tor;
            }else if(data[i] == '{'){
                int count = 1;
                res = "{";
                i++;
                while(count!=0){
                    res += data[i];
                    if(data[i] == '{'){
                        count ++;
                    }else if(data[i] == '}'){
                        count --;
                    }
                    i++;
                }
                i--;
            }else if(data[i] == ',' || data[i] == '}'){
                if(name == "car_number"){
                    car_number_ = res;
                }else if(name == "passenger_name"){
                    passenger_name_ = res;
                }else if(name == "driver_name"){
                    driver_name_ = res;
                }else if(name == "address_to"){
                    address_to_ = Address::from_string(res);
                }else if(name == "address_from"){
                    address_from_ = Address::from_string(res);
                }else if(name == "method"){
                    method_ = Payment::from_string(res);
                }else if(name == "sum"){
                    sum_ = std::stoi(res);
                }
                i+=1;
            }else{
                if(tor){
                    name += data[i];
                }else{
                    res += data[i];
                }
            }
        }
        return Order(address_from_, address_to_, car_number_, passenger_name_, driver_name_, method_,
                     sum_);
    }
    string to_string(){
        return "{'address_from': "+_address_from.to_string()+", 'address_to': " + _address_to.to_string() + ", 'car_number': " + _car_number + ", 'passenger_name': " + _passenger_name + ", 'driver_name': " + _driver_name + ", 'method': " + _method.to_string() + ", 'sum': " + std::to_string(_sum) + "}";
    }
    bool is_picked(){
        return _driver_name.empty() || _car_number.empty();
    }
    void add_active(){
        ofstream file;
        if(!file_exists("ActiveOrders.txt")){
            file.open("ActiveOrders.txt");
        }else{
            file.open("ActiveOrders.txt", ios::app);
        }
        file<<to_string()<<endl;
        file.close();
    }
    void print(){
        cout<<"Address From : " <<_address_from.get_position()<<endl;
        cout<<"Address To : " <<_address_to.get_position()<<endl;
        cout<<"Car number : " <<_car_number<<endl;
        cout<<"Passenger Name : " <<_passenger_name<<endl;
        cout<<"Driver Name : " <<_driver_name<<endl;
        cout<<"Sum : "<<_sum<<endl;
    }
    string get_passenger_name(){
        return _passenger_name;
    }
    string get_driver_name(){
        return _driver_name;
    }
    void accept(string driver_name, string car_number){
        _driver_name = std::move(driver_name);
        _car_number = std::move(car_number);
    }
    Address get_address_from(){
        return _address_from;
    }
    Address get_address_to(){
        return _address_to;
    }
    void save_history(){
        string line;
        ifstream file_input("Drivers/orderHistory/" + _driver_name + ".txt");
        ofstream file_output("Drivers/orderHistory/" + _driver_name + ".txt");
        while(getline(file_input,line)){
            file_output<<line<<endl;
        }
        file_output<<to_string()<<endl;
        file_input.close();
        file_output.close();

        file_input.open("Passengers/orderHistory/" + _passenger_name + ".txt");
        file_output.open("Passengers/orderHistory/" + _passenger_name + ".txt");
        while(getline(file_input,line)){
            file_output<<line<<endl;
        }
        file_output<<to_string()<<endl;
        file_input.close();
        file_output.close();
    }
};



#endif //UNTITLED3_ORDER_H
