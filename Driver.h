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
#include "Payment.h"
#include "Car.h"


using namespace std;

class Driver{
private:
    string _name, _password;
    double _rating = 0.0;
    int _rate_count = 0, _status = 0;
    bool _blocked = false;
    Car _car;
    Address _coordinates;
public:
    Driver()= default;
    Driver(string name, string password, double rating = 0.0, int rate_count = 0, Car car = Car(), int status = 0,
           Address coordinates = Address(), bool blocked = false){
        _name = std::move(name);
        _password = std::move(password);
        _rating = rating;
        _rate_count = rate_count;
        _car = std::move(car);
        _status = status;
        _coordinates = std::move(coordinates);
        _blocked = blocked;
    }
    Address get_address(){
        return _coordinates;
    }
    string get_position(){
        return _coordinates.get_position();
    }
    void set_address(Address address){
        _coordinates = std::move(address);
        save();
    }
    string get_name(){
        return _name;
    }
    double get_rating() const{
        return _rating;
    }
    Car get_car(){
        return _car;
    }
    void rate_driver(double rate){
        _rating = _rating*_rate_count + rate;
        _rate_count ++;
        _rating /= _rate_count;
        save();
    }
    void save(){
        ofstream file("Drivers/Users/" + _name+ ".txt");
        file<<to_string();
        file.close();
        if(!file_exists("Drivers/orderHistory/" + _name+ ".txt")){
            file.open("Drivers/orderHistory/" + _name+ ".txt");
            file.close();
        }
        if(!file_exists("Drivers/AcceptedCars/" + _name+ ".txt")){
            file.open("Drivers/AcceptedCars/" + _name+ ".txt");
            file.close();
        }
    }
    static Driver get_driver(const string& name){
        ifstream file;
        file.open ("Drivers/Users/" + name+ ".txt");
        string data;
        getline(file, data);
        file.close();
        return from_string(data);
    }
    bool check_password(const string& password){
        return password == _password;
    }
    static Driver from_string(string data){
        string name, res, name_, password_;
        bool tor = false, blocked_ = false;
        double rating_ = 0.0;
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
                    rate_count_ = std::stoi(res);
                }else if(name == "car"){
                    car_ = Car::from_string(res);
                }else if(name == "password"){
                    password_ = res;
                }else if(name == "status"){
                    status_ = std::stoi(res);
                }else if(name == "coordinates"){
                    coordinates_ = Address::from_string(res);
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
        return Driver(name_, password_, rating_, rate_count_, car_, status_, coordinates_, blocked_);
    }
    string to_string(){
        return "{'name': "+_name+", 'password': " + _password +", 'rating': " + std::to_string(_rating) + ", 'rate_count': "
        + std::to_string(_rate_count) + ", 'car': " + _car.to_string() + ", 'status': " + std::to_string(_status) +
        ", 'blocked': " + std::to_string(_blocked) + "}";
    }
    void set_status(int status){
        _status = status;
        save();
    }
    int get_status() const{
        return _status;
    }
    vector<Car> get_accepted_cars(){
        vector<Car> res;
        ifstream file;
        if(!file_exists("Drivers/AcceptedCars/" + _name+ ".txt")){
            return res;
        }
        file.open ("Drivers/AcceptedCars/" + _name+ ".txt");
        string data;
        while(getline(file, data)){
            res.push_back(Car::from_string(data));
        }
        file.close();
        return res;
    }
    void add_car(Car car){
        string none_active_car, line;
        ifstream file_in;
        file_in.open ("Drivers/Cars.txt");
        while(getline(file_in, line)){
            none_active_car += line + '\n';
        }
        file_in.close();
        none_active_car += "{'driver': "+_name + ", 'car': " + car.to_string() + "}\n";

        ofstream file_out;
        file_out.open ("Drivers/Cars.txt");
        file_out<<none_active_car;
        file_out.close();
    }
    void set_current_car(Car car){
        _car = std::move(car);
        save();
    }
    void add_bottles(int count){
        _car.add_bottles(count);
        save();
    }
    bool is_blocked() const {
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


#endif //UNTITLED3_DRIVER_H
