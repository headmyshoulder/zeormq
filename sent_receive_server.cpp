/*
 * main.cpp
 * Date: 2013-06-01
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include <zmq.hpp>
#include <string>
#include <iostream>
#include <unistd.h>
#include <thread>
#include <chrono>

int main ( int argc , char **argv )
{
    std::string str( 100 , 'X' );

    // Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REP);
    socket.bind ("tcp://*:5555");

    while (true)
    {
        zmq::message_t request;

        // Wait for next request from client
        socket.recv (&request);
        std::cout << "Received Hello" << std::endl;
        
        // std::this_thread::sleep_for( std::chrono::microseconds( 10 ) );

        // Send reply back to client
        zmq::message_t reply (100);
        memcpy ((void *) reply.data (), str.c_str() , 100 );
        socket.send (reply);
    }
    return 0;
}