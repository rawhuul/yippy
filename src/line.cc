#include <cstddef>
#include <cstring>
#include <iostream>
#include <string>
#include <termios.h>

using namespace std;

enum KEY_ACTION {
  KEY_NULL,
  CTRL_A,
  CTRL_B,
  CTRL_C,
  CTRL_D,
  CTRL_E,
  CTRL_F,
  CTRL_H,
  TAB,
  CTRL_K,
  CTRL_L,
  ENTER,
  CTRL_N,
  CTRL_P,
  CTRL_T,
  CTRL_U,
  CTRL_W,
  ESC,
  BACKSPACE
};

class line {
private:
  int stdin_fd;
  int stdout_fd;
  string buffer;
  size_t buffer_len;
  string prompt;
  size_t prompt_len;
  size_t curr_pos;
  size_t old_pos;
  size_t len;
  unsigned int rows;
  unsigned int cols;
  unsigned int hist_index;

public:
  line(string Prompt) { prompt = Prompt; }
  void refresh();
};
