#include "OSCconnection.h"

// UDP stuff derived from 
// https://www.geeksforgeeks.org/udp-server-client-implementation-c/
// https://github.com/tahelka/UDP-ClientAndServer

OSCConnection::OSCConnection() {
    // Initialize Winsock (Windows Sockets).
    WSAData wsaData;
    if (NO_ERROR != WSAStartup(MAKEWORD(2, 2), &wsaData))
    {
        std::cout << "Time Client: Error at WSAStartup()" << std::endl;
        return;
    }

    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        int error_code = WSAGetLastError();

        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
   
    memset(&servaddr, 0, sizeof(servaddr)); 
       
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
}

OSCConnection::~OSCConnection() {
    // Close socket when connection object is destroyed.
	closesocket(sockfd); 
}

size_t OSCConnection::init_buffer()
{
    bReady_to_send = false;

    // Construct a packet
    OSC_packet = OSCPP::Client::Packet(buffer.data(), MAX_PACKET_SIZE);
    OSC_packet
        // Open a bundle with a timetag
        .openBundle(0);

    buffer_size = OSC_packet.size();
    return buffer_size;
}

size_t OSCConnection::add_to_buffer(const char* message, int32_t value) {
    if (!bReady_to_send) {
        OSC_packet
            .openMessage(message, 1)
            .int32(value)
            .closeMessage();
    } else {
        std::cerr << "Do not add to buffer if staged to send." << std::endl;
    }

    buffer_size = OSC_packet.size();
    return buffer_size;
}

size_t OSCConnection::finalize_buffer() {
    OSC_packet.closeBundle();
    bReady_to_send = true;

    buffer_size = OSC_packet.size();
    return buffer_size;
}

size_t OSCConnection::send()
{
    if (bReady_to_send) {
        size_t n = OSC_packet.size();

        sendto(sockfd, buffer.data(), n, 
            0, (const struct sockaddr *) &servaddr,  
                sizeof(servaddr)); 
        std::cout << "Message sent." << std::endl;
        return n; 
    }

    return -1;
}

// Code for receiving data (currently unused and untested).

size_t OSCConnection::recv()
{
    // MSG_WAITALL
    size_t n = recvfrom(sockfd, buffer.data(), MAX_PACKET_SIZE,  
                0, (struct sockaddr *) &servaddr, 0); 
    buffer[n] = '\0';

    std::cout << "Server :" << buffer.data() << std::endl; 
    
    handle_packet(OSCPP::Server::Packet(buffer.data(), n));
    return n;
}



void OSCConnection::handle_packet(const OSCPP::Server::Packet& packet)
{
    if (packet.isBundle()) {
        // Convert to bundle
        OSCPP::Server::Bundle bundle(packet);

        // Print the time
        std::cout << "#bundle " << bundle.time() << std::endl;

        // Get packet stream
        OSCPP::Server::PacketStream packets(bundle.packets());

        // Iterate over all the packets and call handlePacket recursively.
        // Cuidado: Might lead to stack overflow!
        while (!packets.atEnd()) {
            handle_packet(packets.next());
        }
    } else {
        // Convert to message
        OSCPP::Server::Message msg(packet);

        // Get argument stream
        OSCPP::Server::ArgStream args(msg.args());

        // Directly compare message address to string with operator==.
        // For handling larger address spaces you could use e.g. a
        // dispatch table based on std::unordered_map.
        if (msg == "/s_new") {
            const char* name = args.string();
            const int32_t id = args.int32();
            std::cout << "/s_new" << " "
                      << name << " "
                      << id << " ";
            // Get the params array as an ArgStream
            OSCPP::Server::ArgStream params(args.array());
            while (!params.atEnd()) {
                const char* param = params.string();
                const float value = params.float32();
                std::cout << param << ":" << value << " ";
            }
            std::cout << std::endl;
        } else if (msg == "/n_set") {
            const int32_t id = args.int32();
            const char* key = args.string();
            // Numeric arguments are converted automatically
            // to float32 (e.g. from int32).
            const float value = args.float32();
            std::cout << "/n_set" << " "
                      << id << " "
                      << key << " "
                      << value << std::endl;
        } else {
            // Simply print unknown messages
            std::cout << "Unknown message: " << msg << std::endl;
        }
    }
}