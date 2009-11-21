/*
 * sballserial.h -- Machine/OS dependent serial I/O code for Libsball
 *
 *  Copyright 1997 John E. Stone (j.stone@acm.org)
 *
 *
 *
 */

/*
 * Machine/OS dependent serial port I/O routines.
 *
 * sball_comm_open() - open the serial port device for communication with
 *                   the sball.  Settings are 9600,N,8,1, non-blocking,
 *                   no controlling tty.
 * sball_comm_read() - nonblocking read of up to size bytes
 * sball_comm_write() - blocking write of up to size bytes
 * sball_comm_close() - close the serial port device
 */

typedef void * SBallCommHandle;

int sball_comm_open(char * commname, SBallCommHandle * commhandleptr); 
int sball_comm_write(SBallCommHandle commhandle, char * buf); 
int sball_comm_read(SBallCommHandle commhandle, char * buf, int sz); 
int sball_comm_close(SBallCommHandle * commhandleptr); 

