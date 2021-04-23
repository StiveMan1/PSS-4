#ifndef UNTITLED3_ADDRESS_H
#define UNTITLED3_ADDRESS_H

#include <iostream>
#include <utility>
#include <thread>
#include <fstream>
#include <chrono>
#include<string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <queue>
#include <cmath>


using namespace std;

class Address{
public:
    string _name;
    int _x = 0, _y = 0;
    Address(int x, int y, string name = ""){
        _x = x;
        _y = y;
        _name = std::move(name);
    }
    Address()= default;
    string get_position() const{
        return "Position("+::to_string(_x)+","+::to_string(_y)+")";
    }
    int get_distance(const Address& to) const{
        int x_ = _x- to._x, y_ = _y - to._y;
        x_ *= x_;
        y_ *= y_;
        return sqrt(x_ + y_);
    }
    string get_address() const{
        return _name;
    }
    static Address from_string(string data){
        string name, res;
        string name_;
        int x, y;
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
            }else if(data[i] == ',' || data[i] == '}'){
                if(name == "name"){
                    name_ = res;
                }else if(name == "x"){
                    x = std::stoi(res);
                }else if(name == "y"){
                    y = std::stoi(res);
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
        return Address(x,y,name_);
    }
    string to_string() const{
        return "{'name': "+_name+", 'x': " + ::to_string(_x) + ", 'y': " + ::to_string(_y) + "}";
    }
    int get_x() const{
        return _x;
    }
    int get_y() const{
        return _y;
    }
    void set_y(int y){
        _y = y;
    }
    void set_x(int x){
        _x = x;
    }
};


#endif //UNTITLED3_ADDRESS_H
