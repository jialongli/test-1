#include <termios.h>
#include <unistd.h>

int tcgetattr(int fd, struct termios *termios_p);

