#ifndef UNTITLED3_CAR_H
#define UNTITLED3_CAR_H

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


using namespace std;

class Car{
private:
    string _model, _number, _color;
    int _carType = 1, _freeBottleOfWater = 0;
public:
    Car()= default;
    Car(string model, string number, string color, int carType, int freeBottleOfWater = 0, const Address& coordinates = Address()){
        _model = std::move(model);
        _number = std::move(number);
        _color = std::move(color);
        _carType = carType;
        _freeBottleOfWater = freeBottleOfWater;
    }

    void parkRightInFrontOfTheEntrance() const{
        if(_carType == 4){

        }
    }
    string get_car_model(){
        return _model;
    }
    string get_car_color(){
        return _color;
    }
    string get_car_number(){
        return _number;
    }
    int get_car_type() const{
        return _carType;
    }
    static Car from_string(string data){
        string name, res, model_, number_, color_;
        int carType_, freeBottleOfWater_;
        bool tor = false;
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
                if(name == "model"){
                    model_ = res;
                }else if(name == "number"){
                    number_ = res;
                }else if(name == "color"){
                    color_ = res;
                }else if(name == "carType"){
                    carType_ = std::stoi(res);
                }else if(name == "freeBottleOfWater"){
                    freeBottleOfWater_ = std::stoi(res);
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
        return Car(model_, number_, color_, carType_, freeBottleOfWater_);
    }
    string to_string(){
        return "{'model': "+_model+", 'number': " + _number + ", 'color': " + _color + ", 'carType': " + std::to_string(_carType) + ", 'freeBottleOfWater': " + std::to_string(_freeBottleOfWater) + "}";
    }
    void add_bottles(int count){
        _freeBottleOfWater += count;
    }
    int get_free_bottles() const {
        return _freeBottleOfWater;
    }
    void print(){
        cout<<"Car model : "<<get_car_model()<<endl;
        cout<<"Car number : "<<get_car_number()<<endl;
        cout<<"Car color : "<<get_car_color()<<endl;
        cout<<"Car Type : ";
        if(get_car_type() == 1){
            cout<<"Economy\n";
        }else if(get_car_type() == 2){
            cout<<"Comfort\n";
        }else if(get_car_type() == 3){
            cout<<"ComfortPlus\n";
        }else if(get_car_type() == 4){
            cout<<"Business\n";
        }
        if(get_car_type() > 1){
            cout<<"Free Bottle : "<<get_free_bottles()<<endl;
        }
    }
};


#endif //UNTITLED3_CAR_H
