
#ifndef MSG_PARAMS_HPP
#define MSG_PARAMS_HPP

#include <cstdint>

#include <cstring>

namespace t6a
{

  struct __attribute__((packed)) MSG_Params
  {
    uint8_t modelAndCraft; // 0xf0 model 1-4 0x0f acro, heli120, heli90, heli140
    uint8_t reverseMask; // XX654321 0 normal 1 reversed
    
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
