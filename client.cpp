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
const int BUFFERSIZE = 2048;
const char* LOCALHOST = "127.0.0.1";

int main(int argc, char* argv[]){

	string function;
	string word; // to store the input
	vector<string> output;
	if (argc <2 ){
		cout<< "You haven't passed in any arguments\n";
		return 0;
	}
	
	function = argv[1];
	word = argv[2];

	string end = "$";


///////----------------------TCP SOCKET CONNECTION ADAPATED FROM BEEJUS---------------------------------------------/////////////////////

	char* aws_output = new char[BUFFERSIZE];
	int client_socket = socket(AF_INET,SOCK_STREAM,0);

	if (client_socket<1)
	{
		perror("client_socket has failed to be created");
		return 0;
	}
	struct sockaddr_in aws_addr;
	aws_addr.sin_family = AF_INET;
	aws_addr.sin_addr.s_addr = inet_addr(LOCALHOST);
	aws_addr.sin_port = htons(TCPClientPort);

	int connect_sock = connect(client_socket,(struct sockaddr*)&aws_addr, sizeof(aws_addr));

	if(connect_sock < 0){
		cout<<"Connection error: "<<TCPClientPort<<endl;
	}
	std::cout << "The client is up and running. \n";
	std::cout <<"The client sent <" <<word<< "> and <" <<function<<"> to AWS"<<endl;
	vector<string> results;
	send(client_socket,function.c_str(),BUFFERSIZE,0);
	send(client_socket,word.c_str(),BUFFERSIZE,0);
	send(client_socket,end.c_str(),BUFFERSIZE,0);
	do {
		recv(client_socket,aws_output,BUFFERSIZE,0);
		string aws_outputstr (aws_output);
		results.push_back(aws_outputstr);
	} while (*aws_output != '$');
	results.pop_back();
	if ((function.compare("prefix")==0) || (function.compare("suffix")==0))
	{
		if (results.size()==0){
			cout<<"Found no matches for < "<<word<<" >"<<endl;
		}
		else {
			cout<<"Found < "<<results.size()<<" > matches"<<endl;		
		}
	}
	else if (function.compare("search") == 0){
		if (results.size() == 0){
			cout<<"Found no matches for < "<<word<<" >"<<endl;
		}
		else {
			cout<<"Found a match for < "<<word<<" >:"<<endl;	
		}
	}
	for (int i=0;i<results.size();i++)
	{
		cout<<results[i]<<endl;
	}
	close(client_socket);
	return 0;
	

}