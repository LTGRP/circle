#ifndef _linux_rwlock_h
#define _linux_rwlock_h

#include <linux/types.h>

#ifdef __cplusplus
extern "C" {
#endif

// A read-write lock can be acquired by multiple readers,
// but only by one exclusive writer.

typedef struct
{
	void *lockobj;		// opaque pointer
}
rwlock_t;

void rwlock_init (rwlock_t *lock);

void read_lock_bh (rwlock_t *lock);
void read_unlock_bh (rwlock_t *lock);

void write_lock_bh (rwlock_t *lock);
void write_unlock_bh (rwlock_t *lock);

#ifdef __cplusplus
}
#endif

#endif
