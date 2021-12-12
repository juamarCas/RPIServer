#include "Serial.h"

Serial::Serial(std::string path, bool parity, bool stop, bool flow_control,Serial::DATA_SIZE size, Serial::BAUD baud){
	_parity = parity; 
	_stop = stop; 
	_flowControl = flow_control; 
	_size = size; 
	_baud = baud;
	_path = path;
}

bool Serial::begin(){
	_serialPort = open(_path.c_str(), O_RDWR);
	if(_serialPort < 0){
		std::cout<<"Error opening port: "<<strerror(errno)<<std::endl;
		return 0;
	}

	if(tcgetattr(_serialPort, &_tty) != 0){
		std::cout<<"Error from tcgetattr"<<std::endl;
	}

	/*configuration of serial port*/
	if(_parity) _tty.c_cflag |= PARENB; 
	else       _tty.c_cflag &= ~CSTOPB;

	_tty.c_cflag &= ~CSIZE;
	
	int d_size = (int)_size;

	switch(d_size){
		case B8:
		_tty.c_cflag |= CS8;
		break; 

		case B9:
		break;


	}

	_tty.c_cflag &= ~CRTSCTS;
        _tty.c_cflag |= CREAD | CLOCAL; /*special commands*/
	_tty.c_lflag &= ~ICANON;
        _tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);
        _tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
        _tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
        _tty.c_cc[VMIN] = 0;
	
 	int b; 
	int d_baud = (int)_baud;
	
	switch(d_baud){
		case BA9600: 
		b = B9600;
		break; 

		case BA115200:
		break;
	}

	cfsetispeed(&_tty, b);
        cfsetospeed(&_tty, b);
}

void Serial::serialRead(const std::function<void(char *)>& callback){
	char _buff[12];
	int n = read(_serialPort, &_buff, 12);
	std::cout<<_buff<<std::endl;
	if(n > 0){
		for(int i = 0; i < 12; i++){
				std::cout<<"["<<i<<"] = "<<_buff[i]<<std::endl;
		}
		(callback)(_buff);
		sleep(2);
		tcflush(_serialPort, TCIOFLUSH);
	}
}
