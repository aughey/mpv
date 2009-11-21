/*
 * sball.h -- The user accessible API for libsball.  
 *
 *  Copyright 1997-2001 John E. Stone (j.stone@acm.org)
 *
 *
 */

#if !defined(SBALL_H)
#define SBALL_H 1

#ifdef  __cplusplus
extern "C" {
#endif

typedef void * SBallHandle; /* Handle type, used by all sball API functions */

/* Spaceball Button bit-masks */
#define SBALL_BUTTON_1           1   /* bit  0 */
#define SBALL_BUTTON_2           2   /* bit  1 */
#define SBALL_BUTTON_3           4   /* bit  2 */
#define SBALL_BUTTON_4           8   /* bit  3 */
#define SBALL_BUTTON_5          16   /* bit  4 */
#define SBALL_BUTTON_6          32   /* bit  5 */
#define SBALL_BUTTON_7          64   /* bit  6 */
#define SBALL_BUTTON_8         128   /* bit  7 */
#define SBALL_BUTTON_9         256   /* bit  8 */
#define SBALL_BUTTON_10        512   /* bit  9 */
#define SBALL_BUTTON_11       1024   /* bit 10 */
#define SBALL_BUTTON_12       2048   /* bit 11 */

/* The Spaceball 3003 and 3003 FLX only have "left" and "right" buttons */
#define SBALL_BUTTON_LEFT     4096   /* bit 12 */
#define SBALL_BUTTON_RIGHT    8192   /* bit 13 */

/* The Spaceball 2003A and 2003B have a dedicated pick button on the ball */
/* The Spaceball 2003 FLX uses "button 9" as the pick button.             */
/* All of them return this as "button 9" in their encoded button data     */
#define SBALL_BUTTON_PICK      128   /* bit  8 */

/* On Spaceball 2003A and 2003B, the Rezero is "button 8" on the device */
/* On the newer devices, there are dedicated rezero buttons */
#define SBALL_BUTTON_REZERO  16384   /* bit 14 */

/* The Spaceball 4000 FLX has a configurable palm rest which can be in    */
/* either "left" or "right" handed mode.  When it is configured in "left" */
/* handed mode, the "lefty" bit is set, and coordinate systems need to be */
/* inverted on one axis.                                                  */
#define SBALL_MODE_LEFTY     32768   /* bit 15 */

/* 
 * sball_open() 
 *   Open a named serial port which a Spaceball is attached to.
 * Returns a handle which is used by all other sball API functions.
 * If the serial port open fails, or the sball does not pass initialization
 * tests, then a NULL is returned as the handle.   
 */
SBallHandle sball_open(char * sballname); 

/*
 * sball_close()
 *   Closes down the Spaceball serial port, frees allocated resources and
 * discards any unprocessed sball messages.
 */ 
int sball_close(SBallHandle voidhandle);

/*
 * sball_getstatus()
 *   Polls the Spaceball serial port for new packets, performs any optional
 * postprocessing of Spaceball data such as null-region, scaling, and
 * value clamping.  The most recent values for translation, rotation and
 * buttons are stored in the memory locations supplied by the caller.
 * Returns the number of events processed.  If the number of events returned
 * is less than 1, either an error occured or there were no Spaceball
 * events to process.
 */
int sball_getstatus(SBallHandle voidhandle, 
                 int * tx, int * ty, int * tz, 
                 int * rx, int * ry, int * rz, int * buttons);

/*
 * sball_rezero()
 *   Forces the Orb to re-zero itself at the present twist/position.
 * All future event data is relative to this zero point.
 */
int sball_rezero(SBallHandle voidhandle);

/* 
 * sball_init()
 *   Performs a software re-initialization of the Spaceball, clearing
 * all unprocessed events.  Initialization also forces the Orb to re-zero 
 * itself.
 */
int sball_init(SBallHandle voidhandle);

/* 
 * sball_set_nullregion()
 *  Enables null-region processing on Spaceball output.
 * The null-region is the area (centered at 0) around which
 * each coordinate will report zero even when the Spaceball itself
 * reports a number whose absolute value is less than the null region
 * value for that coordinate.  For example, if the null region on the
 * X translation coordinate is set to 50, all sball_getstatus() would report
 * 0 if X is less than 50 and greater than -50.  If X is 51, sball_getstatus
 * would report 1.  If X is -51, sball_getstatus() would report -1.  
 * Null-regions help novice users gradually become accustomed to the 
 * incredible sensitivity of the Spaceball, and make some applications 
 * significantly easier to control.  A resonable default nullregion for all
 * six axes is 65.  Null regions should be tunable by the user, since its
 * likely that not all Spaceballs are quite identical, and it is guaranteed
 * that users have varying levels of manual dexterity.
 * Note that setting the null-region too high significantly reduces the
 * dynamic range of the output values from the Spaceball.
 */
void sball_set_nullregion(SBallHandle voidhandle,
        int nulltx, int nullty, int nulltz,
        int nullrx, int nullry, int nullrz);


#ifdef  __cplusplus
}
#endif

#endif
