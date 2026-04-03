/* $Header: /usr/cvsroot/target/h/wrn/wm/common/thread.h,v 1.2 2003/01/16 18:20:14 josh Exp $ */

/*
 * Copyright (C) 1999-2004 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */

/****************************************************************************
 *  Copyright 2000-2001 Wind River Systems, Inc.
 *  Copyright 1998-1999 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: thread.h,v $
 * Revision 1.2  2003/01/16 18:20:14  josh
 * directory structure shifting
 *
 * Revision 1.1.1.1  2001/11/05 17:47:18  tneale
 * Tornado shuffle
 *
 * Revision 1.13  2001/06/12 08:06:07  paul
 * Mutex priorities are unsigned.
 * Added debugging to condition variable routines.
 *
 * Revision 1.12  2001/03/22 20:29:38  paul
 * The diab compiler is pickier than gcc.
 *
 * Revision 1.11  2001/03/22 16:54:08  paul
 * Mutex and condition variable functions now return errors.
 *
 * Revision 1.10  2001/03/20 17:11:14  paul
 * Removed unused timeout macros.
 *
 * Revision 1.9  2001/01/19 22:21:29  paul
 * Update copyright.
 *
 * Revision 1.8  2000/10/16 19:21:43  paul
 * Restore sockets and mempool code.
 *
 * Revision 1.6  2000/03/17 00:16:41  meister
 * Update copyright message
 *
 * Revision 1.5  2000/03/09 17:15:43  tneale
 * Added #idef C++ to declare extern C if needed
 *
 * Revision 1.4  1999/12/20 23:21:33  qli
 * added ETC_GET_SEL_CONDVAR and ETC_REL_SEL_CONDVAR
 *
 * Revision 1.3  1999/04/28 23:53:24  sra
 * Clean up various minor installation option glitches.
 *
 * Revision 1.2  1999/02/18 04:41:22  wes
 * Sockets merge: Everything Else
 *  - memory pools
 *  - thread support
 *  - port-specific headers
 *
 * Revision 1.1.6.12  1999/01/25 21:08:01  paul
 * added the first half of the copyright notice (oops)
 *
 * Revision 1.1.6.11  1999/01/22 19:48:50  paul
 * Added this_thread(), copyright notice.
 *
 */


/* [clearcase]
modification history
-------------------
*/

#ifndef COMMON_THREAD_H
#define COMMON_THREAD_H

