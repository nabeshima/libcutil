/**
 * $Id: Exception.ipp 5 2013-09-25 08:42:57Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

extern "C" {
#include <execinfo.h>
#include <cxxabi.h>
}

#include <sstream>
#include <fstream>

#include "Process.h"

namespace cutil {

namespace {

inline std::string convertToString(int i) {
  std::stringstream sstrm;
  sstrm << i;
  return sstrm.str();
}

std::string getCurrentProcName() {
  std::stringstream command_ps;
  command_ps << "ps -o comm= -p " << ::getpid();

  cutil::Process proc_ps;

  if (!proc_ps.open(command_ps.str())) {
    return std::string("");
  }

  std::string ret;
  proc_ps.stdout() >> ret;

  proc_ps.close();

  return ret;
}

//! retrieve source file name and line using gdb command
bool getGDBInfo(const std::string &proc_name, const std::string &address,
                std::string &src_name, std::string &line_num) {
  //! temporary gdb command file
  std::ofstream outf("/tmp/.gdbcmds");
  outf << "info line *" << address;
  outf.close();

  std::stringstream command_gdb;
  command_gdb << "gdb --batch --quiet -x /tmp/.gdbcmds -s " << proc_name;

  cutil::Process proc_gdb;

  if (!proc_gdb.open(command_gdb.str())) {
    return false;
  }

  std::string dummy;
  proc_gdb.stdout() >> dummy;

  if (dummy == "No") {
    return false;
  }

  proc_gdb.stdout() >> line_num >> dummy >> src_name;

  if (src_name.size() > 2) {  // to support no-gdb environment
    src_name = src_name.substr(1, src_name.size() - 2);
  }

  proc_gdb.close();

  return true;
}
}

inline void Exception::getStackTrace() {
  //! get process name
  std::string proc_name(getCurrentProcName());

  //! flag to hide traces until originally thrown Exception appears
  bool showFlag = (proc_name == "");

  //! address list
  void *addrlist[FUNCLIST_NUM];

  //! retrieve stacktrace
  int addrlen = ::backtrace(addrlist, sizeof(addrlist) / sizeof(addrlist[0]));

  //! convert to symbollist
  char **symbollist = ::backtrace_symbols(addrlist, addrlen);

  for (int i = 1;  //! the first trace is this function getStackTrace()
       i < addrlen; i++) {
    //! parse symbol to address and internal symbol

    std::string dummy, proc, address, symbolstr;

#if defined(__APPLE__) && !defined(__FreeBSD__)
    //! for mac
    std::stringstream sstrm(symbollist[i]);
    sstrm >> dummy >> proc >> address >> symbolstr;

    if (symbolstr == "0x0") {
      continue;
    }
#else
    //! for linux
    std::string str(symbollist[i]);
    std::string::iterator it = str.begin(), endIt = str.end();
    while (it != endIt) {
      switch (*it) {
        case '(':
        case ')':
        case '[':
        case ']':
        case '+':
          *it = ' ';
      }
      ++it;
    }

    std::stringstream sstrm(str);
    sstrm >> proc >> symbolstr >> dummy >> address;
    if (address == "") {
      address = symbolstr;
      symbolstr = "";
    }
#endif

    //! demangle internal symbol to human-readable format symbol.
    size_t funcnamesize = FUNCNAME_NUM;
    char *funcname = (char *)malloc(funcnamesize);
    int status;

    char *ret = abi::__cxa_demangle(symbolstr.c_str(), funcname, &funcnamesize,
                                    &status);
    if (status == 0) {
      symbolstr = ret;
    }

    //! resultant trace.
    std::string msg;

    if (proc_name != "") {
      //! output info from gdb
      std::string src_name, line_num;

      if (getGDBInfo(proc_name, address, src_name, line_num)) {
        std::string src_body(src_name);
        std::string::size_type src_len = src_body.find_last_of("/");

        if (src_len != std::string::npos) {
          src_body = src_body.substr(src_len + 1);
        }

        std::string orig_body(orig_file);
        std::string::size_type orig_len = orig_body.find_last_of("/");
        if (orig_len != std::string::npos) {
          orig_body = orig_body.substr(orig_len + 1);
        }

        if (!showFlag && (orig_body == src_body)) {
          showFlag = true;
        }

        //! store trace message
        if (showFlag) {
          msg = src_name + ":" + line_num + "\t" + symbolstr;
        }
      }
    }

    if (showFlag) {
      if (msg != "") {
        stacktrace.push_back(msg);
      } else {
        stacktrace.push_back(proc + "\t" + address + "\t" + symbolstr);
      }
    }

    ::free(funcname);
  }
  ::free(symbollist);
}

inline Exception::Exception() : std::runtime_error("") {}

inline Exception::Exception(const std::string &msg, const std::string &file,
                            int line, const std::string &func)
    : std::runtime_error(msg),
      orig_file(file),
      orig_line(convertToString(line)),
      orig_func(func) {
#ifndef _NO_STACKTRACE_
  getStackTrace();
#endif
}

inline Exception::Exception(const std::runtime_error &e,
                            const std::string &file, int line,
                            const std::string &func)
    : std::runtime_error(e),
      orig_file(file),
      orig_line(convertToString(line)),
      orig_func(func) {
#ifndef _NO_STACKTRACE_
  getStackTrace();
#endif
}

inline Exception::Exception(const Exception &e)
    : std::runtime_error(e),
      orig_file(e.orig_file),
      orig_line(e.orig_line),
      orig_func(e.orig_func) {
  std::vector<std::string>::const_iterator it = e.stacktrace.begin(),
                                           endIt = e.stacktrace.end();

  while (it != endIt) {
    stacktrace.push_back(*it);
    it++;
  }
}

inline std::string Exception::where() const {
  return orig_file + ":" + orig_line;
}

inline void Exception::printStackTrace(std::ostream *out) const {
  std::vector<std::string>::const_iterator it = stacktrace.begin(),
                                           endIt = stacktrace.end();

  if (it == endIt) {
    (*out) << "No stack trace." << std::endl;
  }

  while (it != endIt) {
    (*out) << *it << std::endl;
    it++;
  }
}
}
