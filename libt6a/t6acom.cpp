
#include "t6acom.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <exception>
#include <stdexcept>

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
    
    /* Baud rate/port settings TODO
     * 
tcgetattr(fd, &oldtio); /* save current port settings */ 


/* set new port settings for canonical input processing * 
midtio.c_cflag = BAUDRATE9600 | CS8 | CLOCAL | CSTOPB ;
midtio.c_iflag = IGNPAR | ICRNL;
midtio.c_oflag = 0;
midtio.c_lflag = ICANON;

newtio.c_cflag = BAUDRATE19200 | CRTSCTS | CS8 | CLOCAL | CREAD;
newtio.c_iflag = IGNPAR | ICRNL;
newtio.c_oflag = 0;
newtio.c_lflag = ICANON;


tcsetattr(fd, TCSANOW, &midtio);
char break[] = {0x00}; // break field (0000 0000)

c = write (fd, break, sizeof(break));

tcsetattr(fd, TCSANOW, &newtio);  


char sync [] = {0x55}; 
char PID [] = {0xE2}; 
char data [] = {0xAA}; 
char data1 [] = {0xA5};  
char check [] = {0xAF}; 
c = write (fd, sync, sizeof(sync));
c = write (fd, PID, sizeof(PID));
c = write (fd, data, sizeof(data));
c = write (fd, data1, sizeof(data1));
c = write (fd, check, sizeof(check));

/* restore old port settings *
 tcsetattr(fd,TCSANOW,&oldtio);*/
  }
  
  T6ACom::~T6ACom()
  {
    close( m_handle );
  }
  
  bool T6ACom::getPotState( MSG_PotState& state, unsigned int maxRequests )
  {
    ssize_t bytesRead(0);
    unsigned int requests(0);
    while( bytesRead == 0 && requests < maxRequests )
    {
      ++requests;
      state.reset();
      bytesRead = read( m_handle, &state, sizeof(MSG_PotState) );
      if( !state.valid() )
      {
        bytesRead = 0;
      }
    }
    return bytesRead != 0;
  }

};
