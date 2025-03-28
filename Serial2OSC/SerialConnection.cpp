#include "SerialConnection.h"

// Function to open the serial port
/*
int SerialConnection::openSerialPort(const char* portname)
{
    int fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        std::cerr << "Error opening " << portname << ": "
             << strerror(errno) << std::endl;
        return -1;
    }
    return fd;
}
*/

SerialConnection::SerialConnection()
{
	hSerial = CreateFile("COM4", GENERIC_READ, 0, 0, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (hSerial == INVALID_HANDLE_VALUE) {
		handle_error();
	}

	// Note: source document substitutes hSerial for dcbSerial. This may be in error.

	// Parameters for handle
	DCB dcbSerialParams = {0};
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	if (!GetCommState(hSerial, &dcbSerialParams)) {
		handle_error();
	}
	
	dcbSerialParams.BaudRate=CBR_9600;
	dcbSerialParams.ByteSize=8;
	dcbSerialParams.StopBits=ONESTOPBIT;
	dcbSerialParams.Parity=NOPARITY;
	
	if(!SetCommState(hSerial, &dcbSerialParams)){
		//error setting serial port state
		handle_error();
	}

	// Timeouts
	COMMTIMEOUTS timeouts={0};
	timeouts.ReadIntervalTimeout=50;
	timeouts.ReadTotalTimeoutConstant=50;
	timeouts.ReadTotalTimeoutMultiplier=10;
	
	timeouts.WriteTotalTimeoutConstant=50;
	timeouts.WriteTotalTimeoutMultiplier=10;
	if(!SetCommTimeouts(hSerial, &timeouts)){
		//error setting serial port state
		handle_error();
	}

	//uint8_t counter = 0;
	//while (true) {

		//std::cout << ".";
	//	counter++;

	// recv();	

	//	if (dw_bytes_read != 0) {
	//		std::cout << dw_bytes_read << ": ";
	//		std::cout.write(read_buffer, dw_bytes_read);
	//		std::cout << std::endl;
	//	}

	//	if (counter == 50) {
	//		counter = 0;
	//		//std::cout << std::endl;
	//	}
	//}
}

DWORD SerialConnection::recv() {
	buffer_offset = 0;

	if (!ReadFile(hSerial, read_buffer, READ_BUFFER_SIZE, &dw_bytes_read, NULL)) {
		handle_error();
	}

	return dw_bytes_read;
}

bool SerialConnection::parse() {
	if (dw_bytes_read == 0) return false;
	if (buffer_offset == dw_bytes_read) return false;

//	if (dw_bytes_read > 0) {
//		std::cout << dw_bytes_read << ": ";
//		std::cout.write(read_buffer, dw_bytes_read);
//		std::cout << std::endl;
//	} else {
//		std::cout << dw_bytes_read << ".";
//	}

	parse_string();
	value = parse_uint32();
	value_type = parse_char();
	parse_uint8();
	return true;
}


inline void SerialConnection::parse_string() {
	message = read_buffer + buffer_offset + 1;
	buffer_offset += 1 + read_buffer[buffer_offset];
}

inline uint8_t SerialConnection::parse_uint8(){
	return read_buffer[buffer_offset++];
}

inline uint32_t SerialConnection::parse_uint32(){
	uint32_t* value = reinterpret_cast<uint32_t*>(read_buffer + buffer_offset);
	buffer_offset += 4;
	return *value;
}

inline char SerialConnection::parse_char(){
	return read_buffer[buffer_offset++];
}

inline void SerialConnection::handle_error() {
	e = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		last_error,
		ERROR_STRING_SIZE,
		NULL
	);
	
	throw std::runtime_error(last_error);
}