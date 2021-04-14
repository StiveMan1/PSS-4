#ifndef UNTITLED3_DRIVERGATEWAY_H
#define UNTITLED3_DRIVERGATEWAY_H

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
#include "Driver.h"
#include "Order.h"
#include "Passenger.h"

using namespace std;

class DriverGateway{
private:
    Driver _current_driver;
    string last_passenger;
public:
    void info(){
        cout<<"User Name : " << _current_driver.get_name()<<endl;
        cout<<"Rating : " << _current_driver.get_rating()<<endl;
        Car car = _current_driver.get_car();
        cout<<"Status : ";
        if(_current_driver.get_satus() == 1){
            cout<<"working\n";
        }else if(_current_driver.get_satus() == 2){
            cout<<"in ride\n";
        }else{
            cout<<"not working\n";
        }
        cout<<"Position : "<<_current_driver.get_position()<<endl;
        cout<<"Car model : "<<car.get_car_model()<<endl;
        cout<<"Car number : "<<car.get_car_number()<<endl;
        cout<<"Car Type : ";
        if(car.get_car_type() == 1){
            cout<<"Economy\n";
        }else if(car.get_car_type() == 2){
            cout<<"Comfort\n";
        }else if(car.get_car_type() == 3){
            cout<<"ComfortPlus\n";
        }else if(car.get_car_type() == 4){
            cout<<"Business\n";
        }
        if(car.get_car_type() > 1){
            cout<<"Free Bottle : "<<car.get_free_bottles()<<endl;
        }
    }
    void show_active_orders(){
        ifstream file("ActiveOrders.txt");
        string line; Order order;
        int i = 1;
        while(getline(file, line)){
            order = order.from_string(line);
            cout<<"Number "<<i<<endl;
            order.print();
            i++;
            cout<<endl;
        }
    }
    void set_status(){
        int type;
        cout<<"1 - working\n2 - in ride\n 3 - not working\nWrite type : ";
        cin>>type;
        _current_driver.set_status(type);
    }
    static void in_ride(Order order, Driver driver){
        Address now = driver.get_address(), user = order.get_address_from(), to = order.get_address_to();
        int speed = 60;
        while(now.get_x() != user.get_x() || now.get_y() != user.get_y()){
            int horizontal = now.get_x() - user.get_x();
            int x1 = now.get_x() , x2 = user.get_x();
            int x = x1 - x2, _x = min(60, abs(x));
            if(x > 0){
                x1 -= _x;
            }else{
                x1 += _x;
            }
            int y1 = now.get_y() , y2 = user.get_y();
            int y = y1 - y2, _y = min(60 - _x, abs(y));
            if(y > 0){
                y1 -= _y;
            }else{
                y1 += _y;
            }
            now.set_x(x1);
            now.set_y(y1);
            driver.set_address(now);
            std::this_thread::sleep_for(std::chrono::milliseconds(10000));
        }
        while(now.get_x() != to.get_x() || now.get_y() != to.get_y()){
            int horizontal = now.get_x() - to.get_x();
            int x1 = now.get_x() , x2 = to.get_x();
            int x = x1 - x2, _x = min(60, abs(x));
            if(x > 0){
                x1 -= _x;
            }else{
                x1 += _x;
            }
            int y1 = now.get_y() , y2 = to.get_y();
            int y = y1 - y2, _y = min(60 - _x, abs(y));
            if(y > 0){
                y1 -= _y;
            }else{
                y1 += _y;
            }
            now.set_x(x1);
            now.set_y(y1);
            driver.set_address(now);

            std::this_thread::sleep_for(std::chrono::milliseconds(10000));
        }
    }
    void select_order(){
        ifstream file("ActiveOrders.txt");
        string line, accept, total = ""; Order order;
        int i = 1;
        while(getline(file, line)){
            order = order.from_string(line);
            cout<<"Number "<<i<<endl;
            order.print();
            i++;

            cout<<"Accept / Decline (A/d) : ";
            cin>>accept;
            if(accept[0] == 'A' || accept[0] == 'a'){
                while(getline(file, line)){
                    total += line + '\n';
                }
                file.close();
                ofstream file_out("ActiveOrders.txt");
                file_out<<total;
                file_out.close();
                order.accept(_current_driver.get_name(), _current_driver.get_car().get_car_number());
                ofstream file_accepted("Passengers/Accepted/" + order.get_passenger_name()+".txt");
                file_accepted<<order.to_string();
                file_accepted.close();
                _current_driver.set_status(2);
                thread my_thread(in_ride, order, _current_driver);
                last_passenger = order.get_passenger_name();
                return;
            }
            total += line + '\n';
        }
        file.close();
    }
    void SingUp(){
        string name, password;
        cout<<"User Name : ";
        cin>>name;
        cout<<"Password : ";
        cin>>password;
        if(!file_exists("Drivers/Users/" + name+ ".txt")){
            Driver(name, password).save();
        }
        cout<<"Registered\n";
    }
    string SingIn(){
        string name, password;
        cout<<"User Name : ";
        cin>>name;
        cout<<"Password : ";
        cin>>password;
        bool tor = false;
        if(file_exists("Drivers/Users/" + name+ ".txt")){
            Driver x = Driver::get_driver(name);
            if(x.check_password(password)){
                tor = true;
                _current_driver = x;
            }else{
            }
        }
        if(tor){
            cout<<"Login\n";
            return name;
        }else{
            cout<<"Error\n";
            return "";
        }
    }
    void set_car(){
        string model,number,color;
        int carType;
        cout<<"Car model : ";
        cin>>model;
        cout<<"Select type :\n1 - Economy\n2 - Comfort\n3 - ComfortPlus\n4 - Business\nWrite Type : ";
        cin>>carType;
        cout<<"Car number : ";
        cin>>number;
        cout<<"Car color : ";
        cin>>color;
        Car car = Car(model, number, color, carType, 0, Address(0,0));
        _current_driver.set_car(car);
    }
    void add_bottles(){
        if(_current_driver.get_car().get_car_type() > 1){
            cout<<"Enter count : ";
            int count;
            cin >> count;
            _current_driver.add_bottles(count);
        }else{
            cout<<"type less then Comfort can not add free bottles\n";
        }
    }
    void show_history(){
        ifstream file("Drivers/orderHistory/" + _current_driver.get_name() + ".txt");
        string line; Order order;
        int i = 1;
        while(getline(file, line)){
            order = order.from_string(line);
            cout<<"Number "<<i<<endl;
            order.print();
            i++;
            cout<<endl;
        }
    }
    void rate_user(){
        int rating;
        cout<<"Rating User : "<<last_passenger<<endl;
        cout<<"Rate from 0 to 10 : ";
        cin>>rating;
        Passenger::get_passenger(last_passenger).rate_user(rating);
    }
};


#endif //UNTITLED3_DRIVERGATEWAY_H
