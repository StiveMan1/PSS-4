#ifndef UNTITLED3_DRIVERGATEWAY_H
#define UNTITLED3_DRIVERGATEWAY_H

#include <iostream>
#include <utility>
#include <thread>
#include <thread>
#include <fstream>
#include <chrono>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <queue>
#include "thread"
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
    bool in = false;
public:
    void info(){
        if(!in){
            cout<<"You need to sing in\n";
            return;
        }
        cout<<"User Name : " << _current_driver.get_name()<<endl;
        cout<<"Rating : " << _current_driver.get_rating()<<endl;
        Car car = _current_driver.get_car();
        cout<<"Status : ";
        if(_current_driver.get_status() == 1){
            cout<<"working\n";
        }else if(_current_driver.get_status() == 2){
            cout<<"in ride\n";
        }else{
            cout<<"not working\n";
        }
        cout<<"Position : "<<_current_driver.get_position()<<endl;
        car.print();
    }
    static void show_active_orders(){
        ifstream file("ActiveOrders.txt");
        string line; Order order;
        int i = 1;
        while(getline(file, line)){
            order = Order::from_string(line);
            cout<<"Number "<<i<<endl;
            order.print();
            i++;
            cout<<endl;
        }
    }
    void set_status(){
        if(!in){
            cout<<"You need to sing in\n";
            return;
        }
        int type;
        cout<<"1 - working\n2 - in ride\n 3 - not working\nWrite type : ";
        cin>>type;
        _current_driver.set_status(type);
    }
    static void in_ride(Order order, Driver driver){
        Address now = driver.get_address(), user = order.get_address_from(), to = order.get_address_to();
        int speed = 60;
        while(now.get_x() != user.get_x() || now.get_y() != user.get_y()){
            int x1 = now.get_x() , x2 = user.get_x();
            int x = x1 - x2, _x = min(speed, abs(x));
            if(x > 0){
                x1 -= _x;
            }else{
                x1 += _x;
            }
            int y1 = now.get_y() , y2 = user.get_y();
            int y = y1 - y2, _y = min(speed - _x, abs(y));
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
        order.save_history();
    }
    void select_order(){
        if(!in){
            cout<<"You need to sing in\n";
            return;
        }
        if(_current_driver.is_blocked()){
            cout<<"You can not select any orders\n";
            return;
        }
        ifstream file("ActiveOrders.txt");
        string line, accept, total; Order order;
        int i = 1;
        while(getline(file, line)){
            order = Order::from_string(line);
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
    static void SingUp(){
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
        if(in){
            cout<<"You already in\n";
            return _current_driver.get_name();
        }
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
            }
        }
        if(tor){
            cout<<"Login\n";
            in = true;
            thread my_thread(update, this);
            return name;
        }else{
            cout<<"Error\n";
            return "";
        }
    }
    void add_bottles(){
        if(!in){
            cout<<"You need to sing in\n";
            return;
        }
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
        if(!in){
            cout<<"You need to sing in\n";
            return;
        }
        ifstream file("Drivers/orderHistory/" + _current_driver.get_name() + ".txt");
        string line; Order order;
        int i = 1;
        while(getline(file, line)){
            order = Order::from_string(line);
            cout<<"Number "<<i<<endl;
            order.print();
            i++;
            cout<<endl;
        }
    }
    void rate_user(){
        if(!in){
            cout<<"You need to sing in\n";
            return;
        }
        int rating;
        cout<<"Rating User : "<<last_passenger<<endl;
        cout<<"Rate from 0 to 10 : ";
        cin>>rating;
        Passenger::get_passenger(last_passenger).rate_user(rating);
    }
    void add_car(){
        if(!in){
            cout<<"You need to sing in\n";
            return;
        }
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
        _current_driver.add_car(car);
    }
    void choose_current_car(){
        if(!in){
            cout<<"You need to sing in\n";
            return;
        }
        vector<Car> cars = _current_driver.get_accepted_cars();
        for(int i=0;i<cars.size();i++){
            cout<<"ID "<<i+1<<endl;
            cars[i].print();
        }
        int id;
        cout<<"Enter ID : ";
        cin>>id;
        id--;
        if(id<0 || id>=cars.size()){
            return;
        }
        _current_driver.set_current_car(cars[id]);
    }
    void LogOut(){
        in = false;
    }
    static void update(DriverGateway *gateway) {
        while (gateway->in) {
            gateway->_current_driver = Driver::get_driver(gateway->_current_driver.get_name());
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    }
};


#endif //UNTITLED3_DRIVERGATEWAY_H
