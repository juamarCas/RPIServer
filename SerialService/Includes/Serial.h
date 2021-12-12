#include <iostream>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h> //read(), close(), write()
#include <cstring>
#include <cstdint>
#include <functional>
/*
 * Abstraction class for serial, there are a lot default options
 * but in the future i may add those options
 * it takes as default vtime = 10 and vmin = 0
 */
class Serial{
public: 
	
	enum DATA_SIZE {B8, B9};
	enum BAUD {BA9600, BA115200};

private:
	int _baud;
	int _serialPort;
	struct termios _tty;
	std::string _path; 
	bool _parity; 
	bool _stop; 
	bool _flowControl; 
	Serial::DATA_SIZE _size;
		
public: 
	/*
	 *@brief Constructor of the serial service
	 *@param path char array with the location of the device
	 *@param parity bool indicating if using parity bit (true if want parity bit, false otherwise)
	 *@param stop bool indicating if want 1 or 2 stop bits (true for 1 bit, false otherwise)
	 *@param bool indicating if want flow control
	 *@param size enum indicating bps of the communication
	 */
	Serial(std::string path, bool parity, bool stop, bool flow_control,Serial::DATA_SIZE size, Serial::BAUD _baud);
	~Serial(){
	
	}
	/*
	 *@brief begin communication
	 *@param packet int, packet size of the buffer
	 */
	bool begin();

	void SetVTIME(int time) { _tty.c_cc[VTIME] = time; } 
	
	void setVMIN (int min)  { _tty.c_cc[VMIN]  = min; }
	
	/*
	 *@brief reads data from buffer calls a callback to a function that is user defined, the callback receives a string
	 * 
	 */
	void serialRead(const int packet_size, const std::function<void(char *)>& func);
}; 
