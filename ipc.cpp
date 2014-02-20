/*
 * ipc.cpp
 * Date: 2014-02-20
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 *
 */

#include <zmq.h>
#include <zmq.hpp>

#include <iostream>
#include <thread>
#include <chrono>


class Timer
{
    typedef std::chrono::high_resolution_clock clock_type;
    clock_type::time_point m_start_time;

    template< class T >
    static inline double get_seconds( T t )
    {
        return double( std::chrono::duration_cast< std::chrono::microseconds >( t ).count() ) / 1000000.0;
    }

public:

    Timer( void ) : m_start_time( clock_type::now() ) { }

    inline double seconds( void ) const
    {
        return get_seconds( clock_type::now() - m_start_time );
    }

    inline void restart( void )
    {
        m_start_time = clock_type::now();
    }
};


void server_thread( zmq::context_t &context )
{
    std::string str( 100 , 'X' );

    // Prepare our context and socket
    zmq::socket_t socket (context, ZMQ_REP);
    socket.bind ( "inproc://testing" );

    while (true)
    {
        zmq::message_t request;

        // Wait for next request from client
        socket.recv (&request);
        // std::cout << "Received Hello" << std::endl;
        
        // std::this_thread::sleep_for( std::chrono::microseconds( 10 ) );

        // Send reply back to client
        zmq::message_t reply (100);
        memcpy ((void *) reply.data (), str.c_str() , 100 );
        socket.send (reply);
    }
}

void client_thread( zmq::context_t &context )
{
    std::cout.precision( 14 );
    
    // Prepare our context and socket
    zmq::socket_t socket (context, ZMQ_REQ);

    std::cout << "Connecting to hello world server…" << std::endl;
    socket.connect ( "inproc://testing" );

    // Do 10 requests, waiting each time for a response
    for (int request_nbr = 0; request_nbr != 1000; request_nbr++)
    {
        zmq::message_t request (6);
        memcpy ((void *) request.data (), "Hello", 5);
        std::cout << "Sending Hello " << request_nbr << "…" << std::endl;
        Timer timer;
        socket.send (request);

        // Get the reply.
        zmq::message_t reply;
        socket.recv (&reply);
        std::cout << "Received World " << request_nbr << " in " << timer.seconds() << " seconds." << std::endl;
        
        std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    }
}

using namespace std;

int main( int argc , char *argv[] )
{
    zmq::context_t context (1);
    
    cout << "Starting server" << endl;
    std::thread t1 { server_thread , std::ref( context ) };
    
    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
  
    cout << "Starting client" << endl;
    std::thread t2 { client_thread , std::ref( context ) };
    
    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    
    t1.join();
    t2.join();
    
    return 0;
}
