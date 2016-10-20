
#ifndef MSG_POTSTATE_HPP
#define MSG_POTSTATE_HPP

#include <cstdint>

#include <cstring>

#include <unistd.h>

namespace t6a
{

  struct MSG_PotState
  {
    enum Field
    {
      Header1, // 0x55
      Header2, // 0xFC
      Channel1MSB,
      Channel1LSB,
      Channel2MSB,
      Channel2LSB,
      Channel3MSB,
      Channel3LSB,
      Channel4MSB,
      Channel4LSB,
      Channel5MSB,
      Channel5LSB,
      Channel6MSB,
      Channel6LSB,
      UnknownMSB,
      UnknownLSB,
      CheckSumMSB,
      CheckSumLSB
    };

    static uint8_t header1();
    static uint8_t header2();
    
    bool valid() const;
    void reset();
    /// Call after reading the 2 header bytes
    /// Remaining data will be read into structure
    void readState( int fd );

    uint16_t channel1() const;
    uint16_t channel2() const;
    uint16_t channel3() const;
    uint16_t channel4() const;
    uint16_t channel5() const;
    uint16_t channel6() const;
    uint16_t checksum() const;

    uint8_t m_buffer[18];
    bool m_valid;
  };

  inline bool MSG_PotState::valid() const
  {
    return m_valid;
  }
  
  inline void MSG_PotState::reset()
  {
    memset( m_buffer, 0x00, sizeof(m_buffer) );
    m_valid = false;
  }

  inline void MSG_PotState::readState( int fd )
  {
    // Header already handled by caller
    m_buffer[Header1] = 0x55;
    m_buffer[Header2] = 0xFC;

    ssize_t bytesRead(2);
    while( bytesRead < 18 )
    {
      ssize_t num( read( fd, m_buffer + bytesRead, 18 - bytesRead ) );
      if( num > 0 )
      {
        bytesRead += num;
      }
    }

    // Calculate checksum
    unsigned int sum(0);
    for(unsigned int i(2); i < 16; ++i )
    {
      sum += m_buffer[i];
    }    
    if( sum == checksum() )
    {
      m_valid = true;
    }
    else
    {
      m_valid = false;
    }
  }
  
  inline uint16_t MSG_PotState::channel1() const
  {
    uint16_t result( m_buffer[Channel1MSB] );
    result <<= 8;
    result |= m_buffer[Channel1LSB];
    return result;
  }
  
  inline uint16_t MSG_PotState::channel2() const
  {
    uint16_t result( m_buffer[Channel2MSB] );
    result <<= 8;
    result |= m_buffer[Channel2LSB];
    return result;
  }
  
  inline uint16_t MSG_PotState::channel3() const
  {
    uint16_t result( m_buffer[Channel3MSB] );
    result <<= 8;
    result |= m_buffer[Channel3LSB];
    return result;
  }
  
  inline uint16_t MSG_PotState::channel4() const
  {
    uint16_t result( m_buffer[Channel4MSB] );
    result <<= 8;
    result |= m_buffer[Channel4LSB];
    return result;
  }
  
  inline uint16_t MSG_PotState::channel5() const
  {
    uint16_t result( m_buffer[Channel5MSB] );
    result <<= 8;
    result |= m_buffer[Channel5LSB];
    return result;
  }
  
  inline uint16_t MSG_PotState::channel6() const
  {
    uint16_t result( m_buffer[Channel6MSB] );
    result <<= 8;
    result |= m_buffer[Channel6LSB];
    return result;
  }

  inline uint16_t MSG_PotState::checksum() const
  {
    uint16_t result( m_buffer[CheckSumMSB] );
    result <<= 8;
    result |= m_buffer[CheckSumLSB];
    return result;
  }
};

#endif
