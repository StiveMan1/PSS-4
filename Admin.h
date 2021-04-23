//
// Created by 04024 on 23.04.2021.
//

#ifndef UNTITLED3_ADMIN_H
#define UNTITLED3_ADMIN_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Admin {
private:
    string _name, _password;
public:
    Admin()= default;;
    Admin(string name, string password){
        _name = std::move(name);
        _password = std::move(password);
    }
    bool check_password(const string& password){
        return password == _password;
    }
    static Admin from_string(string data){
        string name, res, name_, password_;
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
                if(name == "name"){
                    name_ = res;
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
        return Admin(name_, password_);
    }
    string to_string(){
        return "{'name': " + _name + ", 'password': " + _password + "}";
    }

    void save(){
        ofstream file("Admin/Users/" + _name+ ".txt");
        file<<to_string();
        file.close();
    }

    static Admin get_admin(const string& name){
        ifstream file;
        file.open ("Admin/Users/" + name+ ".txt");
        string data;
        getline(file, data);
        return from_string(data);
    }
    string get_name(){
        return _name;
    }
};


#endif //UNTITLED3_ADMIN_H
