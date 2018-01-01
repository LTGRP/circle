#include <linux/completion.h>
#include <linux/jiffies.h>
#include <circle/sched/scheduler.h>

void complete (struct completion *x)
{
	x->done++;
}

void complete_all (struct completion *x)
{
	x->done = (unsigned) -1 / 2;
}

void wait_for_completion (struct completion *x)
{
	while (x->done == 0)
	{
		CScheduler::Get ()->Yield ();
	}

	x->done--;
}

int try_wait_for_completion (struct completion *x)
{
	if (x->done == 0)
	{
		return 0;
	}

	x->done--;

	return 1;
}

long wait_for_completion_interruptible_timeout (struct completion *x, unsigned long timeout)
{
	unsigned long start = jiffies;

	while (x->done == 0)
	{
		unsigned long now = jiffies;
		if (now-start >= timeout)
		{
			return 0;
		}

		CScheduler::Get ()->Yield ();
	}

	x->done--;

	return 1;
}
