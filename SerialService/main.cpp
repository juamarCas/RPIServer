#include <iostream>
#include <csignal>
#include <cstdint>
#include <thread>

/*custom headers*/
#include "Serial.h"

#define PACKET_SIZE 12
void signalHandler(int signum);
void serialRead();
#pragma pack(1)
typedef struct Payloads{
	std::uint16_t moist_1;
	std::uint16_t moist_2;
	float         temp;
	float         hum;
}Payload, *PPayload;
#pragma pack()

Serial serial("/dev/serial0", false, false, false, Serial::B8, Serial::BA9600);

int main(){
	
	signal(SIGINT, signalHandler);
	std::thread serialService{serialRead};
	serialService.join();
	return 0;

}

void serialRead(){
	PPayload payload;
	serial.begin();
	auto callback = [&payload](char * data){
                payload = (PPayload)data;
                std::cout<<payload->hum<<std::endl;
        };

	while(1){
		serial.serialRead(PACKET_SIZE, callback);
	}
}

void signalHandler(int signum){
	serial.ClosePort();
	exit(signum);
}

