#include <iostream>
#include <unistd.h>
#include <string>
#include "Address.h"
#include "Car.h"
#include "Driver.h"
#include "Order.h"
#include "Passenger.h"
#include "DriverGateway.h"
#include "AdminGateway.h"

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
    AdminGateway x;
    string name;
    while(true){
        cout<<name<<"& ";
        cin>> input;
        if(input == "exit" || input == "out"){
            return 1;
        }else if(input == "login"){
            name = x.SingIn();
        }else if(input == "register"){
            AdminGateway::SingUp();
        }else if(input == "show_active_orders"){
            DriverGateway::show_active_orders();
        }else if(input == "show_non_active_cars"){
            x.show_non_active_cars();
        }else if(input == "accept_cars"){
            x.accept_cars();
        }else if(input == "block_passenger"){
            x.block_passenger();
        }else if(input == "block_driver"){
            x.block_driver();
        }else if(input == "LogOut"){
            x.LogOut();
        }
    }
}