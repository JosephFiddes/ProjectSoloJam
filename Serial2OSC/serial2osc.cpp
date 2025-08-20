#include <memory>

#include "Controller.h"
#include "SerialConnection.h"

#define SERIAL_PORT "COM7"

#define I_TRUE 1
#define I_FALSE 0

SerialConnection* newSerialConnection(const char* port);

SerialConnection* newSerialConnection(const char* port)
{
    return new SerialConnection(port);
}

inline void update();


std::unique_ptr<SerialConnection> s(newSerialConnection(SERIAL_PORT));

Controller controller;

int main(int, char**)
{
    try {
        while(true) {
            update();
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        s->close();
    }

    return 0;
}

// Run repeatedly.
inline void update() {
    DWORD message_size;
    bool bMessage_empty = true;

    // Collect messages until nothing is received.
    s->recv();
    while (s->parse()) {  
        controller.add_command(s->command_type, s->command_source, s->command_value);
    }

    controller.send_commands();

    /*
    // Initial test: Arm recording for track 3, and disarm recording for track 2.
    t->init_buffer();
    t->add_to_buffer("b/track/3/recarm", I_TRUE);
    t->add_to_buffer("b/track/2/recarm", I_FALSE);
    t->finalize_buffer();
    t->send();
    */
}