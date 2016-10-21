
#ifndef MSG_PARAMS_HPP
#define MSG_PARAMS_HPP

#include <cstdint>

#include <cstring>

namespace t6a
{
  struct MSG_ParamRequest
  {
    static void write( int fd );
  };
  
  inline void MSG_ParamRequest::write( int fd )
  {
    uint8_t buffer[3];
    buffer[0] = 0x55;
    buffer[1] = 0xFA;
    buffer[2] = 0x00;
    write( fd, buffer, 3 );
  }
};

#endif
