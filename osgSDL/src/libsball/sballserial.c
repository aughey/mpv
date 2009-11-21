/*
 * sballserial.c -- Machine/OS dependent serial I/O code for Libsball
 *
 *  Copyright 1997 (unix code)  John E. Stone (j.stone@acm.org)
 *                 (Win32 code) Mike Connell  (M.Connell@swansea.ac.uk)  
 *
 *
 *
 */

/*
 * modified by Marco Jez for Win32 compatibility. Some
 * include paths have been changed to fit the current
 * project.
 *                                           (9/7/2002)
 */

//#define BLOCKING_SPACEBALL_COMM_

#if !defined(WIN32) && !defined(_MSC_VER)

/*
 * Unix serial I/O code by John Stone
 */

#define _POSIX_SOURCE 1

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "sballserial.h" /* protos and types for this file */

typedef struct {
  int fd;        /* serial port device file descriptor */
} commstruct;

int sball_comm_open(char * commname, SBallCommHandle * commhandleptr) {
  struct termios sballtermio;
  commstruct * comm;

  *commhandleptr = NULL;

  comm = malloc(sizeof(commstruct));
  if (comm == NULL)
    return -1;

  comm->fd = open(commname, O_RDWR | O_NONBLOCK | O_NOCTTY);

  if (comm->fd == -1) {
    free(comm);
    return -1;  /* failed open of comm port */
  }
  tcgetattr(comm->fd, &sballtermio);

#if 0
  /* TIOCEXCL exclusive access by this process */
#if defined(TIOCEXCL)
  if (ioctl(comm->fd, TIOCEXCL) < 0) {
    return -1; /* couldn't get exclusive use of port */
  }
#endif
#endif

  sballtermio.c_lflag = 0;
  sballtermio.c_lflag = 0;
  sballtermio.c_iflag = IGNBRK | IGNPAR;
  sballtermio.c_oflag = 0;
  sballtermio.c_cflag = CREAD | CS8 | CLOCAL | HUPCL;
  sballtermio.c_cc[VEOL] = '\r';
  sballtermio.c_cc[VERASE] = 0;
  sballtermio.c_cc[VKILL] = 0;
  sballtermio.c_cc[VMIN] = 0;
  sballtermio.c_cc[VTIME] = 0;

  /* use of baud rate in cflag is deprecated according to the */
  /* single unix spec, also doesn't work in IRIX > 6.2        */
  cfsetispeed(&sballtermio, B9600);
  cfsetospeed(&sballtermio, B9600);

  tcsetattr(comm->fd, TCSAFLUSH, &sballtermio);

  *commhandleptr = (SBallCommHandle) comm;
 
  return 0;
}

int sball_comm_write(SBallCommHandle commhandle, char * buf) {
  commstruct * comm = (commstruct *) commhandle;

  if (comm == NULL) 
    return -1;

  return write(comm->fd, buf, strlen(buf));
}

int sball_comm_read(SBallCommHandle commhandle, char * buf, int sz) {
  commstruct * comm = (commstruct *) commhandle;

  if (comm == NULL) 
    return -1;

  return read(comm->fd, buf, sz);
}

int sball_comm_close(SBallCommHandle * commhandleptr) {
  commstruct * comm = (commstruct *) (*commhandleptr);

  if (comm == NULL)
    return -1;

  close(comm->fd);

  free(*commhandleptr);
  *commhandleptr = NULL;

  return 0;
}

/* end of unix code */

#else

/* 
 * Win32 serial code by Marco Jez
 */

#include <string.h>
#include <windows.h>
#include "sballserial.h"

int sball_comm_open(char * commname, SBallCommHandle * commhandleptr)
{
	DCB dcb;
	COMMTIMEOUTS timeouts;

	*commhandleptr = CreateFile(commname, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
	if (*commhandleptr == INVALID_HANDLE_VALUE) return -1;

	if (!GetCommState(*commhandleptr, &dcb)) return -1;

	dcb.BaudRate = CBR_9600;
	dcb.ByteSize = 8;        
	dcb.Parity = NOPARITY;    
	dcb.StopBits = ONESTOPBIT;

	if (!SetCommState(*commhandleptr, &dcb)) return -1;

	if (!GetCommTimeouts(*commhandleptr, &timeouts)) return -1;

	timeouts.ReadIntervalTimeout = 0;
	timeouts.ReadTotalTimeoutConstant = 0;
	timeouts.ReadTotalTimeoutMultiplier = 0;

	if (!SetCommTimeouts(*commhandleptr, &timeouts)) return -1;

#	ifdef BLOCKING_SPACEBALL_COMM_
	SetCommMask(*commhandleptr, EV_RXCHAR);
#	endif

	return 0;
}

int sball_comm_write(SBallCommHandle commhandle, char * buf)
{
	unsigned long nw = 0;	
	if (!WriteFile(commhandle, buf, (DWORD)strlen(buf), &nw, 0)) return -1;	
	return nw;
}

int sball_comm_read(SBallCommHandle commhandle, char * buf, int sz)
{
	COMSTAT status; 	
	unsigned long etat;
	unsigned long nr = 0;

#	ifdef BLOCKING_SPACEBALL_COMM_
	DWORD mask;
	WaitCommEvent(commhandle, &mask, 0);
#	endif

	if (!ClearCommError(commhandle, &etat, &status)) return -1;
	if (!ReadFile(commhandle, buf, (sz < (int)status.cbInQue) ? sz: status.cbInQue, &nr, 0)) return -1;

	return nr;
}

int sball_comm_close(SBallCommHandle * commhandleptr)
{
	if (CloseHandle(*commhandleptr) != S_OK) return -1;
	*commhandleptr = 0;
	return 0;
}

/* end of Win32 code */

#endif 


