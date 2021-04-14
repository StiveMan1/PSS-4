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


using namespace std;

class Passenger{
private:
    string _name, _password;
    float _rating = 0.0;
    int _rate_count = 0;
    Address _pinned;
    Payment _method;
public:
    Passenger(string name,string password,float rating = 0.0,int rate_count = 0,Address pinned = Address(), Payment method = Payment()){
        _name = name;
        _rating = rating;
        _rate_count = rate_count;
        _pinned = pinned;
        _method = method;
        _password = password;
    }
    Passenger(){}
    string get_name(){
        return _name;
    }
    float get_rating(){
        return _rating;
    }
    string get_pinned(){
        return _pinned.get_address();
    }
    string payment_type(){
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
        _pinned = my_address;
        save();
    }
    void set_payment(Payment my_method){
        cout<<my_method.to_string()<<endl;
        _method = my_method;
        save();
    }
    void rate_user(float rait){
        _rating = _rating*_rate_count + rait;
        _rate_count ++;
        _rating /= _rate_count;
        save();
    }
    static Passenger from_string(string data){
        string name = "", res = "", name_ = "", password_ = "";
        bool tor = false;
        float rating_ = 0.0;
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
                    rate_count_ = string_to_int(res);
                }else if(name == "pinned"){
                    pinned_ = Address::from_string(res);
                }else if(name == "method"){
                    method_ = Payment::from_string(res);
                }else if(name == "password"){
                    password_ = res;
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
        return Passenger(name_, password_, rating_, rate_count_, pinned_, method_);
    }
    string to_string(){
        return "{'name': "+_name+", 'password': " + _password +", 'rating': " + std::to_string(_rating) + ", 'rate_count': " + std::to_string(_rate_count) + ", 'pinned': " + _pinned.to_string() + ", 'method': " + _method.to_string() + "}";
    }
    void save(){
        ofstream file("Passengers/Users/" + _name+ ".txt");
        file<<to_string();
        file.close();
        if(!file_exists("Passengers/orderHistory/" + _name+ ".txt")){
            ofstream file("Passengers/orderHistory/" + _name+ ".txt");
            file.close();
            ofstream file1("Passengers/Accepted/" + _name+ ".txt");
            file1.close();
        }
    }
    static Passenger get_passenger(string name){
        ifstream file;
        file.open ("Passengers/Users/" + name+ ".txt");
        string data;
        getline(file, data);
        return from_string(data);
    }
    bool check_password(string password){
        return password == _password;
    }
};


#endif //UNTITLED3_PASSENGER_H
