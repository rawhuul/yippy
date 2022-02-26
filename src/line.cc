#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stddef.h>
#include <string>
#include <termios.h>
#include <unistd.h>

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
  void refresh(line *s);
  int ifSupported(void);
  size_t getCur_pos();
  size_t getColumns;
};

int line::ifSupported(void) {
  string term = getenv("TERM");

  if (!term.length()) {
    return 0;
  } else if (term == "dumb") {
    return 0;
  } else {
    return 1;
  }
}

size_t line::getCur_pos() {
  char buffer[32];
  int row, column;

  unsigned int i = 0;

  if (write(stdout_fd, "\x1b[6n", 4) != 4) {
    return -1;
  }

  while (i < sizeof(buffer) - 1) {
    if (read(stdin_fd, buffer + i, 1) != 1) {
      break;
    } else if (buffer[i] == 'R') {
      break;
    }
    i++;
  }
  buffer[i] = '\0';
  if (buffer[0] != ESC || buffer[1] != '[') {
    return -1;
  }
  if (sscanf(buffer + 2, "%d;%d", &row, &column) != 2) {
    return -1;
  }

  return cols;
}
