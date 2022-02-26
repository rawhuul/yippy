#include <asm-generic/ioctls.h>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iterator>
#include <stddef.h>
#include <string>
#include <sys/ioctl.h>
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
  line(string Prompt);
  void refresh(line *s);
  int ifSupported(void);
  size_t getCur_pos(void);
  size_t getColumns(void);
  void clear(void);
};

line::line(string Prompt) { prompt = Prompt; }

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

size_t line::getColumns() {
  struct winsize ws;

  if (ioctl(1, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
    int start, columns;
    start = getCur_pos();
    if (start == -1) {
      return 80;
    }

    if (write(stdout_fd, "\x1b[999C", 6) != 6) {
      return 80;
    }

    columns = getCur_pos();
    if (cols == -1) {
      return 80;
    }

    if (columns > start) {
      char sequence[32];
      snprintf(sequence, 32, "\x1b[%dD", columns - start);
      if (write(stdout_fd, sequence, strlen(sequence)) == -1) {
        cout << "Something Unexpected Happend, exit program." << endl;
        exit(-1);
      }
    }
    return columns;
  } else {
    return ws.ws_col;
  }

  return 80;
}

void line::clear(void) {
  if (write(STDOUT_FILENO, "\x1b[H\x1b[2J", 7) <= 0) {
    return;
  }
}

void linenoiseClearScreen(void) {
  if (write(STDOUT_FILENO, "\x1b[H\x1b[2J", 7) <= 0) {
    /* nothing to do, just to avoid warning. */
  }
}
