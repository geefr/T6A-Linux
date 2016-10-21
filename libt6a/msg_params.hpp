
#ifndef MSG_PARAMS_HPP
#define MSG_PARAMS_HPP

#include <cstdint>

#include <cstring>

namespace t6a
{
  struct MSG_Params
  {
    enum Field
    {
      TXModelAndCraftType, // Mode [1-4] | Craft type [Acro, Heli120, Heli90, Heli140]
      ReverseMask, // XX654321
      Channel1DROn,
      Channel1DROff,
      Channel2DROn, // vv [0-127] vv
      Channel2DROff,
      Channel4DROn,
      Channel4DROff,
      Channel1Swash, // Heli
      Channel2Swash, // Heli
      Channel6Swash,
      Channel1LLimit,
      Channel1RLimit,
      Channel2LLimit,
      Channel2RLimit,
      Channel3LLimit,
      Channel3RLimit,
      Channel4LLimit,
      Channel4RLimit,
      Channel5LLimit,
      Channel5RLimit,
      Channel6LLimit,
      Channel6RLimit,
      TCurveEP0Normal,
      TCurveEP0ID,
      TCurveEP1Normal,
      TCurveEP1ID,
      TCurveEP2Normal,
      TCurveEP2ID,
      TCurveEP3Normal,
      TCurveEP3ID,
      TCurveEP4Normal,
      TCurveEP4ID,
      PCurveEP0Normal,
      PCurveEP0ID,
      PCurveEP1Normal,
      PCurveEP1ID,
      PCurveEP2Normal,
      PCurveEP2ID,
      PCurveEP3Normal,
      PCurveEP3ID,
      PCurveEP4Normal,
      PCurveEP4ID,
      Channel1SubTrim,
      Channel2SubTrim,
      Channel3SubTrim,
      Channel4SubTrim,
      Channel5SubTrim,
      Channel6SubTrim,
      Mix1SourceDest, // Upper nibble source [CH1-CH6,VRA,VRB]. Lower nibble dest [CH1-CH6]
      Mix1Uprate, // [0-127]
      Mix1Downrate,
      Mix1Switch, // [SWA, SWB, ON, OFF]
      Mix2SourceDest,
      Mix2Uprate,
      Mix2Downrate,
      Mix2Switch,
      Mix3SourceDest,
      Mix3Uprate,
      Mix3Downrate,
      Mix3Switch,
      SwitchA, // [None, DR, TCut, Normal/Idle(heli?)]
      SwitchB,
      VRA, // [None, Pitch Adjust (heli)]
      VRB,
      PayloadSize // 65
    };
    
    enum TXModel
    {
      Model1 = 1,
      Model2,
      Model3,
      Model4,
    };
    
    enum CraftType
    {
      Acro = 1, 
      Heli120, 
      Heli90, 
      Heli140
    };
    
    void reset();
    
    /// Call after reading previous message data
    void readState( int fd );

    /// Sum of payload
    uint32_t checksum() const;
    
    TXModel txModel() const;
    CraftType craftType() const;

    uint8_t m_buffer[PayloadSize];
  };
  
  inline void MSG_Params::reset()
  {
    memset( m_buffer, 0x00, PayloadSize );
  }
  
  inline void MSG_Params::readState( int fd )
  {
    // Header already handled by caller
    ssize_t bytesRead(0);
    while( bytesRead < PayloadSize )
    {
      ssize_t num( read( fd, m_buffer + bytesRead, PayloadSize - bytesRead ) );
      if( num > 0 )
      {
        bytesRead += num;
      }
    }
  }
  
  inline uint32_t MSG_Params::checksum() const
  {
    // Calculate checksum
    uint32_t sum(0);
    for(unsigned int i(0); i < PayloadSize; ++i )
    {
      sum += m_buffer[i];
    }
  }

  inline TXModel MSG_Params::txModel() const
  {
    uint8_t data( m_buffer[Field::TXModelAndCraftType] );
    data >> 4;
    return (TXModel)data;
  }
  
  inline CraftType MSG_Params::craftType() const
  {  
    uint8_t data( m_buffer[Field::TXModelAndCraftType] );
    data &= 0b00001111;
    return (CraftType)data;
  }
};

#endif
