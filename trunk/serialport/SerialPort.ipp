namespace cutil {

inline
SerialPort::SerialPort() throw ()
  : fd( -1 ) {}

inline
SerialPort::~SerialPort() throw () {
  close();
}

inline
bool SerialPort::open( const std::string& portname,
		       int baudrate,
		       DataBit databit,
		       ParityBit paritybit,
		       StopBit stopbit,
		       bool hardware_control )
  throw ( SerialPortException ) {
  
  if ( isOpen() ) {
    close();
  }
  
  fd = ::open( portname.c_str(), O_RDWR | O_NOCTTY | O_NDELAY );
  
  if ( fd == -1 ) {
    return false;
  }


  termios term_attr;
  
  if ( ::tcgetattr( fd, &term_attr ) < 0 ) {
    close();
    throw SerialPortException( "::tcgetattr error", __EXCEPTION_INFO__ );
  }
  pre_term_attr = term_attr;
  
  
  if ( ::cfsetispeed( &term_attr, baudrate ) != 0 ||
       ::cfsetospeed( &term_attr, baudrate ) != 0 ) {
    close();
    throw SerialPortException( "::cfset{i/o}speed error", __EXCEPTION_INFO__ );
  }
  
  term_attr.c_iflag = 0;
  term_attr.c_oflag = 0;

  term_attr.c_cflag &= ~( CSIZE | PARENB | CSTOPB ); // clear
  term_attr.c_cflag |= databit | paritybit | stopbit | CREAD | CLOCAL;

  term_attr.c_iflag &= ~( ICANON | ECHO | ECHOE | ISIG ); // non-canonical mode
  
  if ( hardware_control ) {
    term_attr.c_cflag |= CRTSCTS;
  }
  
  term_attr.c_cc[ VMIN ] = 1;  // is correct?
  term_attr.c_cc[ VTIME ] = 0;
  
  if ( ::tcsetattr( fd, TCSANOW, &term_attr ) < 0 ) {
    close();
    throw SerialPortException( "::tcsetattr error", __EXCEPTION_INFO__ );
  }
  
  return true;
}

inline
void SerialPort::close() 
  throw ( SerialPortException ) {

  if ( !isOpen() ) {
    return;
  }
  
  if ( ::tcsetattr( fd, TCSANOW, &pre_term_attr ) < 0 ) {
    throw SerialPortException( "::tcsetattr error", __EXCEPTION_INFO__ );
  }
  
  ::close( fd );

  fd = -1;
}

inline  
bool SerialPort::isOpen() const throw () {
  return fd != -1;
}

inline
bool SerialPort::isReadable( uint64_t timeout_usec )
  throw ( SerialPortException, SerialPortTimeoutException ) {
  if ( !isOpen() ) {
    return false;
  }
  
  fd_set rmask;
  FD_ZERO( &rmask );
  FD_SET( fd, &rmask );
  
  timeval waitval;
  waitval.tv_sec = timeout_usec / 1000000;
  waitval.tv_usec = timeout_usec - waitval.tv_sec * 1000000;
  
  int rc = ::select( fd + 1, &rmask, NULL, NULL, &waitval );
  
  if ( rc == 0 ) {
    throw SerialPortTimeoutException( "timeout ::select", __EXCEPTION_INFO__ );
  }
  else if ( rc < 0 ) {
    close();
    throw SerialPortException( "::select error", __EXCEPTION_INFO__ );
  }
  
  return ( FD_ISSET( fd, &rmask ) != 0 );
}


inline
int SerialPort::read( void *data, int size ) throw () {
  if ( !isOpen() ||
       size < 0 ) {
    return -1;
  }

  int ret = ::read( fd, data, size );
  
  if ( size > 0 && ret <= 0 ) {
    close();
  }

  return ret;
}


inline
bool SerialPort::isWritable( uint64_t timeout_usec )
  throw ( SerialPortException, SerialPortTimeoutException ) {
  if ( !isOpen() ) {
    return false;
  }
  
  fd_set wmask;
  FD_ZERO( &wmask );
  FD_SET( fd, &wmask );
  
  timeval waitval;
  waitval.tv_sec = timeout_usec / 1000000;
  waitval.tv_usec = timeout_usec - waitval.tv_sec * 1000000;
  
  int rc = ::select( fd + 1, NULL, &wmask, NULL, &waitval );
  
  if ( rc == 0 ) {
    throw SerialPortTimeoutException( "timeout ::select", __EXCEPTION_INFO__ );
  }
  else if ( rc < 0 ) {
    close();
    throw SerialPortException( "::select error", __EXCEPTION_INFO__ );
  }
  
  return ( FD_ISSET( fd, &wmask ) != 0 );
}

inline
int SerialPort::write( const void *data, int size ) throw () {
  if ( !isOpen() ) {
    return false;
  }

  int ret = ::write( fd, data, size );

  if ( ret < 0 ) {
    close();
  }
  return ret;

}

}
