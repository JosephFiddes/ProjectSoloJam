#ifndef JDF_S2OSC_SERIAL_CONNECTION
#define JDF_S2OSC_SERIAL_CONNECTION

#include <Windows.h>
#include <iostream>

// Code makes use of "Windows Serial Port Programming" by Robertson Bayer (2008).

#define ERROR_STRING_SIZE 1024
#define READ_BUFFER_SIZE 1024

class SerialConnection 
{
public:
	SerialConnection(const char* port);
	~SerialConnection();

	char* message;
	int32_t value;

	char command_type;
	uint32_t command_source;
	uint32_t command_value;

	// Receive message from serial connection, and place in read_buffer.
	DWORD recv();

	// Parses info in read_buffer, and puts into message and value.
	// Returns true if there is data to parse. Returns false otherwise.
	bool parse();

	// CLoses the connection.
	void close();

private:
	HANDLE hSerial;

	char read_buffer[READ_BUFFER_SIZE];
	DWORD dw_bytes_read = 0;
	uint16_t buffer_offset;

	// Error 
	DWORD e; 
	char last_error[ERROR_STRING_SIZE];



	// Function to open the serial port
	//int openSerialPort(const char* portname);
	inline void handle_error();

	inline void parse_string();
	inline uint8_t parse_uint8();
	inline uint32_t parse_uint32();
	inline char parse_char();
};

#endif