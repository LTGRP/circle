#include <linux/linuxemu.h>
#include <linux/timer.h>
#include <linux/kthread.h>

int linuxemu_init (unsigned kthread_core)
{
	static int init_done = 0;
	if (init_done)
	{
		return 0;
	}
	init_done = 1;

	int ret = linuxemu_init_timer ();
	if (ret != 0)
	{
		return ret;
	}

	ret = linuxemu_init_kthread (kthread_core);
	if (ret != 0)
	{
		return ret;
	}

	return 0;
}
