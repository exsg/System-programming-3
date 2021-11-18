OBJS	= whoClient.o whoClientRead.o Queue.o Protocol.o
SOURCE	= whoClient.cpp whoClientRead.cpp Queue.cpp Protocol.cpp
HEADER	= whoClientRead.h Queue.h Protocol.h
OUT	= client.out
CC	 = g++
FLAGS	 = -g -c -Wall -std=c++11
LFLAGS	 = -lpthread -pthread

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

whoClient.o: whoClient.cpp
	$(CC) $(FLAGS) whoClient.cpp 

whoClientRead.o: whoClientRead.cpp
	$(CC) $(FLAGS) whoClientRead.cpp 

Queue.o: Queue.cpp
	$(CC) $(FLAGS) Queue.cpp 

Protocol.o: Protocol.cpp
	$(CC) $(FLAGS) Protocol.cpp 


clean:
	rm -f $(OBJS) $(OUT)

run: $(OUT)
	./$(OUT)
