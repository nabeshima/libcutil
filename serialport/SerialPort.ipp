extern "C" {
#include <sys/ioctl.h>
#include <unistd.h>
}

namespace cutil {

inline SerialPort::SerialPort() : fd(-1) {}

inline SerialPort::~SerialPort() { close(); }

inline bool SerialPort::open(const std::string &portname, int baudrate,
                             DataBit databit, ParityBit paritybit,
                             StopBit stopbit, bool hardware_control) {
  if (isOpen()) {
    close();
  }

  fd = ::open(portname.c_str(), O_RDWR | O_NOCTTY);

  if (fd == -1) {
    return false;
  }

  termios term_attr;

  if (::tcgetattr(fd, &term_attr) < 0) {
    _close();
    return false;
  }
  pre_term_attr = term_attr;

  if (::cfsetispeed(&term_attr, baudrate) != 0 ||
      ::cfsetospeed(&term_attr, baudrate) != 0) {
    _close();
    return false;
  }

  term_attr.c_iflag = 0;
  term_attr.c_oflag = 0;

  term_attr.c_cflag &= ~(CSIZE | PARENB | CSTOPB);  // clear
  term_attr.c_cflag |= databit | paritybit | stopbit | CREAD | CLOCAL;

// non-canonical mode.
#if defined(__APPLE__)
  // IEXTEN should be off to ignore extended special characters for Mac OS X
  term_attr.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG | IEXTEN);
#else
  term_attr.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
#endif

  if (hardware_control) {
    term_attr.c_cflag |= CRTSCTS;
  }

  term_attr.c_cc[VMIN] = 1;
  term_attr.c_cc[VTIME] = 0;

  if (::tcsetattr(fd, TCSANOW, &term_attr) < 0) {
    _close();
    return false;
  }

  return true;
}

inline bool SerialPort::close() {
  if (!isOpen()) {
    return false;
  }

  ::tcsetattr(fd, TCSANOW, &pre_term_attr);
  _close();

  return true;
}

inline void SerialPort::_close() {
  if (!isOpen()) {
    return;
  }

  ::close(fd);

  fd = -1;
}

inline bool SerialPort::isOpen() const { return fd != -1; }

inline bool SerialPort::isReadable(uint64_t timeout_usec) {
  if (!isOpen()) {
    return false;
  }

  fd_set rmask;
  FD_ZERO(&rmask);
  FD_SET(fd, &rmask);

  timeval waitval;
  waitval.tv_sec = timeout_usec / 1000000;
  waitval.tv_usec = timeout_usec - waitval.tv_sec * 1000000;

  int rc = ::select(fd + 1, &rmask, NULL, NULL, &waitval);

  if (rc == 0) {
    return false;
  } else if (rc < 0) {
    _close();
    return false;
  }

  return (FD_ISSET(fd, &rmask) != 0);
}

inline int SerialPort::read(void *data, int size) {
  if (!isOpen() || size < 0) {
    return -1;
  }

  int ret = ::read(fd, data, size);

  if (size > 0 && ret <= 0) {
    _close();
  }

  return ret;
}

inline bool SerialPort::isWritable(uint64_t timeout_usec) {
  if (!isOpen()) {
    return false;
  }

  fd_set wmask;
  FD_ZERO(&wmask);
  FD_SET(fd, &wmask);

  timeval waitval;
  waitval.tv_sec = timeout_usec / 1000000;
  waitval.tv_usec = timeout_usec - waitval.tv_sec * 1000000;

  int rc = ::select(fd + 1, NULL, &wmask, NULL, &waitval);

  if (rc == 0) {
    return false;
  } else if (rc < 0) {
    _close();
    return false;
  }

  return (FD_ISSET(fd, &wmask) != 0);
}

inline int SerialPort::write(const void *data, int size) {
  if (!isOpen()) {
    return false;
  }

  int ret = ::write(fd, data, size);

  if (ret < 0) {
    _close();
  }
  return ret;
}

inline int SerialPort::bytesInReadBuffer() {
  if (!isOpen()) {
    return 0;
  }

  int num;

  if (::ioctl(fd, FIONREAD, &num) != 0) {
    return -1;
  }

  return num;
}

inline void SerialPort::flushReadBuffer() {
  if (isOpen()) {
    ::tcflush(fd, TCIFLUSH);
  }
}

inline void SerialPort::flushWriteBuffer() {
  if (isOpen()) {
    ::tcflush(fd, TCOFLUSH);
  }
}
}  // namespace cutil
