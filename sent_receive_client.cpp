

//
// Hello World client in C++
// Connects REQ socket to tcp://localhost:5555
// Sends "Hello" to server, expects "World" back
//
#include <zmq.hpp>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>


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


int main ( int argc , char **argv )
{
    std::cout.precision( 14 );
    
    // Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REQ);

    std::cout << "Connecting to hello world server…" << std::endl;
    socket.connect ("tcp://localhost:5555");

    // Do 10 requests, waiting each time for a response
    for (int request_nbr = 0; request_nbr != 100; request_nbr++)
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
    return 0;
}
