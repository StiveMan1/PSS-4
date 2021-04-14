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
    Car _current_car;
    Order _current_order;
public:
    void info(){
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
        int type;
        cout<<"Coose payment type:\n1 - Cash\n2 - Card\nWrite type : ";
        cin >> type;
        string numbers = "", date = "";
        if(type == 2){
            cout<<"Card Number : ";
            cin>>numbers;
            cout<<"Card date : ";
            cin>>date;
        }
        _current_user.set_payment(Payment(type == 2, numbers, date));
    }
    void order_driver(){
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
        sum *= 1 + float(car_type)/4;
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
        _current_order = order;
        // add in oredr to driver select
    }
    void show_history(){
        ifstream file("Passengers/orderHistory/" + _current_user.get_name() + ".txt");
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
    void show_position(){
    }
    void SingUp(){
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
        string name, password;
        cout<<"User Name : ";
        cin>>name;
        cout<<"Password : ";
        cin>>password;
        bool tor = false;
        if(file_exists("Passengers/Users/" + name+ ".txt")){
            Passenger x = Passenger().get_passenger(name);
            if(x.check_password(password)){
                tor = true;
                _current_user = x;
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
};


#endif //UNTITLED3_PASSENGERGATEWAY_H
