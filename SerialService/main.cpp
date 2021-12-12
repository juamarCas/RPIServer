#include <iostream>
#include <csignal>
#include <cstdint>

/*custom headers*/
#include "Serial.h"

#define PACKET_SIZE 12 
void signalHandler(int signum);

#pragma pack(1)
typedef struct Payloads{
	std::uint16_t moist_1;
	std::uint16_t moist_2;
	float         temp; 
	float         hum;
}Payload, *PPayload;
#pragma pack()

Serial * serial = new Serial("/dev/serial0", false, false, false, Serial::B8, Serial::BA9600);

int main(){
	
	signal(SIGINT, signalHandler);
	serial->begin();
	PPayload payload;
	
	auto callback = [&payload](char * data){
		payload = (PPayload)data;
	 	std::cout<<payload->hum<<std::endl;
	};
	while(1){
		serial->serialRead(PACKET_SIZE ,callback);
	}
	return 0; 

}

void signalHandler(int signum){
	serial->ClosePort();
	exit(signum);
}

