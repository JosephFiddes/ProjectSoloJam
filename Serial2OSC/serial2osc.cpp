#include <memory>

#include "OSCconnection.h"
#include "SerialConnection.h"

#define I_TRUE 1
#define I_FALSE 0

OSCConnection* newOSCConnection();

OSCConnection* newOSCConnection()
{
    return new OSCConnection;
}

SerialConnection* newSerialConnection();

SerialConnection* newSerialConnection()
{
    return new SerialConnection;
}

inline void update();


std::unique_ptr<SerialConnection> s(newSerialConnection());
std::unique_ptr<OSCConnection> t(newOSCConnection());

int main(int, char**)
{
    try {
        while(true) {
            update();
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}

// Run repeatedly.
inline void update() {
    DWORD message_size;
    bool bMessage_empty = true;

    t->init_buffer();

    // Collect messages until nothing is received.
    s->recv();
    while (s->parse()) {
        t->add_to_buffer(s->message, s->value, s->value_type);   
    }

    t->finalize_buffer();
    t->send();

    /*
    // Initial test: Arm recording for track 3, and disarm recording for track 2.
    t->init_buffer();
    t->add_to_buffer("b/track/3/recarm", I_TRUE);
    t->add_to_buffer("b/track/2/recarm", I_FALSE);
    t->finalize_buffer();
    t->send();
    */
}