#include <iostream>
#include <unistd.h>
#include <string>
#include "Address.h"
#include "Car.h"
#include "Driver.h"
#include "Order.h"
#include "Passenger.h"
#include "PassengerGateway.h"
#include "DriverGateway.h"

using namespace std;


int main(){
    mkdir("Passengers");
    mkdir("Passengers/orderHistory");
    mkdir("Passengers/Users");
    mkdir("Passengers/Accepted");
    mkdir("Drivers");
    mkdir("Drivers/orderHistory");
    mkdir("Drivers/Users");
    mkdir("Drivers/AcceptedCars");
    string input;
    DriverGateway x;
    string name;
    while(true){
        cout<<name<<"& ";
        cin>> input;
        if(input == "exit" || input == "out"){
            return 1;
        }else if(input == "login"){
            name = x.SingIn();
        }else if(input == "register"){
            DriverGateway::SingUp();
        }else if(input == "info"){
            x.info();
        }else if(input == "show_active_orders"){
            DriverGateway::show_active_orders();
        }else if(input == "set_status"){
            x.set_status();
        }else if(input == "select_order"){
            x.select_order();
        }else if(input == "show_history"){
            x.show_history();
        }else if(input == "add_bottles"){
            x.add_bottles();
        }else if(input == "rate_user"){
            x.rate_user();
        }else if(input == "add_car"){
            x.add_car();
        }else if(input == "LogOut"){
            x.LogOut();
        }else if(input == "choose_current_car"){
            x.choose_current_car();
        }
    }
}