#include <iostream>
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

int main(){
	int serial_port = open("/dev/serial0", O_RDWR);
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

	tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
	tty.c_cc[VMIN] = 0;
	cfsetispeed(&tty, B9600);
	cfsetospeed(&tty, B9600);
	char read_buff[7];
	while(1){

		int n = read(serial_port, &read_buff, 7); 
		if(n > 0){
			std::cout<<"new message: "<<read_buff;
			sleep(2); 
			tcflush(serial_port, TCIOFLUSH);
		}
	}
	return 0; 
}

