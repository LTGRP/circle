//
// scheduler.h
//
// Circle - A C++ bare metal environment for Raspberry Pi
// Copyright (C) 2015-2017  R. Stange <rsta2@o2online.de>
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#ifndef _circle_sched_scheduler_h
#define _circle_sched_scheduler_h

#include <circle/sched/task.h>
#include <circle/sysconfig.h>
#include <circle/spinlock.h>
#include <circle/types.h>

#ifdef ARM_ALLOW_MULTI_CORE
	#define SCHEDULER_CORES		CORES
#else
	#define SCHEDULER_CORES		1
#endif

class CScheduler				// simple cooperative (non-preemtive) scheduler
{
public:
	CScheduler (void);
	~CScheduler (void);

	void Yield (void);			// switch to next task

	void Sleep (unsigned nSeconds);
	void MsSleep (unsigned nMilliSeconds);
	void usSleep (unsigned nMicroSeconds);

	static CScheduler *Get (void);

	static boolean IsActive (void)
	{
		return s_pThis != 0 ? TRUE : FALSE;
	}

private:
	void AddTask (CTask *pTask, unsigned nCore);
	friend class CTask;

	void BlockTask (CTask **ppTask);
	void WakeTask (CTask **ppTask);		// can be called from interrupt context
	friend class CSynchronizationEvent;

#define SCHEDULER_TASKLIST_SIZE		5
	void BlockTaskToList (CTask *pTaskList[]);
	void WakeTasksFromList (CTask *pTaskList[]);
	friend class CMutex;
	friend class CSemaphore;
	friend class CReadWriteLock;

	void RemoveTask (CTask *pTask, unsigned nCore);
	unsigned GetNextTask (unsigned nCore);	// returns index into m_pTask or MAX_TASKS if no task was found

private:
	CTask *m_pTask[SCHEDULER_CORES][MAX_TASKS];
	unsigned m_nTasks[SCHEDULER_CORES];

	CTask *m_pCurrent[SCHEDULER_CORES];
	unsigned m_nCurrent[SCHEDULER_CORES];	// index into m_pTask

	CSpinLock m_SpinLock;

	static CScheduler *s_pThis;
};

#endif
