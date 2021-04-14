#ifndef UNTITLED3_ADDRESS_H
#define UNTITLED3_ADDRESS_H

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


using namespace std;

string int_to_string(int x){
    string res_ = "", res = "";
    while(x > 0){
        res_ += (char)(x%10 + (int)('0'));
        x/=10;
    }
    for(int i = res_.size()-1;i>=0;i--){
        res+=res_[i];
    }
    if(res.size() == 0){
        res = "0";
    }
    return res;
}
int string_to_int(string data){
    int res = 0;
    for(char i : data){
        res*=10;
        res+=i - '0';
    }
    return res;
}
inline bool file_exists(string file_name) {
    ifstream f(file_name.c_str());
    return f.good();
}



class Address{
public:
    string _name="";
    int _x = 0, _y = 0;
    Address(int x, int y, string name = ""){
        _x = x;
        _y = y;
        _name = name;
    }
    Address(){}
    string get_position(){
        return "Position("+int_to_string(_x)+","+int_to_string(_y)+")";
    }
    int get_distance(Address to){
        int x_ = _x- to._x, y_ = _y - to._y;
        x_ *= x_;
        y_ *= y_;
        return sqrt(x_ + y_);
    }
    string get_address(){
        return _name;
    }
    static Address from_string(string data){
        string name = "", res = "";
        string name_ = "";
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
                    x = string_to_int(res);
                }else if(name == "y"){
                    y = string_to_int(res);
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
    string to_string(){
        return "{'name': "+_name+", 'x': " + int_to_string(_x) + ", 'y': " + int_to_string(_y) + "}";
    }
    int get_x(){
        return _x;
    }
    int get_y(){
        return _y;
    }
    int set_y(int y){
        _y = y;
    }
    int set_x(int x){
        _x = x;
    }
};


#endif //UNTITLED3_ADDRESS_H