#ifdef __cplusplus
extern"C" {
#endif

#include <thdport.h>

#define ETC_MUTEX_PRIO_MIN  GLUE_MUTEX_PRIO_MIN
#define ETC_MUTEX_PRIO_LOW  GLUE_MUTEX_PRIO_LOW
#define ETC_MUTEX_PRIO_MID  GLUE_MUTEX_PRIO_MID
#define ETC_MUTEX_PRIO_HIGH GLUE_MUTEX_PRIO_HIGH
#define ETC_MUTEX_PRIO_MAX  GLUE_MUTEX_PRIO_MAX

#if defined(INSTALL_THREAD_DEBUG) && INSTALL_THREAD_DEBUG

/*
 * See the non-debug side of this #ifdef of this for documentation on
 * what the various functions do.
 */

/*
 * other things to consider..
 * mutex names.  __FILE__, __LINE__; last-lock/last-unlock info
 * thread names, cond names.
 * yield before/after lock/unlock, signal, cond wait,
 * per thread storage for current lock prio, chain of held locks..
 * How the heck do we deal with META_LOCK/UNLOCK() after cond_wait?
 * (release user lock, take meta_lock, retake user lock?)
 */

typedef struct etc_mutex_t
{
  int			magic;
  glue_mutex_t		m;
  char			*lock_name;
  void 			*lock_owner;
  unsigned		lock_prio;
  struct etc_mutex_t	*next_lock;
  char *last_lock_file;
  int last_lock_line;
  char *last_unlock_file;
  int last_unlock_line;
} etc_mutex_t;

typedef struct 
{
  glue_cond_t		c;
  etc_mutex_t		*m;
} etc_cond_t;

extern glue_mutex_t	etc_meta_mutex;

int etc_mutex_init(etc_mutex_t *lock, unsigned prio, char *name);
int etc_mutex_lock(etc_mutex_t *lock, unsigned prio, char *file, int line);
int etc_mutex_unlock(etc_mutex_t *lock, unsigned prio, char *file, int line);
void etc_mutex_lock_assert (etc_mutex_t *lock, unsigned prio, char *file, int line);
int etc_mutex_destroy (etc_mutex_t *lock, char *file, int line);

int etc_cond_init(etc_cond_t *cond, etc_mutex_t *mutex, char *file, int line);
int etc_cond_wait(etc_cond_t *cond, etc_mutex_t *mutex, char *file, int line);
int etc_cond_wakeup(etc_cond_t *cond, etc_mutex_t *mutex, char *file, int line);
int etc_cond_broadcast(etc_cond_t *cond, etc_mutex_t *mutex, char *file, int line);
int etc_cond_destroy(etc_cond_t *cond, char *file, int line);

#define ETC_LOCK_MAGIC		0x19661008

#define ETC_META_LOCK() \
	GLUE_MUTEX_LOCK(&etc_meta_mutex, GLUE_MUTEX_PRIO_MIN);

#define ETC_META_UNLOCK() \
	GLUE_MUTEX_UNLOCK(&etc_meta_mutex, GLUE_MUTEX_PRIO_MIN);

#define ETC_MUTEX_INIT(lock, prio, name) \
	etc_mutex_init(lock, prio, name)

#define ETC_MUTEX_LOCK(lock, prio) \
	etc_mutex_lock(lock, prio, __FILE__, __LINE__)

#define ETC_MUTEX_UNLOCK(lock, prio) \
	etc_mutex_unlock(lock, prio, __FILE__, __LINE__)

#define ETC_MUTEX_LOCK_ASSERT(lock, prio) \
	etc_mutex_lock_assert(lock, prio, __FILE__, __LINE__)

#define ETC_MUTEX_DESTROY(lock)	\
	etc_mutex_destroy(lock, __FILE__, __LINE__)

#define ETC_COND_INIT(cond, mutex) \
	etc_cond_init(cond, mutex, __FILE__, __LINE__)

#define ETC_COND_WAIT(cond, mutex) \
	etc_cond_wait(cond, mutex, __FILE__, __LINE__)

#define ETC_COND_WAKEUP(cond, mutex) \
	etc_cond_wakeup(cond, mutex, __FILE__, __LINE__)

#define ETC_COND_BROADCAST(cond, mutex) \
	etc_cond_broadcast(cond, mutex, __FILE__, __LINE__)

#define ETC_COND_DESTROY(cond) \
	etc_cond_destroy(cond, __FILE__, __LINE__)

/* Initialize thread system (if necessary) */ 
extern void common_thread_init(void);

#define ETC_THREAD_INIT() common_thread_init()

void *this_thread(void);

#else /* INSTALL_THREAD_DEBUG */

/* Initialize thread system (if necessary) */ 
#define ETC_THREAD_INIT() GLUE_THREAD_INIT()

/*
 * Mutex locks.
 */

typedef glue_mutex_t etc_mutex_t;

/* Initialize mutex */
#define ETC_MUTEX_INIT(lock, prio, name) GLUE_MUTEX_INIT(lock, prio)

/* Lock, unlock mutex */
#define ETC_MUTEX_LOCK(lock, prio) GLUE_MUTEX_LOCK(lock, prio)
#define ETC_MUTEX_UNLOCK(lock, prio) GLUE_MUTEX_UNLOCK(lock, prio)
/* Destroy mutex */
#define ETC_MUTEX_DESTROY(lock)  GLUE_MUTEX_DESTROY(lock)

/* Try to lock a mutex; return 1 if successful, 0 if not (already locked) */
#define ETC_MUTEX_TRYLOCK(lock, prio) GLUE_MUTEX_TRYLOCK(lock)

/* Bug-check if we don't hold the mutex */
#define ETC_MUTEX_LOCK_ASSERT(lock, prio)

/*
 * Condition variables:
 */
 
typedef glue_cond_t etc_cond_t;

/* initialize condition variable */
#define ETC_COND_INIT(cond, mutex) GLUE_COND_INIT(cond, mutex)

/* wait for condition to happen */
#define ETC_COND_WAIT(cond, mutex) GLUE_COND_WAIT(cond, mutex)
#define ETC_COND_DESTROY(cond)  GLUE_COND_DESTROY(cond)

#define ETC_COND_WAKEUP(cond, mutex) GLUE_COND_WAKEUP(cond, mutex)
#define ETC_COND_BROADCAST(cond, mutex) GLUE_COND_BROADCAST(cond, mutex)

#endif

typedef glue_thread_t etc_thread_t;

/* Return a per-thread cookie */
#define ETC_THD_SELF() GLUE_THD_SELF()
/* Yield the processor */ 
#define ETC_THD_YIELD() GLUE_THD_YIELD()

/*
 * the following are mostly for the benefit of snark and similar test code.
 */

/* Create a thread */
#define ETC_THD_CREATE(thread, func, arg) GLUE_THD_CREATE(thread, func, arg)
#define ETC_THD_WAIT(thread) GLUE_THD_WAIT(thread)

/* Cause the current thread to go poof */ 
#define ETC_THD_EXIT() GLUE_THD_EXIT()

#define ETC_GET_SEL_CONDVAR(cookie)  (etc_cond_t *)GLUE_GET_SEL_CONDVAR(cookie)
#define ETC_REL_SEL_CONDVAR(cond, cookie)  GLUE_REL_SEL_CONDVAR(cond, cookie)

#ifdef __cplusplus
}
#endif

#endif
