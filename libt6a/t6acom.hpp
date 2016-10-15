
#ifndef T6ACOM_HPP
#define T6ACOM_HPP

#include <iostream>

#include "msg_potstate.hpp"

namespace t6a
{

  /// Com port functionality for HK-T6A transmitter
  class T6ACom
  {
    public:
      T6ACom( const std::string& path );
      ~T6ACom();

      void openPort();
      
      bool getPotState( MSG_PotState& state, unsigned int maxRequests = 100 );
    
    private:
      std::string m_path;
      int m_handle;
  };

};

#endif
