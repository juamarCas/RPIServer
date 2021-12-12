C_INCLUDE = -I./Includes
FLAGS = -Wall

all: main.o Serial.o
	g++ $(FLAGS) -o app main.o Serial.o -pthread

main.o: Serial.o
	g++ -c main.cpp $(C_INCLUDE) -pthread

Serial.o: 
	g++ -c Serial.cpp $(C_INCLUDE)
clean: 
	rm -f ./app *.o
