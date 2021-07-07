//
//Full name: Srushti Yadhunath
//Student ID: yadhunat


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <errno.h>
#include <vector>
#include <unistd.h>
#include <clocale>
#include <cstring>
#include <ctype.h>

using std::endl;
using std::string;
using std::vector;
using std::cout;
using std::size_t;
using std::getline;
using std::ifstream;


#define serverC_UDP_PORT 23456
#define AWSClient 24987
const char* LOCALHOST = "127.0.0.1";
const int BUFFERSIZE = 2048;

string search(string);
vector<string> prefix(string);
vector<string> suffix(string);
vector<string> searchOneDist(string);

int main()
{
	string pre_func = "prefix";
	string search_func = "search";
	string suf_func = "suffix";



///////----------------------UDP SOCKET CONNECTION ADAPATED FROM BEEJUS---------------------------------------------/////////////////////

	int udp_socket = socket(AF_INET,SOCK_DGRAM,0);
	struct sockaddr_in udp_serv_addr;
	struct sockaddr_in aws_client;
	socklen_t server_addr_len;
	char* message = new char[BUFFERSIZE];
	if (udp_socket<0){
		perror("Socket hasn't been created");
		close(udp_socket);
		return 0;
	}


	
	udp_serv_addr.sin_family = AF_INET;
	udp_serv_addr.sin_addr.s_addr = inet_addr(LOCALHOST);
	udp_serv_addr.sin_port = htons(serverC_UDP_PORT);

	int bind_to_socket = bind(udp_socket, (struct sockaddr*)&udp_serv_addr, sizeof(udp_serv_addr));
	if(bind_to_socket<0){
		perror("Server C socket can't be bound to");
		close(udp_socket);
		return 0;
	}
	else {
		cout<<"The ServerC is up and running using UDP on port <"<<serverC_UDP_PORT<<">."<<endl;
	}

	
	server_addr_len = sizeof(udp_serv_addr);
	
	int lines = 0;
	string input_function;
	string input_phrase; 

	while(true)
	{
		int recvfromclient = recvfrom(udp_socket,message,BUFFERSIZE,0,(struct sockaddr *)&aws_client,&server_addr_len);
		if (recvfromclient<0)
		{
			perror("Server C socket can't be bound to");
			close(udp_socket);
			return 0;
		}
		string input_function;
		string word;
		input_function = message;
		recvfromclient = recvfrom(udp_socket,message,BUFFERSIZE,0,(struct sockaddr *)&aws_client,&server_addr_len);
		word = message;
		cout<<"The ServerC received input <"<<word<<"> and operation <"<<input_function<<">"<<endl;
		if (input_function.compare(pre_func)==0)
		{
			vector<string> pre_out_data = prefix(word);
			for (int i=0;i<pre_out_data.size();i++)
			{
				string send_data = pre_out_data[i];
				sendto(udp_socket,send_data.c_str(), BUFFERSIZE,0,(struct sockaddr *)&aws_client,server_addr_len);
			}
			cout<<"The ServerC has found < "<<pre_out_data.size()-1<<" > matches"<<endl;

		}
		else if (input_function.compare(search_func)==0)
		{
			string outData = search(word);
			vector<string> search_outData = searchOneDist(word);
			string output;
			if (outData.compare("") == 0){
				output = "The ServerC has found < 0 > matches and ";
			}
			else {
				output = "The ServerC has found < 1 > matches and ";
			}
			if (search_outData.size() == 0){
				output = output + "< 0 > similar words";
			}
			else {
				output = output + "< 1 > similar words";
			}
			cout<<output<<endl;
			search_outData.push_back(outData);
			for (int i=0;i<search_outData.size();i++){
				string send_data = search_outData[i];
				sendto(udp_socket,send_data.c_str(), BUFFERSIZE,0,(struct sockaddr *)&aws_client,server_addr_len);
			}
			string end = "$";
			sendto(udp_socket,end.c_str(), BUFFERSIZE,0,(struct sockaddr *)&aws_client,server_addr_len);

		}
		else if (input_function.compare(suf_func)==0)
		{
			vector<string> suf_out_data = suffix(word);
			cout<<"The ServerC has found < "<<suf_out_data.size()-1<<" > matches"<<endl;
			for (int i=0;i<suf_out_data.size();i++)
			{
				string send_data = suf_out_data[i];
				sendto(udp_socket,send_data.c_str(), BUFFERSIZE,0,(struct sockaddr *)&aws_client,server_addr_len);
			}
		}
		else 
		{
			cout<<"User has asked for an invalid function"<<endl;
		}
		cout<<"The ServerC finished sending the output to AWS"<<endl;


	}
}


