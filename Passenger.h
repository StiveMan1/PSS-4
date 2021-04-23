#ifndef UNTITLED3_PASSENGER_H
#define UNTITLED3_PASSENGER_H

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
#include "Order.h"


using namespace std;

class Passenger{
private:
    string _name, _password;
    double _rating = 0.0;
    int _rate_count = 0;
    bool _blocked = false;
    Address _pinned;
    Payment _method;
public:
    Passenger(string name,string password,double rating = 0.0,int rate_count = 0,Address pinned = Address(),
              Payment method = Payment(), bool blocked = false){
        _name = std::move(name);
        _rating = rating;
        _rate_count = rate_count;
        _pinned = std::move(pinned);
        _method = std::move(method);
        _password = std::move(password);
        _blocked = blocked;
    }
    Passenger()= default;
    string get_name(){
        return _name;
    }
    double get_rating() const{
        return _rating;
    }
    string get_pinned(){
        return _pinned.get_address();
    }
    string payment_type() const{
        if(_method._is_card){
            return "Card";
        }else{
            return "Сash";
        }
    }
    Payment get_method(){
        return _method;
    }
    Address get_pinned_address(){
        return _pinned;
    }
    void set_pinned_address(Address my_address){
        _pinned = std::move(my_address);
        save();
    }
    void set_payment(Payment my_method){
        cout<<my_method.to_string()<<endl;
        _method = my_method;
        save();
    }
    void rate_user(double rait){
        _rating = _rating*_rate_count + rait;
        _rate_count ++;
        _rating /= _rate_count;
        save();
    }
    static Passenger from_string(string data){
        string name, res, name_, password_;
        bool tor = false, blocked_ = false;
        double rating_ = 0.0;
        int rate_count_ = 0;
        Address pinned_;
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
                if(name == "name"){
                    name_ = res;
                }else if(name == "rating"){
                    rating_ = std::stod(res);
                }else if(name == "rate_count"){
                    rate_count_ = std::stoi(res);
                }else if(name == "pinned"){
                    pinned_ = Address::from_string(res);
                }else if(name == "method"){
                    method_ = Payment::from_string(res);
                }else if(name == "password"){
                    password_ = res;
                }else if(name == "blocked"){
                    blocked_ = std::stoi(res);
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
        return Passenger(name_, password_, rating_, rate_count_, pinned_, method_, blocked_);
    }
    string to_string(){
        return "{'name': "+_name+", 'password': " + _password +", 'rating': " + std::to_string(_rating) + ", 'rate_count': "
        + std::to_string(_rate_count) + ", 'pinned': " + _pinned.to_string() + ", 'method': " + _method.to_string() +
        ", 'blocked': " + std::to_string(_blocked) + "}";
    }
    void save(){
        ofstream file("Passengers/Users/" + _name+ ".txt");
        file<<to_string();
        file.close();
        if(!file_exists("Passengers/orderHistory/" + _name+ ".txt")){
            file.open("Passengers/orderHistory/" + _name+ ".txt");
            file.close();
            ofstream file1("Passengers/Accepted/" + _name+ ".txt");
            file1.close();
        }
    }
    static Passenger get_passenger(const string& name){
        ifstream file;
        file.open ("Passengers/Users/" + name+ ".txt");
        string data;
        getline(file, data);
        return from_string(data);
    }
    bool check_password(const string& password){
        return password == _password;
    }
    Order get_active_order(){
        string line;
        ifstream file("Passengers/Accepted/" + _name+ ".txt");
        getline(file, line);
        return Order::from_string(line);
    }
    bool is_blocked() const{
        return _blocked;
    }
    void block(){
        _blocked = true;
        save();
    }
    void unblock(){
        _blocked = false;
        save();
    }
};


#endif //UNTITLED3_PASSENGER_H
