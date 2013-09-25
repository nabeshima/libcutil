#include "SerialPort.h"

#include <iostream>

using namespace std;
using namespace cutil;

int main() {
  SerialPort sport;

  try {
    // to secify baudrate by using B460800 if available
    bool ret = sport.open( "/def/ttyACM0", 460800, EIGHT, NONE, ONE );  
    cout << ret << endl;
  }
  catch( Exception &e ) {
    e.printStackTrace();
  }
  
  while ( sport.isReadable( 20000 ) ) {

    long l = 0;
    int rc = sport.read( &l, sizeof( l ) );
    
    cout << rc << " " << l << endl;
  }
  

  return 0;
}