string search(string input)
{
	string output;
	string line;
	string meaning;
	string junk;
	int count = 0;
	string delimiter = ":";
	char input_arr[input.size()];
	strcpy(input_arr,input.c_str());
	char c;
	int i=0;
	while(input_arr[i])
	{
		c = input_arr[i];
		input_arr[i] = tolower(c);
		i++;
	}
	string inputstr(input_arr);
	ifstream inFile("backendC.txt");
	if (inFile.is_open()){
		while(getline(inFile,line,':'))
		{
			int size = line.size();
			getline(inFile,junk,':');
			getline(inFile,meaning);
			string word = line.substr(0,line.size()-1);
			char word_arr[word.size()];
			strcpy(word_arr,word.c_str());
			char c;
			int i=0;
			while(word_arr[i])
			{
				c = word_arr[i];
				word_arr[i] = tolower(c);
				i++;
			}
			string word_out(word_arr);
			int result = inputstr.compare(word_out);
			if (result == 0){
				output = meaning;
				return output;
			}
		}
	}
	else {
		cout<<"File not found"<<endl;
		return "";
	}
	//cout<<input<<" not found"<<endl;
	return "";
}

vector<string> prefix(string searchstr)
{
	vector<string> output;
	string line,junk,meaning;
	int input_size = searchstr.size();
	ifstream inFile("backendC.txt");
	if (inFile.is_open()){
		while(getline(inFile,line,':'))
		{
			getline(inFile,junk,':');
			getline(inFile,meaning);
			string word = line.substr(0,input_size);
			char word_arr[word.size()];
			strcpy(word_arr,word.c_str());
			char c;
			int i=0;
			while(word_arr[i])
			{
				c = word_arr[i];
				word_arr[i] = tolower(c);
				i++;
			}
			string word_out(word_arr);
			int compare = searchstr.compare(word_out);
			if (compare == 0)
			{
				output.insert(output.end(),line);
			}
		}
	}
	else {
		cout<<"File not found"<<endl;
		return output;
	}
	output.push_back("$");
	return output;
}

vector<string> suffix(string input)
{
	vector<string> output;
	string line,junk,meaning;
	int input_size = input.size();
	ifstream inFile("backendC.txt");
	if (inFile.is_open()){
		while(getline(inFile,line,':'))
		{
			getline(inFile,junk,':');
			getline(inFile,meaning);
			string word = line.substr(0,line.size()-1);
			if (word.size()>input_size) 
			{
				int word_size = word.size() - input_size;
				word = word.substr(word_size,word.size()-1);
				char word_arr[word.size()];
				strcpy(word_arr,word.c_str());
				char c;
				int i=0;
				while(word_arr[i])
				{
					c = word_arr[i];
					word_arr[i] = tolower(c);
					i++;
				}
				string word_out(word_arr);
				int compare = input.compare(word_out);
				if (compare == 0)
				{
					output.insert(output.end(),line);
				}
			}
			
		}
	}
	else {
		cout<<"File not found"<<endl;
		return output;
	}
	output.push_back("$");
	return output;
}


vector<string> searchOneDist(string oneDist){
	vector<string> output;
	string line, junk, meaning;
	int oneDistLen = oneDist.size();
	char oneDist_arr[oneDistLen];
	strcpy(oneDist_arr,oneDist.c_str());
	char c;
	int i=0;
	while(oneDist_arr[i])
	{
		c = oneDist_arr[i];
		oneDist_arr[i] = tolower(c);
		i++;
	}
	ifstream inFile("backendA.txt");
	int count=0;
	if (inFile.is_open()){
		while(getline(inFile,line,':')){
			getline(inFile,junk,':');
			getline(inFile,meaning);
			string word = line.substr(0,line.size()-1);
			char word_arr[word.size()];
			strcpy(word_arr,word.c_str());
			char c;
			int i=0;
			while(word_arr[i])
			{
				c = word_arr[i];
				word_arr[i] = tolower(c);
				i++;
			}
			if (word.size() == oneDist.size()){
				for (int i=0;i<oneDist.size();i++){
					if (oneDist_arr[i] != word_arr[i]){
						count++;	
					}
				}	
				if (count == 1)
				{
					output.push_back(meaning);
					output.push_back(word);
					return output;
				}
				else {
					count = 0;
				}
			}
		}
	}
	return output;
}
