//
//  main.cpp
//  SimpleServer
//
//  Created by Aaron Ng on 5/11/17.
//  Copyright © 2017 admin. All rights reserved.
//


/*
Implementation of a server program and client program. 

 The server will use 1 plus the last four digits of my student ID as the server port number to avoid conflicting with other students’ server program. For example, if the last four digits of my student ID are 3456, then as the server port number is 13456.
 
 In this project, there are multiple clients connecting to the server at the same time. The server can support MAXCLIENTS number of concurrent clients. For the grading purpose, set MAXCLIENTS = 3.
 
 The following commands are implemented at the client side and the server side implements the corresponding  functions required to support these commands. When the server starts, it should first read the user account information from a file.
 
 For grading purpose, the initial user accounts (UserID, Password) are (Tom, Tom11), (David, David22), (Beth, Beth33), and (John, John44).
 
 
1. login UserID Password
The client will send the UserID/Password information to the server. If the server can verify the UserID and the Password, the server will send a confirmation message to the client and inform all other clients that this client joins the chat room; otherwise, the server will decline login and send an error message to the client.
 
 
2. send all message
Send the “message” to the server. The server will precede the message with the UserID of the sender and broadcast the message to all other clients.
 
 
3. send UserID message
Send the “message” to the server. The server will precede the message with the UserID of the sender and unicast the message to the client “UserID”.
 
 
4. who 
List all the clients in the chat room.
 
 
5. logout
Logout from the chat room. Once logout, the connection between the server and client will be closed. The server will inform all other clients that this client left.
 
6. update
Get all of the messages meant for a specific client.
*/


#include "main.hpp"
#define PORT 19214

int main(){
    string myName = "1";
    
    string name, pass, command, newInfo;
    fstream clients, updates;
    
    //populate server with valid users
    User client;
    clients.open("users.txt");
    while(!clients.eof()){
        clients >> client.userID;
        clients >> client.password;
        mainServer.addUser(client);
    }
    clients.close();
    
    
    //create a log to synchronize threads
    log.open("log.txt", fstream::app);
    
    while(command != "stop"){
        
        //list of commands
        cout << "You can type:" << endl<< "'login' using proper credentials" << endl << "'send(to all or another user)'" << endl <<"'logout' to log a client out" << endl << "'who' to see who else is in the chat room" << endl << "'stop' to stop the simulation of the server" << endl << "'update' to get new messages" << endl;
        getline(cin,command);
        
        
        
        
        
        
        //update all variables and synch up all running processes
        updates.open("log.txt");
        while(!updates.eof()){
            getline(updates, newInfo);
            string temp;
            
            //if somebody has been logged in
            if(newInfo[3] == 'i'){
                for (int i=6; i < newInfo.length(); i++){
                    temp = temp + newInfo[i];
                }
                mainServer.update("login", temp);
            }
            
            //if somebody has been logged out
            else if(newInfo[3] == 'o'){
                for (int i=7; i < newInfo.length(); i++){
                    temp = temp + newInfo[i];
                }
                mainServer.update("logout", temp);
            }
        }
        updates.close();
        
        
        
        
        
        
        //get new messages from other clients
        if (command == "update"){
            bool got = false;
            updates.open("log.txt");
            cout << endl;
            while(!updates.eof()){
                getline(updates, newInfo);
                
                string sender, destination, message;
                
                
                if(newInfo[0] == 'M'){
                    int i = 8;
                    
                    //parse
                    for (; newInfo[i] != ' '; i++){
                        sender = sender + newInfo[i];
                    }
                    i++;
                    for (; newInfo[i] != ' '; i++){
                        destination = destination + newInfo[i];
                    }
                    i++;
                    for (; i<newInfo.length(); i++){
                        message = message + newInfo[i];
                    }
                    
                    
                    //If the current client is receiving a new message
                    if(destination == myName){
                        cout << ">From " << sender << ": " << message << endl;
                        got = true;
                    }
                    
                }
                
            }
            if(got == false){
                cout << "No new messages" << endl;
            }
            cout << endl;

            
            
            updates.close();
        }
        
        
        
        
        
        
        //login a client
        else if(command == "login"){
            if(myName == "1"){
                login(myName, 19214);
            }
            else{
                cout << "You are already logged in as " << myName << endl << endl;
            }
        }
        
        
        
        
        
        
        //logout a client
        else if(command == "logout"){
            if(myName != "1"){
                loggingOut(myName);
                myName = "1";
            }
            else{
                cout << "You are not logged in" << endl << endl;
            }
        }

        
        
        
        
        //parse and detect command 'send'
        else if(command[0] == 's' && command[1] == 'e' && command[2] == 'n' && command[3] == 'd' && command[4] == ' '){
            if(myName == "1"){
                cout << "You cannot send a message if you are not logged in" << endl << endl;
            }
            else{
                string destination, message;
                int i = 5;
                
                //find and separate the destination of the message
                while(command[i] != ' ' && i < command.length()){
                    destination = destination+command[i];
                    i++;
                }
                i++;
                
                //find and separate the message portion
                while(i < command.length()){
                    message = message + command[i];
                    i++;
                }
                
                
                //in case of a broadcast message to 'all'
                if(destination == "all"){
                    mainServer.broadcastMessage(myName, message);
                }
                
                //in case of a direct message to a specific client
                else if(mainServer.find(destination)){
                    findSendOnline(myName, destination, message);
                }
                
                //if the specified client is unrecognized
                else{
                    cout << "The designated receiver of the message was unrecognized";
                }
                
                cout << endl << endl;
            }
            
        }
        
        
        
        
        
        //who is online
        else if(command == "who"){
            findOnline(myName);
        }
        
        
        
        
        //stop
        else if (command != "stop"){
            cout << "Please enter a proper command" << endl << endl;
        }
        
        
    }
    
    //logout the user if they terminate the program properly
    if(myName != "1"){
        loggingOut(myName);
        myName = "1";
    }
    
    log.close();
    
    return 0;
}
