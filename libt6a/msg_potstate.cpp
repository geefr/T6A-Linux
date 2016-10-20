
#include "msg_potstate.hpp"

namespace t6a
{
  uint8_t MSG_PotState::header1()
  {
    return 0x55;
  }

  uint8_t MSG_PotState::header2()
  {
    return 0xFC;
  }    
};

