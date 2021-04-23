#ifndef UNTITLED3_PASSENGERGATEWAY_H
#define UNTITLED3_PASSENGERGATEWAY_H

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

class PassengerGateway{
private:
    Passenger _current_user;
    bool in = false;
public:
    void info(){
        if(!in){
            cout<<"You need to sing in\n";
            return;
        }
        cout<<"User Name : " << _current_user.get_name()<<endl;
        cout<<"Pind Address : " << _current_user.get_pinned()<<endl;
        cout<<"Rating : " << _current_user.get_rating()<<endl;
        Payment method = _current_user.get_method();
        cout<<"Payment Type : " << _current_user.payment_type()<<endl;
        if(method._is_card){
            cout<<"Card : "<<method.get_card_number()<<"\n";
            cout<<"Date : "<<method.get_date()<<"\n";
        }
    }
    void set_pinned_address(){
        if(!in){
            cout<<"You need to sing in\n";
            return;
        }
        string name;
        int x, y;
        cout<<"Address Name : ";
        cin>> name;
        cout<<"X coordinate : ";
        cin>>x;
        cout<<"Y coordinate : ";
        cin>>y;
        _current_user.set_pinned_address(Address(x,y,name));
    }
    void set_payment(){
        if(!in){
            cout<<"You need to sing in\n";
            return;
        }
        int type;
        cout<<"Coose payment type:\n1 - Cash\n2 - Card\nWrite type : ";
        cin >> type;
        string numbers, date;
        if(type == 2){
            cout<<"Card Number : ";
            cin>>numbers;
            cout<<"Card date : ";
            cin>>date;
        }
        _current_user.set_payment(Payment(type == 2, numbers, date));
    }
    void order_driver(){
        if(!in){
            cout<<"You need to sing in\n";
            return;
        }
        if(_current_user.is_blocked()){
            cout<<"You can not make any orders\n";
            return;
        }
        int type, car_type;
        Address addressFrom, addressTo;
        Payment method = _current_user.get_method();
        cout<<"Choose Address From:\n1 - from pinned address\n2 - set address\n Choose type : ";
        cin>> type;
        if(type == 1){
            addressFrom = _current_user.get_pinned_address();
            int x, y;
            cout<<"Second Address Coordinates\n";
            cout<<"X coordinate : ";
            cin>>x;
            cout<<"Y coordinate : ";
            cin>>y;
            addressTo = Address(x, y);
        }else{
            int x, y;
            cout<<"First Address Coordinates\n";
            cout<<"X coordinate : ";
            cin>>x;
            cout<<"Y coordinate : ";
            cin>>y;
            addressFrom = Address(x, y);

            cout<<"Choose Address To:\n1 - from pinned address\n2 - set address\n Choose type : ";
            cin>>type;
            if(type == 1){
                addressTo = _current_user.get_pinned_address();
            }else{
                cout<<"Second Address Coordinates\n";
                cout<<"X coordinate : ";
                cin>>x;
                cout<<"Y coordinate : ";
                cin>>y;
                addressTo = Address(x, y);
            }
        }
        int sum = addressFrom.get_distance(addressTo);
        cout<<"Time : " << sum/10 <<" min\n";
        cout<<"Select type:"<<endl;
        cout<<"1 - Economy "<<sum * 1.25<<endl;
        cout<<"2 - Comfort "<<sum * 1.50<<endl;
        cout<<"3 - ComfortPlus "<<sum * 1.75<<endl;
        cout<<"4 - Business "<<sum * 2<<endl;
        cout<<"Write type : ";
        cin>> car_type;
        sum =  int((double)sum * (1.0 + double(car_type)/4));
        cout<<"Select type:"<<endl;
        cout<<"1 - Use default payment method"<<endl;
        cout<<"2 - Card"<<endl;
        cout<<"3 - Cash"<<endl;
        cout<<"Write type : ";
        cin>>type;
        if(type == 2){
            string numbers, date;
            cout<<"Card Number : ";
            cin>>numbers;
            cout<<"Card date : ";
            cin>>date;
            method = Payment(true, numbers, date);
        }else if(type == 3){
            method = Payment(false, "", "");
        }
        Order order = Order(addressFrom, addressTo, "", _current_user.get_name(), "" , method, sum);
        order.add_active();
        // add in oredr to driver select
    }
    void show_history(){
        if(!in){
            cout<<"You need to sing in\n";
            return;
        }
        ifstream file("Passengers/orderHistory/" + _current_user.get_name() + ".txt");
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
    void show_position(){
        if(!in){
            cout<<"You need to sing in\n";
            return;
        }
        Order order = _current_user.get_active_order();
        if(order.is_picked()){
            cout<<"Position : " + Driver::get_driver(order.get_driver_name()).get_position() + '\n';
        }else{
            cout<<"No active orders\n";
        }
    }
    void show_order(){
        if(!in){
            cout<<"You need to sing in\n";
            return;
        }
        Order order = _current_user.get_active_order();
        if(order.is_picked()){
            order.print();
        }else{
            cout<<"No active orders\n";
        }
    }
    static void SingUp(){
        string name, password;
        cout<<"User Name : ";
        cin>>name;
        cout<<"Password : ";
        cin>>password;
        if(!file_exists("Passengers/Users/" + name+ ".txt")){
            Passenger(name, password).save();
        }
        cout<<"Registered\n";
    }
    string SingIn(){
        if(in){
            cout<<"You already in\n";
            return _current_user.get_name();
        }
        string name, password;
        cout<<"User Name : ";
        cin>>name;
        cout<<"Password : ";
        cin>>password;
        bool tor = false;
        if(file_exists("Passengers/Users/" + name+ ".txt")){
            Passenger x = Passenger::get_passenger(name);
            if(x.check_password(password)){
                tor = true;
                _current_user = x;
            }else{
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
    void rate_driver(){
        if(!in){
            cout<<"You need to sing in\n";
            return;
        }
        int rating;
        string driver_name = _current_user.get_active_order().get_driver_name();
        cout<<"Rating Driver : "<<driver_name<<endl;
        cout<<"Rate from 0 to 10 : ";
        cin>>rating;
        Driver::get_driver(driver_name).rate_driver(rating);
    }
    void LogOut(){
        in = false;
    }
    static void update(PassengerGateway* gateway) {
        while (gateway->in) {
            gateway->_current_user = Passenger::get_passenger(gateway->_current_user.get_name());
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    }
};


#endif //UNTITLED3_PASSENGERGATEWAY_H
