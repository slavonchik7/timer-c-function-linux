

#ifndef SIGTIME_H
#define SIGTIME_H 1

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>


#define tm_seconds double /* type to use milliseconds in set_sigitimer() */


/* the structure stores the error message */
struct st_err_mssg{
    const char *mssg;
    size_t mssglen;
};


/* the structure stores information about timer, signal, handler functions */
struct sigitimer {
    struct itimerval itmval;
    __sighandler_t func_sig_handler_ptr;
    __itimer_which_t itimer_type_value;
    int sigt_num;
};

/* the function sets the timer according to the arguments passed */
struct sigitimer *set_sigitimer(__itimer_which_t itimer_type,
        tm_seconds      interval_msec,
        tm_seconds      value_msec,
        __sighandler_t  func_sig_handler);

/* the function stops the timer, removes the handler signal processing function,
*  clears memory by sigtmval pointer */
int unset_sigitimer(struct sigitimer *sigtmval);


/* a global variable for storing error information in a function
*  set_sigitimer() */
extern struct st_err_mssg sterrmssg;

#endif /* SIGTIME_H */
