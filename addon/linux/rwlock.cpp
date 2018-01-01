#include <linux/rwlock.h>
#include <circle/sched/readwritelock.h>

void rwlock_init (rwlock_t *lock)
{
	lock->lockobj = new CReadWriteLock;
}

void read_lock_bh (rwlock_t *lock)
{
	CReadWriteLock *pLock = reinterpret_cast<CReadWriteLock *> (lock->lockobj);

	pLock->ReadLock ();
}

void read_unlock_bh (rwlock_t *lock)
{
	CReadWriteLock *pLock = reinterpret_cast<CReadWriteLock *> (lock->lockobj);

	pLock->ReadUnlock ();
}

void write_lock_bh (rwlock_t *lock)
{
	CReadWriteLock *pLock = reinterpret_cast<CReadWriteLock *> (lock->lockobj);

	pLock->WriteLock ();
}

void write_unlock_bh (rwlock_t *lock)
{
	CReadWriteLock *pLock = reinterpret_cast<CReadWriteLock *> (lock->lockobj);

	pLock->WriteUnlock ();
}
