
#include "t6acom.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#include <exception>
#include <stdexcept>

#include <fstream>
#include <thread>
#include <chrono>
namespace t6a
{

  T6ACom::T6ACom( const std::string& path )
  : m_path( path )
  {
  }

  void T6ACom::openPort()
  {
    m_handle = open( m_path.c_str(), O_RDWR | O_NOCTTY );//| O_NDELAY );
    if( m_handle == -1 )
    {
      throw std::runtime_error( "Failed to open com port. Make sure it exists and that you have permission to open it." );
    }
  
    fcntl( m_handle, F_SETFL, 0 );

    struct termios term;
    memset( &term, 0x00, sizeof(termios) );
    if( tcgetattr( m_handle, &term ) != 0 )
    {
      close(m_handle);
      throw std::runtime_error( "tcgetattr failed." );
    }

    // Set baudrate to 115200
    if( cfsetispeed( &term, B115200 ) != 0 )
    {
      close(m_handle);
      throw std::runtime_error( "Failed to set input baudrate" );
    }
    if( cfsetospeed( &term, B115200 ) != 0 )
    {
      close(m_handle);
      throw std::runtime_error( "Failed to set input baudrate" );
    }

    // Set format and disable flow control
    term.c_cflag &= ~CSIZE;
    term.c_cflag |= CS8;

    term.c_iflag &= ~(IXON | IXOFF | IXANY);

    term.c_cc[VMIN] = 0;
    term.c_cc[VTIME] = 0;

    if( tcsetattr( m_handle, TCSANOW, &term ) != 0 )
    {
      close( m_handle );
      throw std::runtime_error( "tcsetattr failed" );
    }

  }
  
  T6ACom::~T6ACom()
  {
    close( m_handle );
  }
  
  void T6ACom::getPotState( MSG_PotState& state )
  {
    ssize_t bytesRead(0);
    unsigned int requests(0);
    state.reset();
    // Flush to discard old events
    // tcflush( m_handle, TCIOFLUSH );
      
    uint8_t header1(0x00);
    uint8_t header2(0x00);
    while( header1 != MSG_PotState::header1() )
    {
      read( m_handle, &header1, sizeof(uint8_t) );
    }
    while( header2 != MSG_PotState::header2() )
    {
      read( m_handle, &header2, sizeof(uint8_t) );
    }
    state.readState( m_handle );
  }

  void T6ACom::dump()
  {
    std::ofstream strm("./dump", std::ios::binary );
    ssize_t bytesToRead(1024);
    ssize_t bytesRead(0);
    char buffer[1024];
std::this_thread::sleep_for(std::chrono::milliseconds(500));
    while( bytesRead < bytesToRead )
    {
      ssize_t numRead( read( m_handle, buffer + bytesRead, bytesToRead - bytesRead ) );
      if( numRead > 0 )
      {
        bytesRead += numRead;
      }
    }

    strm.write( buffer, 1024 );
    strm.close();
  }

};
