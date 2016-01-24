/**
 * $Id: Readline.ipp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

extern "C" {
#include <unistd.h>
}

#include <iostream>
#include <algorithm>

#include "Cursor.h"

#include "_key_def.h"

namespace cutil {

namespace {

inline void completion_search(const std::string &key,
                              const std::vector<std::string> &commands,
                              std::string &ret,
                              std::vector<std::string> &candidates) {
  candidates.clear();

  for (unsigned int i = 0; i < commands.size(); ++i) {
    if (commands[i].begin() == std::search(commands[i].begin(),
                                           commands[i].end(), key.begin(),
                                           key.end())) {
      candidates.push_back(commands[i]);
    }
  }

  if (candidates.size() == 0) {
    ret = "";
    return;
  }

  ret = candidates[0];

  for (unsigned int i = 1; i < candidates.size(); ++i) {
    if (ret.size() > candidates[i].size()) {
      std::pair<std::string::iterator, std::string::iterator> it =
          std::mismatch(ret.begin(), ret.end(), candidates[i].begin());
      ret.assign(ret.begin(), it.first);
    } else {
      std::pair<std::string::iterator, std::string::iterator> it =
          std::mismatch(candidates[i].begin(), candidates[i].end(),
                        ret.begin());
      ret.assign(candidates[i].begin(), it.first);
    }
  }
}
}

inline bool Readline::_open() {
  if (_isOpen()) {
    return false;
  }

  termios term_attr;

  if (::tcgetattr(STDIN_FILENO, &term_attr) < 0) {
    _close();
    return false;
  }
  pre_term_attr = term_attr;

  term_attr.c_lflag &= ~(ICANON | ECHO);
  term_attr.c_cc[VMIN] = 1;
  term_attr.c_cc[VTIME] = 0;

  if (::tcsetattr(STDIN_FILENO, TCSANOW, &term_attr) < 0) {
    _close();
    return false;
  }

  winHeight = Cursor::getHeight();

  is_open = true;
  return true;
}

inline bool Readline::_close() {
  if (!_isOpen()) {
    return false;
  }

  if (::tcsetattr(STDIN_FILENO, TCSANOW, &pre_term_attr) < 0) {
    return false;
  }

  if (instance().history_outf) {
    history_outf.close();
  }

  is_open = false;
  return true;
}

inline bool Readline::_isOpen() const { return is_open; }

inline bool Readline::_read(char *c, int timeout_usec) {
  fd_set rmask;
  FD_ZERO(&rmask);
  FD_SET(STDIN_FILENO, &rmask);

  timeval waitval;
  waitval.tv_sec = timeout_usec / 1000000;
  waitval.tv_usec = timeout_usec - waitval.tv_sec * 1000000;

  int rc =
      ::select(0 + 1, &rmask, NULL, NULL, (timeout_usec > 0) ? &waitval : NULL);

  if (rc == 0) {
    return false;
  } else if (rc < 0) {
    _close();
    return false;
  }

  if (::read(STDIN_FILENO, c, sizeof(char)) != sizeof(char)) {
    _close();
    return false;
  }
  return true;
}

inline void Readline::completion(std::string *buf, size_t *cursor_pos) {
  std::string ret;
  std::vector<std::string> candidate;
  completion_search(*buf, command, ret, candidate);

  if (candidate.size() == 0) {
    return;
  }
  if (candidate.size() > 1) {
    std::cout << std::endl;
    for (unsigned int i = 0; i < candidate.size(); ++i) {
      std::cout << candidate[i] << std::endl;
    }
  }

  *buf = ret;
  if (candidate.size() == 1) {
    *buf += " ";
  }

  *cursor_pos = buf->size();

  Cursor::move(1, winHeight);
  std::cout << prompt << *buf << std::flush;
}

inline void Readline::historyNext(std::string *buf, size_t *cursor_pos,
                                  size_t *history_i) {
  if (*history_i != history.size()) {
    ++(*history_i);
  }
  if (*history_i != history.size()) {
    std::string str = history[*history_i];

    Cursor::move(prompt_offset + 1, winHeight);
    for (unsigned int i = 0; i < buf->size(); ++i) {
      std::cout << " ";
    }
    Cursor::move(prompt_offset + 1, winHeight);
    std::cout << str;
    *buf = str;
    *cursor_pos = str.size();
    Cursor::move(prompt_offset + *cursor_pos + 1, winHeight);
  } else {
    Cursor::move(prompt_offset + 1, winHeight);
    for (unsigned int i = 0; i < buf->size(); ++i) {
      std::cout << " ";
    }
    Cursor::move(prompt_offset + 1, winHeight);
    *buf = "";
    *cursor_pos = 0;
  }
}

inline void Readline::historyPrevious(std::string *buf, size_t *cursor_pos,
                                      size_t *history_i) {
  if (*history_i != 0) {
    --(*history_i);
    std::string str = history[*history_i];

    Cursor::move(prompt_offset + 1, winHeight);
    for (unsigned int i = 0; i < buf->size(); ++i) {
      std::cout << " ";
    }
    Cursor::move(prompt_offset + 1, winHeight);
    std::cout << str;
    *buf = str;
    *cursor_pos = str.size();
    Cursor::move(prompt_offset + *cursor_pos + 1, winHeight);
  }
}

inline void Readline::moveBack(size_t *cursor_pos) {
  *cursor_pos = 0;
  Cursor::move(prompt_offset + 1, winHeight);
}

inline void Readline::moveFront(const std::string &buf, size_t *cursor_pos) {
  *cursor_pos = buf.size();
  Cursor::move(prompt_offset + *cursor_pos + 1, winHeight);
}

inline void Readline::moveBackward(size_t *cursor_pos) {
  if (*cursor_pos != 0) {
    --(*cursor_pos);
    Cursor::move(prompt_offset + *cursor_pos + 1, winHeight);
  }
}

inline void Readline::moveForward(const std::string &buf, size_t *cursor_pos) {
  if (*cursor_pos < buf.size()) {
    ++(*cursor_pos);
    Cursor::move(prompt_offset + *cursor_pos + 1, winHeight);
  }
}

inline void Readline::removeChar(std::string *buf, size_t *cursor_pos) {
  for (unsigned int i = *cursor_pos; i < buf->size(); ++i) {
    std::cout << " ";
  }
  Cursor::move(prompt_offset + *cursor_pos + 1, winHeight);

  buf->erase(*cursor_pos, 1);
  if (buf->size() < *cursor_pos) {
    *cursor_pos = buf->size();
  }
  if (*cursor_pos < buf->size()) {
    std::cout << buf->substr(*cursor_pos) << std::flush;
  }
  Cursor::move(prompt_offset + *cursor_pos + 1, winHeight);
}

inline void Readline::removeLine(std::string *buf, const size_t &cursor_pos) {
  for (unsigned int i = cursor_pos; i < buf->size(); ++i) {
    std::cout << " ";
  }
  *buf = buf->substr(0, cursor_pos);
  Cursor::move(prompt_offset + cursor_pos + 1, winHeight);
}

inline bool Readline::_get(std::string *result, int timeout_usec) {
  if (!_isOpen()) {
    return false;
  }

  std::string buf;

  size_t history_i = history.size();
  int key_state = -1;
  size_t cursor_pos = 0;

  std::cout << prompt << std::flush;

  while (true) {
    char c;
    if (!_read(&c, timeout_usec)) {
      return false;
    }

    //    std::cout << c << "\t" (int)c << std::endl;
    //    continue;

    winHeight = Cursor::getHeight();

    // keyboard callback
    switch (c) {
      case RL_KEY_TAB:
        completion(&buf, &cursor_pos);
        break;
      case RL_KEY_CTRL_a:
        moveBack(&cursor_pos);
        break;
      case RL_KEY_CTRL_b:
        moveBackward(&cursor_pos);
        break;
      case RL_KEY_CTRL_d:
        removeChar(&buf, &cursor_pos);
        break;
      case RL_KEY_CTRL_e:
        moveFront(buf, &cursor_pos);
        break;
      case RL_KEY_CTRL_f:
        moveForward(buf, &cursor_pos);
        break;
      case RL_KEY_CTRL_k:
        removeLine(&buf, cursor_pos);
        break;
      case RL_KEY_CTRL_n:
        historyNext(&buf, &cursor_pos, &history_i);
        break;
      case RL_KEY_CTRL_p:
        historyPrevious(&buf, &cursor_pos, &history_i);
        break;
      case RL_KEY_BS:
        moveBackward(&cursor_pos);
        removeChar(&buf, &cursor_pos);
        break;
      default:
        if (key_state == -1 &&
            (c == RL_CURSOR_KEY_FIRST || c == RL_EXTRA_KEY_FIRST)) {
          key_state = 0;
          break;
        }

        if (key_state == 0 &&
            (c == RL_CURSOR_KEY_SECOND || c == RL_EXTRA_KEY_SECOND)) {
          key_state = 1;
          break;
        }

        if (key_state == 1 && c == RL_EXTRA_KEY_THIRD) {
          key_state = 2;
          break;
        }

        if (key_state == 1) {
          switch (c) {
            case RL_CURSOR_KEY_UP:
              historyPrevious(&buf, &cursor_pos, &history_i);
              break;
            case RL_CURSOR_KEY_DOWN:
              historyNext(&buf, &cursor_pos, &history_i);
              break;
            case RL_CURSOR_KEY_LEFT:
              moveBackward(&cursor_pos);
              break;
            case RL_CURSOR_KEY_RIGHT:
              moveForward(buf, &cursor_pos);
              break;
          }
          key_state = -1;
          break;
        }

        if (key_state == 2 && c == RL_EXTRA_KEY_DELETE) {
          removeChar(&buf, &cursor_pos);
          key_state = -1;
          break;
        }

        if (c == '\n' || c == '\r') {
          std::cout << std::endl;

          // eliminate the last white space
          {
            int i = buf.size() - 1;
            for (; i >= 0; --i) {
              if (buf[i] != ' ') {
                break;
              }
            }
            buf = buf.substr(0, i + 1);
          }

          if (buf != "" && (history.size() == 0 || buf != history.back())) {
            history.push_back(buf);
            history_outf << buf << std::endl;
          }
          *result = buf;
          return true;
        }

        if (cursor_pos < buf.size()) {
          for (unsigned int i = cursor_pos; i < buf.size(); ++i) {
            std::cout << " ";
          }
          Cursor::move(prompt_offset + cursor_pos + 1, winHeight);

          buf.insert(cursor_pos, 1, c);
          std::cout << buf.substr(cursor_pos) << std::flush;
        } else {
          buf += c;
          std::cout << c << std::flush;
        }
        ++cursor_pos;
        Cursor::move(prompt_offset + cursor_pos + 1, winHeight);
        key_state = -1;
    }
  }

  return false;
}

inline Readline::Readline() : is_open(false), winHeight(0), prompt_offset(0) {
  _open();
}

inline Readline::~Readline() { _close(); }

inline Readline &Readline::instance() {
  static Readline readline;
  return readline;
}

inline void Readline::setPrompt(const std::string &prompt) {
  instance().prompt = prompt;
  instance().prompt_offset = instance().prompt.size();
}

inline void Readline::addCommand(const std::string &command) {
  instance().command.push_back(command);
  std::sort(instance().command.begin(), instance().command.end());
}

inline bool Readline::setHistoryFile(const std::string &history_file) {
  instance().history.clear();
  if (instance().history_outf) {
    instance().history_outf.close();
  }

  std::ifstream inf(history_file.c_str());

  while (inf) {
    std::string line;
    std::getline(inf, line);
    if (line != "") {
      instance().history.push_back(line);
    }
  }
  inf.close();

  instance().history_outf.open(history_file.c_str(), std::ios::app);
  if (!instance().history_outf) {
    return false;
  }

  return true;
}

inline void Readline::clearHistory() { instance().history.clear(); }

inline void Readline::clearCommand() { instance().command.clear(); }

inline bool Readline::get(std::string *result, int timeout_usec) {
  return instance()._get(result, timeout_usec);
}
}

#include "_key_undef.h"
