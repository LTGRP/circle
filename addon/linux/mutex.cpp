#include <linux/mutex.h>
#include <circle/sched/mutex.h>

void mutex_init (struct mutex *lock)
{
	lock->mutexobj = new CMutex;
}

void mutex_lock (struct mutex *lock)
{
	CMutex *pMutex = reinterpret_cast<CMutex *> (lock->mutexobj);

	pMutex->Lock ();
}

void mutex_unlock (struct mutex *lock)
{
	CMutex *pMutex = reinterpret_cast<CMutex *> (lock->mutexobj);

	pMutex->Unlock ();
}
