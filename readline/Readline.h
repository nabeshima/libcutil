/**
 * $Id: Readline.h 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef __CUTIL_READLINE_H__
#define __CUTIL_READLINE_H__

extern "C" {
#include <termios.h>
}

#include <fstream>
#include <vector>
#include <string>

namespace cutil {

class Readline {
 private:
  bool is_open;
  termios pre_term_attr;

  int winHeight;
  std::string prompt;
  int prompt_offset;

  std::ofstream history_outf;

  std::vector<std::string> command;
  std::vector<std::string> history;

  bool _open();

  bool _close();

  bool _isOpen() const;

  bool _read(char *c, int timeout_usec);

  void completion(std::string *buf, size_t *cursor_pos);

  void historyNext(std::string *buf, size_t *cursor_pos, size_t *history_i);
  void historyPrevious(std::string *buf, size_t *cursor_pos, size_t *history_i);

  void moveBack(size_t *cursor_pos);
  void moveFront(const std::string &buf, size_t *cursor_pos);
  void moveBackward(size_t *cursor_pos);
  void moveForward(const std::string &buf, size_t *cursor_pos);

  void removeChar(std::string *buf, size_t *cursor_pos);
  void removeLine(std::string *buf, const size_t &cursor_pos);

  bool _get(std::string *result, int timeout_usec);

  Readline();
  ~Readline();

  static Readline &instance();

 public:
  static void setPrompt(const std::string &prompt);
  static bool setHistoryFile(const std::string &history_file);
  static void clearHistory();

  static void addCommand(const std::string &command);
  static void clearCommand();

  static bool get(std::string *result, int timeout_usec = -1);
};
}  // namespace cutil

#include "Readline.ipp"

#endif
