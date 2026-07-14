#ifndef RSERRORS_H
#define RSERRORS_H

/*robot service errors*/
#define RSERR_BASE                   1000
#define RSERR_SUCC                   0
#define RSERR_NOT_ENOUGH_RSHD_BUFFER RSERR_BASE + 1
#define RSERR_RSHD_NO_FOUND          RSERR_BASE + 2
#define RSERR_PARAMETER_ERROR        RSERR_BASE + 3
#define RSERR_CREATE_THREAD_ERROR    RSERR_BASE + 4

#endif // RSERRORS_H
