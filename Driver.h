#ifndef UNTITLED3_DRIVER_H
#define UNTITLED3_DRIVER_H

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
#include "Car.h"


using namespace std;

class Driver{
private:
    string _name, _password;
    float _rating = 0.0;
    int _rate_count = 0, _status = 0;
    Car _car;
    Address _coordinates;
public:
    Driver(){}
    Driver(string name, string password, float rating = 0.0, int rate_count = 0, Car car = Car(), int status = 0, Address coordinates = Address()){
        _name = name;
        _password = password;
        _rating = rating;
        _rate_count = rate_count;
        _car = car;
        _status = status;
        _coordinates = coordinates;
    }
    Address get_address(){
        return _coordinates;
    }
    string get_position(){
        return _coordinates.get_position();
    }
    void set_address(Address address){
        _coordinates = address;
        save();
    }
    string get_name(){
        return _name;
    }
    float get_rating(){
        return _rating;
    }
    Car get_car(){
        return _car;
    }
    void rate_driver(float rait){
        _rating = _rating*_rate_count + rait;
        _rate_count ++;
        _rating /= _rate_count;
        save();
    }
    void save(){
        ofstream file("Drivers/Users/" + _name+ ".txt");
        file<<to_string();
        file.close();
        if(!file_exists("Drivers/orderHistory/" + _name+ ".txt")){
            ofstream file("Drivers/orderHistory/" + _name+ ".txt");
            file.close();
        }
    }
    static Driver get_driver(string name){
        ifstream file;
        file.open ("Drivers/Users/" + name+ ".txt");
        string data;
        getline(file, data);
        return from_string(data);
    }
    bool check_password(string password){
        return password == _password;
    }
    static Driver from_string(string data){
        string name = "", res = "", name_ = "", password_ = "";
        bool tor = false;
        float rating_ = 0.0;
        int rate_count_ = 0, status_ = 0;
        Address coordinates_;
        Car car_;
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
                }else if(name == "car"){
                    car_ = Car::from_string(res);
                }else if(name == "password"){
                    password_ = res;
                }else if(name == "status"){
                    status_ = string_to_int(res);
                }else if(name == "coordinates"){
                    coordinates_ = Address::from_string(res);
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
        return Driver(name_, password_, rating_, rate_count_, car_, status_, coordinates_);
    }
    string to_string(){
        return "{'name': "+_name+", 'password': " + _password +", 'rating': " + std::to_string(_rating) + ", 'rate_count': " + std::to_string(_rate_count) + ", 'car': " + _car.to_string() + ", 'status': " + int_to_string(_status) + "}";
    }
    void set_status(int status){
        _status = status;
        save();
    }
    int get_satus(){
        return _status;
    }
    void set_car(Car car){
        _car = car;
        save();
    }
    void add_bottles(int count){
        _car.add_bottles(count);
        save();
    }
};


#endif //UNTITLED3_DRIVER_H
