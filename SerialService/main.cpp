#include <iostream>
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
#include <csignal>
#include <cstdint>
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

	serial_port = open("/dev/serial0", O_RDWR);
	struct termios tty; 
	if(serial_port < 0){
		std::cout<<"Error from file opening"<<std::endl; 
	}

	if(tcgetattr(serial_port, &tty) != 0){
		std::cout<<"Error from tcgetattr"<<std::endl; 
	}

	/*configure termios*/
	/*
	 *in order: 
	 *parity: no
	 *stop bit: 1 stop bit
	 *data bits: 8 bits
	 *flow control: no
	 *baud: 9600
	 */
	tty.c_cflag &= ~PARENB; 
	tty.c_cflag &= ~CSTOPB; 
	tty.c_cflag &= ~CSIZE;
	tty.c_cflag |= CS8; 
	tty.c_cflag &= ~CRTSCTS; 
	tty.c_cflag |= CREAD | CLOCAL; /*special commands*/
	
	tty.c_lflag &= ~ICANON;
	tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); 
	tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
	tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
	tty.c_cc[VMIN] = 0;
	cfsetispeed(&tty, B9600);
	cfsetospeed(&tty, B9600);
	char read_buff[PACKET_SIZE];
	while(1){
		int n = read(serial_port, &read_buff, PACKET_SIZE); 
		std::cout<<n<<std::endl;
		if(n > 0){
			for(int i = 0; i < sizeof(Payload); i++){
				std::cout<<"["<<i<<"] = "<<read_buff[i]<<std::endl;
			}
			
			PPayload payload = (PPayload)read_buff; 
			std::cout<<payload->hum<<std::endl;
			std::cout<<payload->temp<<std::endl; 
			std::cout<<payload->moist_1<<std::endl; 
			std::cout<<payload->moist_2<<std::endl;
			sleep(2); 
			tcflush(serial_port, TCIOFLUSH);
		}
	}
	return 0; 
}

void signalHandler(int signum){
	std::cout<<"BYEEE!"<<std::endl;
	close(serial_port);
	exit(signum);
}

