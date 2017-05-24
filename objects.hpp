//
//  server.h
//  SimpleServer
//
//  Created by Aaron Ng on 5/11/17.
//  Copyright © 2017 admin. All rights reserved.
//

#include <iostream>
#include <vector>
#include <fstream>
#define PORT 19214

using namespace std;

//declare ofstream log file to synchronize threads
ofstream log;


struct User{
    string userID = "";
    string password = "";
    bool online = false;
};



class Server{
public:
    int getPortNumber(){
        return portNumber;
    }
    
    //synchronize
    void update(string action, string name){
        if(action == "login"){
            for(int i=0; i < allUsers.size(); i++){
                if(allUsers[i].userID == name && allUsers[i].online == false){
                    allUsers[i].online = true;
                    currentClients++;
                }
            }
        }
        else{
            for(int i=0; i < allUsers.size(); i++){
                if(allUsers[i].userID == name && allUsers[i].online == true){
                    allUsers[i].online = false;
                    currentClients--;
                }
            }
        }
    }
    
    //validate login of the client
    bool checkInClient(string userID,string password, string &x){
        for(int i=0; i < allUsers.size(); i++){
            if(allUsers[i].userID == userID && allUsers[i].password == password && currentClients<MAXCLIENTS){
                allUsers[i].online = true;
                currentClients++;
                log << "login " << allUsers[i].userID << endl;
                x = allUsers[i].userID;
                return true;
            }
        }
        return false;
    }
    
    
    
    //check the number of online clients
    bool checkServerCapacity(){
        if(currentClients < MAXCLIENTS) return true;
        cout << "Server is full" << endl;
        return false;
    }
    
    
    
    //logging out a client from the server
    void leavingClient(string userID){
        
        //iterate through the possible users
        for(int i=0; i <= allUsers.size(); i++){
            
            //if there is no matching user
            if(i == allUsers.size()){
                cout << "No such client exists";
            }
            else if(allUsers[i].userID == userID){
                
                //find if the client is "online"
                if(allUsers[i].online){
                    
                    //log the client out
                    allUsers[i].online = false;
                    broadcastMessage("Server", userID + " has logged off.");
                    currentClients--;
                    log << "logout " << allUsers[i].userID << endl;
                    break;
                }
            }
        }
        cout << endl << endl;
    }
    
    
    
    //verify if the client is on the list of possible users
    bool find(string name){
        for (int i = 0; i < allUsers.size(); i++){
            if(name == allUsers[i].userID) return true;
        }
        
        return false;
    }
    
    
    
    //send a broadcast to all online clients
    void broadcastMessage(string sender, string message){
        for (int i = 0; i < allUsers.size(); i++){
            if(allUsers[i].online && allUsers[i].userID != sender){
                log << "Message " << sender << " " << allUsers[i].userID << " " << message << endl;
            }
        }
    }
    
    
    
    //"who" function: find all online clients
    void onlineUsers(int &a,int &b, int &c){
        for( int i = 0; i < allUsers.size(); i++){
            
            if(allUsers[i].online){
                //find atmost three online clients
                if(a == -1){
                    a = i;
                }
                else if(b == -1){
                    b = i;
                }
                else if(c == -1){
                    c = i;
                }
                
            }
        }
    }
    
    
    //find a specific client
    string getClient(int x){
        return allUsers[x].userID;
    }
    
    
    
    //add client to a "white list"
    void addUser(User x){
        allUsers.push_back(x);
    }
    
    
    
private:
    vector<User> allUsers;
    const int MAXCLIENTS = 3;
    int currentClients = 0;
    int portNumber = 19214;
    
    //create only instance of the server
};
