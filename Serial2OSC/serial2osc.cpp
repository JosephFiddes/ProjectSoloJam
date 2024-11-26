#include <memory>
#include <iostream>

#include "OSCconnection.h"

#define I_TRUE 1
#define I_FALSE 0

OSCConnection* newConnection();

OSCConnection* newConnection()
{
    return new OSCConnection;
}

int main(int, char**)
{
    std::unique_ptr<OSCConnection> t(newConnection());
    try {
        // Initial test: Arm recording for track 3, and disarm recording for track 2.
        t->init_buffer();
        t->add_to_buffer("b/track/3/recarm", I_TRUE);
        t->add_to_buffer("b/track/2/recarm", I_FALSE);
        t->finalize_buffer();
        t->send();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}