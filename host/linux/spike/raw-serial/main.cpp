#include <unistd.h>
#include <sys/types.h>  
#include <sys/stat.h>   
#include <fcntl.h> 
#include <termios.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#define PORT        "/dev/ttyACM0"
#include <termios.h>
struct baud_mapping {
    long baud;
    int speed;
};

const char *progname = "hehe";

static struct baud_mapping baud_lookup_table [] = {
  { 1200,   B1200 },
  { 2400,   B2400 },
  { 4800,   B4800 },
  { 9600,   B9600 },
  { 19200,  B19200 },
  { 38400,  B38400 },
  { 57600,  B57600 },
  { 115200, B115200 },
  { 230400, B230400 },
  { 0,      0 }                 /* Terminator. */
};

static speed_t serial_baud_lookup(long baud)
{
  struct baud_mapping *map = baud_lookup_table;

  while (map->baud) {
    if (map->baud == baud)
      return map->speed;
    map++;
  }

  fprintf(stderr, "%s: serial_baud_lookup(): unknown baud rate: %ld\n",
          progname, baud);
  exit(1);
}

int ser_setspeed(int fd, long baud, int flushfirst)
{
  int rc;
  struct termios termios;
  speed_t speed = serial_baud_lookup (baud);

  if (!isatty(fd))
    return -ENOTTY;


  /*
   * initialize terminal modes
   */
  rc = tcgetattr(fd, &termios);
  if (rc < 0) {
    fprintf(stderr, "%s: ser_setspeed(): tcgetattr() failed",
            progname);
    return -errno;
  }

  ///*
  // * copy termios for ser_close if we haven't already
  // */
  //if (! saved_original_termios++) {
  //  original_termios = termios;
  //}



  cfsetospeed(&termios, speed);
  cfsetispeed(&termios, speed);

  rc = tcsetattr(fd, (flushfirst) ? TCSAFLUSH : TCSANOW, &termios);
  if (rc < 0) {
    fprintf(stderr, "%s: ser_setspeed(): tcsetattr() failed\n",
            progname);
    return -errno;
  }
  return 0;
}

int main(int argc, char **argv) {
    int fd = open(PORT,  O_RDWR | O_NOCTTY);
    printf("fd:%d\n", fd);

    struct  termios Opt;
    printf("1. %d\n", tcgetattr(fd, &Opt));
    printf("2. %d\n", cfsetispeed(&Opt, B9600));
    printf("3. %d\n", cfsetospeed(&Opt, B9600));

    Opt.c_iflag = IGNBRK;
    Opt.c_oflag = 0;
    Opt.c_lflag = 0;
    Opt.c_cflag = (CS8 | CREAD | CLOCAL);
    Opt.c_cc[VMIN]  = 1;
    Opt.c_cc[VTIME] = 0;

    printf("4. %d\n", tcsetattr(fd,TCSANOW,&Opt));

    //ser_setspeed(fd, 9600, 1);

    while (1) {
        printf("read\n");

        char buf[32];
        int n = read(fd, buf, sizeof(buf));
        printf("ret:%d\n", n);
        sleep(1);
    }



    printf("started\n");
    return 0;
}