#include <selinux/selinux.h>
#include <pthread.h>

extern int selinux_page_size;

/* Make pthread_once optional */
#pragma weak pthread_once
#pragma weak pthread_key_create
#pragma weak pthread_key_delete
#pragma weak pthread_setspecific

/* Call handler iff the first call.  */
#define __selinux_once(ONCE_CONTROL, INIT_FUNCTION)	\
	do {						\
		if (pthread_once != NULL)		\
			pthread_once (&(ONCE_CONTROL), (INIT_FUNCTION));  \
		else if ((ONCE_CONTROL) == PTHREAD_ONCE_INIT) {		  \
			INIT_FUNCTION ();		\
			(ONCE_CONTROL) = 2;		\
		}					\
	} while (0)

/* Pthread key macros */
#define __selinux_key_create(KEY, DESTRUCTOR)			\
	do {							\
		if (pthread_key_create != NULL)			\
			pthread_key_create(KEY, DESTRUCTOR);	\
	} while (0)

#define __selinux_key_delete(KEY)				\
	do {							\
		if (pthread_key_delete != NULL)			\
			pthread_key_delete(KEY);		\
	} while (0)

#define __selinux_setspecific(KEY, VALUE)			\
	do {							\
		if (pthread_setspecific != NULL)		\
			pthread_setspecific(KEY, VALUE);	\
	} while (0)


