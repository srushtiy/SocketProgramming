//
//Full name: Srushti Yadhunath
//Student ID: yadhunat


#include <stdio.h>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <vector>
#include <errno.h>
#include <fstream>
#include <string>
#include <clocale>
#include <cstring>
#include <ctype.h>
#include <sstream>  

using std::endl;
using std::string;
using std::vector;
using std::cout;
using std::size_t;

#define TCPClientPort 25101
#define TCPMonitorPort 26221
#define UDPPORT 24987
#define UDPPortA 21876
#define UDPPortB 22976
#define UDPPortC 23456
#define CONNECTIONS 6
const int BUFFERSIZE = 2048;
const char* LOCALHOST = "127.0.0.1";


int main(){	

	int client, server, monitor;
    struct sockaddr_in server_addr, monitor_addr;
    socklen_t size_client, size_monitor;
    vector<string> dataInput;

    ///////----------------------TCP SOCKET CONNECTION ADAPATED FROM BEEJUS---------------------------------------------/////////////////////

    client = socket(AF_INET, SOCK_STREAM, 0);

    if (client < 0) 
    {
        cout << "\nError establishing client socket..." << endl;
        exit(1);
    }


    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(TCPClientPort);


    int bind_to_client = bind(client, (struct sockaddr*)&server_addr,sizeof(server_addr));
    if (bind_to_client < 0) 
    {
        cout << "=> Error binding connection, the socket has already been established..." << endl;
        return -1;
    }

  //  size_client = sizeof(server_addr);

    listen(client, 1);
    monitor = socket(AF_INET,SOCK_STREAM,0);
    
    if (monitor<0){
       cout<< "Error establishing monitor socket..." <<endl;
    }
    monitor_addr.sin_family = AF_INET;
    monitor_addr.sin_addr.s_addr = htons(INADDR_ANY);
    monitor_addr.sin_port = htons(TCPMonitorPort);

    int bind_to_monitor = bind(monitor, (struct sockaddr*)&monitor_addr,sizeof(monitor_addr));
    if (bind_to_monitor < 0) 
    {
        cout << "=> Error binding connection with monitor, the socket has already been established..." << endl;
        return -1;
    }   

    //size_monitor = sizeof(monitor);
    listen(monitor, 1);
    socklen_t monitor_len = sizeof(monitor_addr);


	cout<<"The AWS server is up and running \n";

    vector<string> serverA_dataout;
    vector<string> serverB_dataout;
    vector<string> serverC_dataout;


    while(true){
        socklen_t len = sizeof(server_addr);
        char *message = new char[BUFFERSIZE];
        int tcp_socket = accept(client,(struct sockaddr *)&server_addr,&len);
        if (tcp_socket<0){
            perror("socket can't be bound to");
            close(tcp_socket);
            return 0;
        }

        do {
            recv(tcp_socket,message,BUFFERSIZE,0);
            dataInput.push_back(message);

        } while(*message != '$');
        dataInput.pop_back();
       
        cout<<dataInput[0]<<" "<<dataInput[1]<<endl;
        string input = dataInput[0];
        string input2 = dataInput[1];

        
        
//////------------____---------------------------UDP SOCKET AND UDP CONNECTION SETUP ADAPTED FROM BEEJUS-------------------------------------///////////////////////////

        int aws_UDP_socket = socket(AF_INET,SOCK_DGRAM,0);
        if (aws_UDP_socket<0)
        {
            perror("AWS UDP socket has failed to be created");
            close(aws_UDP_socket);
            return 0;
        }

        struct sockaddr_in aws_UDP_addr;
        aws_UDP_addr.sin_family = AF_INET;
        aws_UDP_addr.sin_addr.s_addr = inet_addr(LOCALHOST);
        aws_UDP_addr.sin_port = htons(UDPPORT);

        int aws_bind = bind(aws_UDP_socket,(struct sockaddr *)&aws_UDP_addr,sizeof(aws_UDP_addr));
        if (aws_bind<0){
            perror("AWS socket can't be bound to");
            close(aws_UDP_socket);
            return 0;
        }

        cout<<"The AWS is up and running"<<endl;

        int serverA,serverB,serverC;
        struct sockaddr_in UDP_serverA, UDP_serverB, UDP_serverC, aws_client;
        socklen_t serverA_len, serverB_len, serverC_len, aws_client_len;


        UDP_serverA.sin_family = AF_INET;
        UDP_serverA.sin_addr.s_addr = inet_addr(LOCALHOST);
        UDP_serverA.sin_port = htons(UDPPortA);

        UDP_serverB.sin_family = AF_INET;
        UDP_serverB.sin_addr.s_addr = inet_addr(LOCALHOST);
        UDP_serverB.sin_port = htons(UDPPortB);

        UDP_serverC.sin_family = AF_INET;
        UDP_serverC.sin_addr.s_addr = inet_addr(LOCALHOST);
        UDP_serverC.sin_port = htons(UDPPortC);

        char* serverA_output = new char[BUFFERSIZE];
        char* serverB_output = new char[BUFFERSIZE];
        char* serverC_output = new char[BUFFERSIZE];
        

        serverA_len = sizeof(UDP_serverA);
        serverB_len = sizeof(UDP_serverB);
        serverC_len = sizeof(UDP_serverC);
        

        aws_client_len = sizeof(aws_client);

        cout<<"The AWS received input=<"<<input<<"> and function=<"<<input<<"> from the the client using  TCP over port"<<endl;

        sendto(aws_UDP_socket,input.c_str(),BUFFERSIZE,0,(struct sockaddr *)&UDP_serverA,serverA_len);
        sendto(aws_UDP_socket,input2.c_str(),BUFFERSIZE,0,(struct sockaddr *)&UDP_serverA,serverA_len);
        cout<<"The AWS sent <"<<input2<<"> and <"<<input<<"> to Backend-Server A"<<endl;
        sendto(aws_UDP_socket,input.c_str(),BUFFERSIZE,0,(struct sockaddr *)&UDP_serverB,serverB_len);
        sendto(aws_UDP_socket,input2.c_str(),BUFFERSIZE,0,(struct sockaddr *)&UDP_serverB,serverB_len);
        cout<<"The AWS sent <"<<input2<<"> and <"<<input<<"> to Backend-Server B"<<endl;
        sendto(aws_UDP_socket,input.c_str(),BUFFERSIZE,0,(struct sockaddr *)&UDP_serverC,serverC_len);
        sendto(aws_UDP_socket,input2.c_str(),BUFFERSIZE,0,(struct sockaddr *)&UDP_serverC,serverC_len);
        cout<<"The AWS sent <"<<input2<<"> and <"<<input<<"> to Backend-Server C"<<endl;

        do {
            recvfrom(aws_UDP_socket,serverA_output,BUFFERSIZE,0,(struct sockaddr *)&UDP_serverA,&serverA_len);
            string serverAstr (serverA_output);
            serverA_dataout.push_back(serverAstr);
        } while (*serverA_output != '$');
        
        do {
            recvfrom(aws_UDP_socket,serverB_output,BUFFERSIZE,0,(struct sockaddr *)&UDP_serverB,&serverB_len);
            string serverBstr (serverB_output);
            serverB_dataout.push_back(serverBstr);
        } while (*serverB_output !='$');

        do {
            recvfrom(aws_UDP_socket,serverC_output,BUFFERSIZE,0,(struct sockaddr *)&UDP_serverC,&serverC_len);
            string serverCstr (serverC_output);
            serverC_dataout.push_back(serverCstr);
        } while (*serverC_output != '$');
        if (input.compare("search") == 0){
          
            serverA_dataout.pop_back();
            serverB_dataout.pop_back();
            serverC_dataout.pop_back();
          
            int count = 0;
            if(serverA_dataout.back() == ""){
                cout<<"The AWS received < 0 > similar words from Backend-Server < A > using UDP over port < "<<UDPPortA<<" >"<<endl;
            }
            else {
                count ++;
                cout<<"The AWS received < 1 > similar words from Backend-Server < A > using UDP over port < "<<UDPPortA<<" >"<<endl;
            }

            if (serverB_dataout.back() == ""){
                cout<<"The AWS received < 0 > similar words from Backend-Server < B > using UDP over port < "<<UDPPortB<<" >"<<endl;
            }
            else {
                count++;
                cout<<"The AWS received < 1 > similar words from Backend-Server < B > using UDP over port < "<<UDPPortB<<" >"<<endl;
            }
            if (serverC_dataout.back() == "") {
                cout<<"The AWS received < 0 > similar words from Backend-Server < C > using UDP over port < "<<UDPPortC<<" >"<<endl;
            }
            else {
                count++;
                cout<<"The AWS received < 1 > similar words from Backend-Server < C > using UDP over port < "<<UDPPortC<<" >"<<endl;

            }
            if (count>0){
                cout<<"The AWS sent < 1 > matches to the client"<<endl;
            }
            else {
                cout<<"The AWS sent < 0 > matches to the client"<<endl;
            }
            cout<<"The AWS sent < "<<input2<<" > to the monitor via TCP port "<<endl;

        }
        else if (input.compare("prefix") == 0){
            cout<<"The AWS received < "<<serverA_dataout.size()-1<<" > matches from Backend-Server < A > using UDP over port < "<<UDPPortA<<" >"<<endl;
            cout<<"The AWS received < "<<serverB_dataout.size()-1<<" > matches from Backend-Server < B > using UDP over port < "<<UDPPortB<<" >"<<endl;
            cout<<"The AWS received < "<<serverC_dataout.size()-1<<" > matches from Backend-Server < C > using UDP over port < "<<UDPPortC<<" >"<<endl;
            int sum = serverA_dataout.size() + serverB_dataout.size() + serverC_dataout.size() - 3;
            cout<<"The AWS sent < "<<sum<<" > matches to client"<<endl;
            cout<<"The AWS sent < "<<sum<<" > matches to the monitor via TCP port"<<endl;
        }
        else if (input.compare("suffix") == 0){
            cout<<"The AWS received < "<<serverA_dataout.size()-1<<" > matches from Backend-Server < A > using UDP over port < "<<UDPPortA<<" >"<<endl;
            cout<<"The AWS received < "<<serverB_dataout.size()-1<<" > matches from Backend-Server < B > using UDP over port < "<<UDPPortB<<" >"<<endl;
            cout<<"The AWS received < "<<serverC_dataout.size()-1<<" > matches from Backend-Server < C > using UDP over port < "<<UDPPortC<<" >"<<endl;
            int sum = serverA_dataout.size() + serverB_dataout.size() + serverC_dataout.size() - 3;
            cout<<"The AWS sent < "<<sum<<" > matches to client"<<endl;
            cout<<"The AWS sent < "<<sum<<" > matches to the monitor via TCP port "<<TCPMonitorPort<<endl;
        }

        vector<string> all_results;
        if ((input.compare("prefix") == 0) || (input.compare("suffix") ==0))
        {
            for (int i=0;i<serverA_dataout.size()-1;i++){
                all_results.push_back(serverA_dataout[i]);
            }
            for (int j=0;j<serverB_dataout.size()-1;j++){
                all_results.push_back(serverB_dataout[j]);
            }
            for (int k=0;k<serverC_dataout.size()-1;k++){
                all_results.push_back(serverC_dataout[k]);
            }
        }
        else {
            if (serverA_dataout.size()!= 0){
                all_results.push_back(serverA_dataout.back());
                serverA_dataout.pop_back();
            }
            else if (serverB_dataout.size() != 0){
                all_results.push_back(serverB_dataout.back());
                serverB_dataout.pop_back();
            }
            else if (serverC_dataout.size() != 0){
                all_results.push_back(serverC_dataout.back());
                serverC_dataout.pop_back();
            }
        }
        
       
        for (int b=0;b<all_results.size();b++){
            send(tcp_socket,all_results[b].c_str(),BUFFERSIZE,0);
        }

        string endstr = "$";
        send(tcp_socket,endstr.c_str(),BUFFERSIZE,0);

        
        int tcp_socket_monitor = accept(monitor,(struct sockaddr *)&monitor_addr,&monitor_len);
        if (tcp_socket_monitor<0)
        {
            perror("socket can't be bound to");
            close(tcp_socket_monitor);
            return 0;
        }

        char *message_monitor = new char[BUFFERSIZE];
        
        recv(tcp_socket_monitor,message_monitor,BUFFERSIZE,0);


        for (int j=0;j<all_results.size();j++){
            send(tcp_socket_monitor,all_results[j].c_str(),BUFFERSIZE,0);
        }
        if (input.compare("search")==0){
            if (serverA_dataout.size()!=0){
                for (int a=0;a<serverA_dataout.size();a++){
                    send(tcp_socket_monitor,serverA_dataout[a].c_str(),BUFFERSIZE,0);
                }
            }
            else if (serverB_dataout.size()!=0){
                 for (int c=0;c<serverB_dataout.size();c++){
                    send(tcp_socket_monitor,serverB_dataout[c].c_str(),BUFFERSIZE,0);
                }
            }
            else if (serverC_dataout.size()!=0){
                for (int d=0;d<serverC_dataout.size();d++){
                    send(tcp_socket_monitor,serverB_dataout[d].c_str(),BUFFERSIZE,0);
                }
            }
        }
    
        send(tcp_socket_monitor,input2.c_str(),BUFFERSIZE,0);
        send(tcp_socket_monitor,input.c_str(),BUFFERSIZE,0);
        string end = "$";
        send(tcp_socket_monitor,end.c_str(),BUFFERSIZE,0);

        serverA_dataout.clear();
        serverB_dataout.clear();
        serverC_dataout.clear();
        all_results.clear();
        dataInput.clear();

        close(aws_UDP_socket);
        close(tcp_socket_monitor);
      
    }
}