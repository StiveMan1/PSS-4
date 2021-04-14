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
    string _model = "", _number = "", _color = "";
    int _carType = 1, _freeBottleOfwater = 0;
public:
    Car(){}
    Car(string model, string number, string color, int carType, int freeBottleOfwater = 0, Address coordinates = Address()){
        _model = model;
        _number = number;
        _color = color;
        _carType = carType;
        _freeBottleOfwater = freeBottleOfwater;
    }

    void parkRightInFrontOfTheEntrance(){
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
    int get_car_type(){
        return _carType;
    }
    static Car from_string(string data){
        string name = "", res = "", model_ = "", number_ = "", color_ = "";
        int carType_, freeBottleOfwater_;
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
                    carType_ = string_to_int(res);
                }else if(name == "freeBottleOfwater"){
                    freeBottleOfwater_ = string_to_int(res);
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
        return Car(model_, number_, color_, carType_, freeBottleOfwater_);
    }
    string to_string(){
        return "{'model': "+_model+", 'number': " + _number + ", 'color': " + _color + ", 'carType': " + int_to_string(_carType) + ", 'freeBottleOfwater': " + int_to_string(_freeBottleOfwater) + "}";
    }
    void add_bottles(int count){
        _freeBottleOfwater += count;
    }
    int get_free_bottles(){
        return _freeBottleOfwater;
    }
};


#endif //UNTITLED3_CAR_H
