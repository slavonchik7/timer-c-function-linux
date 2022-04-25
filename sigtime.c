
#include "sigtime.h"


static const char INVALID_TIMER_TYPE_NOT_EXIST[] = "invalid timer type, this type does not exist";
static const char INVALID_INTERVAL_LESS_ZERO[] = "invalid argument interval, it is less than 0";
static const char INVALID_VALUE_LESS_ZERO[] = "invalid argument value, it is less than 0";
static const char INVALID_FUNC_HANDLER_NULL[] = "invalid argument function handler, it is null pointer";
static const char ERROR_ALLOC_MEMORY[] = "failed to allocate memory";

/* define the initial parameters */
struct st_err_mssg sterrmssg = {NULL, 0};

/* getting an error in the set_sigitimer() function */
static void sigitimer_error_handler(const char *errmssg);

/* converting values from and milliseconds to timer counter values */
static void tm_scnds_to_suscnds(struct itimerval *itmval,
        tm_seconds interval_msec, tm_seconds value_msec);


struct sigitimer  *set_sigitimer(__itimer_which_t itimer_type,
        tm_seconds      interval_msec,
        tm_seconds      value_msec,
        __sighandler_t  func_sig_handler) {

    int signum; /* to save the signal corresponding to the timer type timer_type */

    /* search for the desired signal corresponding to the timer type timer_type */
    switch (itimer_type) {
        case ITIMER_REAL: {
            signum = SIGALRM;
            break;
        }
        case ITIMER_VIRTUAL: {
            signum = SIGVTALRM;
            break;
        }
        case ITIMER_PROF: {
            signum = SIGPROF;
            break;
        }
        default: {
            sigitimer_error_handler(INVALID_TIMER_TYPE_NOT_EXIST);
            return NULL;
        }
    }

    /* checking  interval_msec  */
    if (interval_msec < 0) {
        sigitimer_error_handler(INVALID_INTERVAL_LESS_ZERO);
        return NULL;
    }

    /* checking  value_msec */
    if (value_msec < 0) {
        sigitimer_error_handler(INVALID_VALUE_LESS_ZERO);
        return NULL;
    }

    if (func_sig_handler == NULL) {
        sigitimer_error_handler(INVALID_FUNC_HANDLER_NULL);
        return NULL;
    }

    /* allocation of memory for a structure that stores timer information */
    struct sigitimer *sigitmval = (struct sigitimer *)malloc(sizeof(struct sigitimer));

    /* checking  pointer */
    if (sigitmval == NULL) {
        sigitimer_error_handler(ERROR_ALLOC_MEMORY);
        return NULL;
    }

    /* filling in the structure */
    sigitmval->sigt_num = signum;
    sigitmval->func_sig_handler_ptr = func_sig_handler;
    sigitmval->itimer_type_value = itimer_type;

    /* filling in the parameters of the timer counter */
    tm_scnds_to_suscnds(&sigitmval->itmval, interval_msec, value_msec);

    setitimer(itimer_type, &sigitmval->itmval, NULL);
    (void)signal(signum, func_sig_handler);

    return sigitmval;
}


static void sigitimer_error_handler(const char *errmssg) {
    sterrmssg.mssg = errmssg;
    sterrmssg.mssglen = strlen(errmssg);
}


int unset_sigitimer(struct sigitimer *sigitmval) {
    if (sigitmval == NULL)
        return -1;

    (void)signal(sigitmval->sigt_num, NULL);

    free(sigitmval);
    sigitmval = NULL;

    return 0;
}


static void tm_scnds_to_suscnds(struct itimerval *itmval,
        tm_seconds interval_msec, tm_seconds value_msec) {

    itmval->it_interval.tv_sec  = (int)interval_msec / 1000;
    itmval->it_interval.tv_usec = ((int)interval_msec % 1000 + interval_msec - (int)interval_msec) * 1000;

    itmval->it_value.tv_sec  = (int)value_msec / 1000;
    itmval->it_value.tv_usec = ((int)value_msec % 1000 + value_msec - (int)value_msec) * 1000;

}


