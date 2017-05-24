//
//  main.h
//  SimpleServer
//
//  Created by Aaron Ng on 5/11/17.
//  Copyright © 2017 admin. All rights reserved.
//

#include "objects.hpp"
#define PORT 19214
//#include "socket.h"

Server mainServer;



void login(string &x, int port){
    
    if(port != mainServer.getPortNumber()){
        cout << "Improper port" << endl;
        return;
    }
    
    //check current server amount of people
    if(mainServer.checkServerCapacity()){
        string userID, password;
        cout << "Please enter your username:" << endl;
        getline(cin, userID);
        
        cout << "Please enter your password" << endl;
        getline(cin,password);
        
        
        int a=-1,b=-1,c=-1;
        mainServer.onlineUsers(a, b, c);
        
        //check if input is valid
        if(mainServer.getClient(a) == userID || mainServer.getClient(b) == userID || mainServer.getClient(c) == userID){
            cout << userID << " is already online";
        }
        else if(mainServer.checkInClient(userID, password, x)){
            cout << "Logged in as " << userID;
        }
        else{
            //if user ID or password were invalid
            cout << "FAILURE" << endl << "Your user ID was incorrect, your password was incorrect";
        }
    }
    cout << endl << endl;
}



void loggingOut(string temp){
    mainServer.leavingClient(temp);
}



void findOnline(string x){
    int a=-1, b=-1, c=-1;
    mainServer.onlineUsers(a, b, c);
    
    //default to -1 if there is no user online
    if(mainServer.getClient(a) != x && a!=-1){
        cout << mainServer.getClient(a) << " is online." << endl;
    }
    if(mainServer.getClient(b) != x && b!=-1){
        cout << mainServer.getClient(b) << " is online."<< endl;
    }
    if(mainServer.getClient(c) != x && c!=-1){
        cout << mainServer.getClient(c) << " is online."<< endl;
    }
    if(mainServer.getClient(a) == x || mainServer.getClient(b) == x || mainServer.getClient(c) == x){
        cout << "You are online." << endl;
    }
    if(a==-1 && b==-1 && c==-1){
        cout << "Nobody is online";
    }
    cout << endl;
}





void findSendOnline(string sender, string destination, string message){
    int a=-1, b=-1, c=-1;
    
    //find if specific client is online
    mainServer.onlineUsers(a, b, c);
    if(mainServer.getClient(a) == destination || mainServer.getClient(b) == destination || mainServer.getClient(c) == destination){
        
        //if client is online then send the message
        cout << "Message sent" << endl;
        
        log << "Message " << sender << " " << destination << " " << message << endl;
    }
    //if the specific client is not online
    else{
        cout << "That client is not online";
    }

}




