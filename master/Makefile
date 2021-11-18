OBJS	= aggregator_main.o AggregatorShell.o Country.o CountryInfo.o Date.o errors.o Protocol.o worker_main.o WorkerShell.o Patient.o
SOURCE	= aggregator_main.cpp AggregatorShell.cpp Country.cpp CountryInfo.cpp Date.cpp errors.cpp Protocol.cpp worker_main.cpp WorkerShell.cpp Patient.cpp
HEADER	= AggregatorShell.h Country.h CountryInfo.h Date.h errors.h Protocol.h Worker.h WorkerShell.h Patient.h
OUT	= master.out
CC	 = g++
FLAGS	 = -g -c -Wall -std=c++11
LFLAGS	 = -lpthread -pthread

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

aggregator_main.o: aggregator_main.cpp
	$(CC) $(FLAGS) aggregator_main.cpp 

AggregatorShell.o: AggregatorShell.cpp
	$(CC) $(FLAGS) AggregatorShell.cpp 

Country.o: Country.cpp
	$(CC) $(FLAGS) Country.cpp 

CountryInfo.o: CountryInfo.cpp
	$(CC) $(FLAGS) CountryInfo.cpp 

Date.o: Date.cpp
	$(CC) $(FLAGS) Date.cpp 

errors.o: errors.cpp
	$(CC) $(FLAGS) errors.cpp 

Protocol.o: Protocol.cpp
	$(CC) $(FLAGS) Protocol.cpp 

worker_main.o: worker_main.cpp
	$(CC) $(FLAGS) worker_main.cpp 

WorkerShell.o: WorkerShell.cpp
	$(CC) $(FLAGS) WorkerShell.cpp 

Patient.o: Patient.cpp
	$(CC) $(FLAGS) Patient.cpp 


clean:
	rm -f $(OBJS) $(OUT)

run: $(OUT)
	./$(OUT)

