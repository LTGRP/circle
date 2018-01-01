#include <linux/semaphore.h>
#include <circle/sched/semaphore.h>
#include <assert.h>

void sema_init (struct semaphore *sem, int val)
{
	assert (val >= 0);
	sem->semaobj = new CSemaphore ((unsigned) val);
}

void down (struct semaphore *sem)
{
	CSemaphore *pSemaphore = reinterpret_cast<CSemaphore *> (sem->semaobj);

	pSemaphore->Down ();
}

void up (struct semaphore *sem)
{
	CSemaphore *pSemaphore = reinterpret_cast<CSemaphore *> (sem->semaobj);

	pSemaphore->Up ();
}

int down_trylock (struct semaphore *sem)
{
	CSemaphore *pSemaphore = reinterpret_cast<CSemaphore *> (sem->semaobj);

	return pSemaphore->DownTryLock () ? 0 : 1;
}
