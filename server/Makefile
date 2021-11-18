OBJS	= server_main.o Server.o CountryInfo.o Date.o errors.o Protocol.o Queue.o ServerConfiguration.o MasterWorker.o
SOURCE	= server_main.cpp Server.cpp CountryInfo.cpp Date.cpp errors.cpp Protocol.cpp Queue.cpp ServerConfiguration.cpp MasterWorker.cpp
HEADER	= Server.h CountryInfo.h Date.h errors.h Protocol.h Queue.h ServerConfiguration.h MasterWorker.h
OUT	= server.out
CC	 = g++
FLAGS	 = -g -c -Wall -std=c++11
LFLAGS	 = -pthread -lpthread

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

server_main.o: server_main.cpp
	$(CC) $(FLAGS) server_main.cpp 

Server.o: Server.cpp
	$(CC) $(FLAGS) Server.cpp 

CountryInfo.o: CountryInfo.cpp
	$(CC) $(FLAGS) CountryInfo.cpp 

Date.o: Date.cpp
	$(CC) $(FLAGS) Date.cpp 

errors.o: errors.cpp
	$(CC) $(FLAGS) errors.cpp 

Protocol.o: Protocol.cpp
	$(CC) $(FLAGS) Protocol.cpp 

Queue.o: Queue.cpp
	$(CC) $(FLAGS) Queue.cpp 

ServerConfiguration.o: ServerConfiguration.cpp
	$(CC) $(FLAGS) ServerConfiguration.cpp 

MasterWorker.o: MasterWorker.cpp
	$(CC) $(FLAGS) MasterWorker.cpp 


clean:
	rm -f $(OBJS) $(OUT)

run: $(OUT)
	./$(OUT)
