
#include <cstdio>

#include <iostream>

#include <exception>
#include <stdexcept>
#include <thread>

#include <linux/input.h>
#include <linux/uinput.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include <chrono>
#include <thread>

#include "t6acom.hpp"
using namespace t6a;

int main(void)
{ 
  T6ACom port("/dev/ttyUSB0"); 
  try
  {
    port.openPort();
  }
  catch( std::runtime_error& e )
  {
    std::cerr << "Failed to open COM port" << std::endl;
    return 1;
  }

  // Open uinput
  int fd( open("/dev/input/uinput", O_WRONLY | O_NONBLOCK ) );
  if( fd < 0 )
  {
    fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK );
    if( fd < 0 )
    {
      std::cerr << "Failed to open uinput" << std::endl;
      return 1;
    }
  }

  // Enable the event types we need
  int ret(0);
  // KeyPress, absolute axis, synchronisation
//  ret = ioctl( fd, UI_SET_EVBIT, EV_KEY );
//  if( ret != 0 )
//  {
//    std::cerr << "ioctl 1 failed" << std::endl;
//    return 1;
//  }
  ret = ioctl( fd, UI_SET_EVBIT, EV_ABS );
  if( ret != 0 )
  {
    std::cerr << "ioctl 2 failed" << std::endl;
    return 1;
  }

  // Channels
  // 1: ABS_X
  // 2: ABS_Y
  // 3: ABS_RUDDER
  // 4: ABS_THROTTLE
  // 5: ABS_RX
  // 6: ABS_RY

  ret = ioctl( fd, UI_SET_ABSBIT, ABS_X );
  ret = ioctl( fd, UI_SET_ABSBIT, ABS_Y );
  ret = ioctl( fd, UI_SET_ABSBIT, ABS_RUDDER );
  ret = ioctl( fd, UI_SET_ABSBIT, ABS_THROTTLE );
  ret = ioctl( fd, UI_SET_ABSBIT, ABS_RX );
  ret = ioctl( fd, UI_SET_ABSBIT, ABS_RY );
  if( ret != 0 )
  {
    std::cerr << "ioctl 3 failed" << std::endl;
    return 1;
  }
  ret = ioctl( fd, UI_SET_EVBIT, EV_SYN );
  if( ret != 0 )
  {
    std::cerr << "ioctl 4 failed" << std::endl;
    return 1;
  }
  // Finish configuration
  struct uinput_user_dev uidev;
  memset( &uidev, 0x00, sizeof(uidev) );
  snprintf( uidev.name, UINPUT_MAX_NAME_SIZE, "HK-T6A Joystick" );
  uidev.id.bustype = BUS_VIRTUAL;

  uidev.absmax[ ABS_X ] = 2000;
  uidev.absmin[ ABS_X ] = 1000;
  uidev.absmax[ ABS_Y ] = 2000;
  uidev.absmin[ ABS_Y ] = 1000;
  uidev.absmax[ ABS_RUDDER ] = 2000;
  uidev.absmin[ ABS_RUDDER ] = 1000;
  uidev.absmax[ ABS_THROTTLE ] = 2000;
  uidev.absmin[ ABS_THROTTLE ] = 1000;
  uidev.absmax[ ABS_RX ] = 2000;
  uidev.absmin[ ABS_RX ] = 1000;
  uidev.absmax[ ABS_RY ] = 2000;
  uidev.absmin[ ABS_RY ] = 1000;

  ret = write( fd, &uidev, sizeof(uidev) );
  if( ret != sizeof(uidev) )
  {
    std::cerr << "Failed to write uidev structure" << std::endl;
    return 1;
  }

  ret = ioctl( fd, UI_DEV_CREATE );
  if( ret != 0 )
  {
    std::cerr << "Failed to create device handle" << std::endl;
    return 1;
  }
 
 // port.dump();
 // return 0;
    
  try
  {
    while( true )
    {
      MSG_PotState potState;
      // Block for the state
//std::this_thread::sleep_for(std::chrono::milliseconds(100));
      port.getPotState( potState );

      if( !potState.valid() )
      {
        std::cout << "Pot state invalid" << std::endl;
//        continue;
      }
/*
      std::cout << "Pot State:" << std::endl;
      std::cout << "  channel1: " << std::dec << (unsigned int)potState.channel1() << std::endl;
      std::cout << "  channel2: " << std::dec << potState.channel2() << std::endl;
      std::cout << "  channel3: " << std::dec << potState.channel3() << std::endl;
      std::cout << "  channel4: " << std::dec << potState.channel4() << std::endl;
      std::cout << "  channel5: " << std::dec << potState.channel5() << std::endl;
      std::cout << "  channel6: " << std::dec << potState.channel6() << std::endl;
*/
      struct input_event ev;
      memset(&ev, 0x00, sizeof(ev) );
      ev.type = EV_ABS;
      ev.code = ABS_X;
      ev.value = potState.channel1();
      ret = write( fd, &ev, sizeof(ev) );
      ev.code = ABS_Y;
      ev.value = potState.channel2();
      ret = write( fd, &ev, sizeof(ev) );
      ev.code = ABS_RUDDER;
      ev.value = potState.channel3();
      ret = write( fd, &ev, sizeof(ev) );
      ev.code = ABS_THROTTLE;
      ev.value = potState.channel4();
      ret = write( fd, &ev, sizeof(ev) );
      ev.code = ABS_RX;
      ev.value = potState.channel5();
      ret = write( fd, &ev, sizeof(ev) );
      ev.code = ABS_RY;
      ev.value = potState.channel6();
      ret = write( fd, &ev, sizeof(ev) );

      ev.type = EV_SYN;
      ev.code = 0;
      ev.value = 0;
      ret = write(fd, &ev, sizeof(ev));

      std::cerr << "Sync" << std::endl;
    }
  }
  catch( std::runtime_error& e )
  {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  ret = ioctl(fd, UI_DEV_DESTROY);

  return 0;
}
