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
 
int serial_port;

int main(){
	
	signal(SIGINT, signalHandler);
	Serial * serial = new Serial("/dev/serial0", false, false, false, Serial::B8, Serial::BA9600);
	serial->begin();
	PPayload payload;
	
	auto callback = [&payload](char * data){
		payload = (PPayload)data;
	 	std::cout<<payload->hum<<std::endl;
	};
	while(1){
		serial->serialRead(12 ,callback);
	}
	return 0; 

}

void signalHandler(int signum){
	std::cout<<"BYEEE!"<<std::endl;
	close(serial_port);
	exit(signum);
}

