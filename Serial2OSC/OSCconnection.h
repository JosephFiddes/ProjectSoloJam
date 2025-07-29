#ifndef JDF_S2OSC_CONNECTION
#define JDF_S2OSC_CONNECTION

#include <iostream>

// Needed for UDP stuff 
#include <winsock2.h>
#include <windows.h>

#include <oscpp/client.hpp>
#include <oscpp/server.hpp>
#include <oscpp/print.hpp>

// Port number.
#define OSC_PORT 2345

class OSCConnection
{
public:
	static const size_t MAX_PACKET_SIZE = 8192;

private:
	// This needs to be declared early, in order for it 
	// to start on a byte divisible by 4.
	// Whether or not this will work on all compilers is unknown, 
	// so it would be best practise to find a way to guarantee
	// alignment.
	std::array<char, MAX_PACKET_SIZE> buffer;
	size_t buffer_size;

public:

	OSCConnection();
	~OSCConnection();

	// The following functions return the occupied size of the buffer.
	// Initialises the send buffer
	size_t init_buffer();
	// Adds info to buffer
	size_t add_to_buffer(const char*, int32_t, char);
	// Finalises buffer for sending
	size_t finalize_buffer();

	// Send and receive data to and from REAPER.
	size_t send();
	size_t recv();

private:
	void handle_packet(const OSCPP::Server::Packet& packet);

	// Packet information. bReady_to_send is used to make sure that only
	// complete packets are sent.
	bool bReady_to_send = false;
	OSCPP::Client::Packet OSC_packet;

    // Client stuff
	int sockfd;
	struct sockaddr_in servaddr;
};

#endif