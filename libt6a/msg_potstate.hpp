
#ifndef MSG_POTSTATE_HPP
#define MSG_POTSTATE_HPP

#include <cstdint>

#include <cstring>

namespace t6a
{

  struct __attribute__((packed)) MSG_PotState
  {
    uint8_t header1; // 0x55
    uint8_t header2; // 0xFC

    uint8_t channel1MSB;
    uint8_t channel1LSB;
    uint8_t channel2MSB;
    uint8_t channel2LSB;
    uint8_t channel3MSB;
    uint8_t channel3LSB;
    uint8_t channel4MSB;
    uint8_t channel4LSB;
    uint8_t channel5MSB;
    uint8_t channel5LSB;
    uint8_t channel6MSB;
    uint8_t channel6LSB;
    
    uint8_t unknownMSB;
    uint8_t unknownLSB;

    uint8_t checksumMSB;
    uint8_t checksumLSB;
    
    bool valid() const;
    void reset();
    uint16_t channel1() const;
    uint16_t channel2() const;
    uint16_t channel3() const;
    uint16_t channel4() const;
    uint16_t channel5() const;
    uint16_t channel6() const;
  };

  inline bool MSG_PotState::valid() const
  {
    if( header1 != 0x55 )
      return false;
    if( header2 != 0xfc )
      return false;
      
    return true;
  }
  
  inline void MSG_PotState::reset()
  {
    memset( this, 0x00, sizeof(MSG_PotState) );
  }
  
  inline uint16_t MSG_PotState::channel1() const
  {
    uint16_t result( channel1MSB );
    result <<= 8;
    result |= channel1LSB;
    return result;
  }
  
  inline uint16_t MSG_PotState::channel2() const
  {
    uint16_t result( channel2MSB );
    result <<= 8;
    result |= channel2LSB;
    return result;
  }
  
  inline uint16_t MSG_PotState::channel3() const
  {
    uint16_t result( channel3MSB );
    result <<= 8;
    result |= channel3LSB;
    return result;
  }
  
  inline uint16_t MSG_PotState::channel4() const
  {
    uint16_t result( channel4MSB );
    result <<= 8;
    result |= channel4LSB;
    return result;
  }
  
  inline uint16_t MSG_PotState::channel5() const
  {
    uint16_t result( channel5MSB );
    result <<= 8;
    result |= channel5LSB;
    return result;
  }
  
  inline uint16_t MSG_PotState::channel6() const
  {
    uint16_t result( channel6MSB );
    result <<= 8;
    result |= channel6LSB;
    return result;
  }
};

#endif
