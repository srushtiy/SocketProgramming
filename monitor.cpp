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

#define TCPMonitorPort 26221
const int BUFFERSIZE = 2048;
const char* LOCALHOST = "127.0.0.1";


int main(){


	///////----------------------TCP SOCKET CONNECTION ADAPATED FROM BEEJUS---------------------------------------------/////////////////////

		
	char* aws_output = new char[BUFFERSIZE];

	int monitor = socket(AF_INET,SOCK_STREAM,0);
	string end="$";

	if (monitor<1)
	{
		perror("monitor socket has failed to be created");
		return 0;
	}
	struct sockaddr_in aws_addr;
	aws_addr.sin_family = AF_INET;
	aws_addr.sin_addr.s_addr = inet_addr(LOCALHOST);
	aws_addr.sin_port = htons(TCPMonitorPort);

	int connect_sock = connect(monitor, (struct sockaddr*)&aws_addr, sizeof(aws_addr));
	if(connect_sock<0)
	{
		cout<<"Connection error: "<<TCPMonitorPort<<endl;
	}
	cout<<"The monitor is up and running"<<endl;
	listen(monitor,1);

	send(monitor,end.c_str(),BUFFERSIZE,0);
	while(true){
		vector<string> results;
		do {
			recv(monitor,aws_output,BUFFERSIZE,0);
			string aws_outputstr (aws_output);
			results.push_back(aws_outputstr);
		} while (*aws_output != '$');

		results.pop_back();
		if (results.back().compare("search")==0){
			results.pop_back();
				if (results.size()>1){
					cout<<results[0]<<endl;
					cout<<"One edit distance match is <"<<results.back()<<">:";
					results.pop_back();
					cout<<"< "<<results.back()<<" >"<<endl;
				}
				else{
					cout<<results[0]<<endl;
				}
					

		}
		else if ((results.back().compare("prefix")==0) || (results.back().compare("suffix")==0))
		{
			results.pop_back();
			cout<<"Found < "<<results.size()-1<<" > matches for < "<<results.back()<<" >:"<<endl;
			results.pop_back();
			for (int i=0;i<results.size();i++)
			{
				cout<<results[i]<<endl;
			}
		}
		results.clear();

	}
	
	
	close(monitor);
	return 0;



}