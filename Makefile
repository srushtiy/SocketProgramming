# build c++ executable files using the following commands

all: serverA.cpp serverB.cpp serverC.cpp aws.cpp client.cpp monitor.cpp
	g++ -o serverA.o serverA.cpp 
	g++ -o serverB.o serverB.cpp
	g++ -o serverC.o serverC.cpp
	g++ -o aws.o aws.cpp
	g++ -o client client.cpp
	g++ -o monitor.o monitor.cpp

serverA:serverA.o
	./serverA.o

serverB:serverB.o
	./serverB.o

serverC:serverC.o
	./serverC.o

aws: aws.o
	./aws.o

monitor: monitor.o
	./monitor.o
