//
// Created by 04024 on 23.04.2021.
//

#ifndef UNTITLED3_ADMINGATEWAY_H
#define UNTITLED3_ADMINGATEWAY_H

#include <iostream>
#include <string>
#include "Admin.h"
#include "Payment.h"
#include "Passenger.h"
#include "Driver.h"
using namespace std;

class AdminGateway {
private:
    Admin _current_admin;
    bool in = false;
    static string get_accept_cars_driver(string data){
        string name, res, name_;
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
        return name_;
    }
    static Car get_accept_cars_car(string data){
        string name, res;
        bool tor = false;
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
                if(name == "car"){
                    car_ = Car::from_string(res);
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
        return car_;

    }
public:
    static void SingUp(){
        string name, password;
        cout<<"User Name : ";
        cin>>name;
        cout<<"Password : ";
        cin>>password;
        if(!file_exists("Admin/Users/" + name+ ".txt")){
            Admin(name, password).save();
        }
        cout<<"Registered\n";
    }
    string SingIn(){
        if(in){
            cout<<"You already in\n";
            return _current_admin.get_name();
        }
        string name, password;
        cout<<"User Name : ";
        cin>>name;
        cout<<"Password : ";
        cin>>password;
        bool tor = false;
        if(file_exists("Drivers/Users/" + name+ ".txt")){
            Admin x = Admin::get_admin(name);
            if(x.check_password(password)){
                tor = true;
                _current_admin = x;
            }
        }
        if(tor){
            cout<<"Login\n";
            in = true;
            return name;
        }else{
            cout<<"Error\n";
            return "";
        }
    }
    void Out(){
        in = false;
    }
    void show_non_active_cars(){
        if(!in) return;
        string line;
        ifstream file_in;
        file_in.open ("Drivers/Cars.txt");
        while(getline(file_in, line)){
            cout<<line<<endl;
        }
    }
    void accept_cars(){
        if(!in) return;
        string line, none_accepted,input;
        ifstream file_in;
        file_in.open ("Drivers/Cars.txt");
        while(getline(file_in, line)){
            string driver_name = get_accept_cars_driver(line);
            Car car = get_accept_cars_car(line);
            cout<<"Driver : "<<driver_name<<endl;
            car.print();
            cout<<"Accept/decline(A/d)";
            cin>>input;
            if(input[0] == 'A' || input[0] == 'a'){
                string driver_cars;
                ifstream file_input("Drivers/AcceptedCars/" + driver_name+ ".txt");
                string data;
                while(getline(file_input, data)){
                    driver_cars+=data+'\n';
                }
                driver_cars+=car.to_string()+'\n';
                file_input.close();
                ofstream file_output("Drivers/AcceptedCars/" + driver_name+ ".txt");
                file_output<<driver_cars;
                file_output.close();
            }else{
                none_accepted += line;
            }
        }
    }
    void block_passenger(){
        if(!in) return;
        string name;
        int type;
        cout<<"Enter user name : ";
        cin>>name;
        cout<<"Block/Un block(1- Block, 2 - un block) : ";
        cin>>type;
        if(type == 1){
            Passenger::get_passenger(name).block();
        }else if(type == 2){
            Passenger::get_passenger(name).unblock();
        }
    }
    void block_driver(){
        if(!in) return;
        string name;
        int type;
        cout<<"Enter user name : ";
        cin>>name;
        cout<<"Block/Un block(1- Block, 2 - un block) : ";
        cin>>type;
        if(type == 1){
            Driver::get_driver(name).block();
        }else if(type == 2){
            Driver::get_driver(name).unblock();
        }
    }
};


#endif //UNTITLED3_ADMINGATEWAY_H
