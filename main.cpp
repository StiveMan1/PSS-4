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
    PassengerGateway x;
    string name;
    while(true){
        cout<<name<<"& ";
        cin>> input;
        if(input == "exit" || input == "out"){
            return 1;
        }else if(input == "login"){
            name = x.SingIn();
        }else if(input == "register"){
            PassengerGateway::SingUp();
        }else if(input == "info"){
            x.info();
        }else if(input == "set_payment"){
            x.set_payment();
        }else if(input == "set_pinned_address"){
            x.set_pinned_address();
        }else if(input == "order_driver"){
            x.order_driver();
        }else if(input == "show_history"){
            x.show_history();
        }else if(input == "show_position"){
            x.show_position();
        }else if(input == "show_order"){
            x.show_order();
        }else if(input == "rate_driver"){
            x.rate_driver();
        }else if(input == "LogOut"){
            x.LogOut();
        }
    }
}