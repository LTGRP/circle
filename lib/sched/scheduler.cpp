//
// scheduler.cpp
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
#include <circle/sched/scheduler.h>
#include <circle/multicore.h>
#include <circle/timer.h>
#include <circle/logger.h>
#include <assert.h>

static const char FromScheduler[] = "sched";

CScheduler *CScheduler::s_pThis = 0;

CScheduler::CScheduler (void)
:	m_SpinLock (TASK_LEVEL)
{
	assert (s_pThis == 0);
	s_pThis = this;

	for (unsigned nCore = 0; nCore < SCHEDULER_CORES; nCore++)
	{
		m_nTasks[nCore] = 0;
		m_nCurrent[nCore] = 0;

		m_pCurrent[nCore] = new CTask (0, nCore);	// main task currently running
		assert (m_pCurrent[nCore] != 0);
	}
}

CScheduler::~CScheduler (void)
{
	for (unsigned nCore = 0; nCore < SCHEDULER_CORES; nCore++)
	{
		assert (m_nTasks[nCore] == 1);
		assert (m_pTask[nCore][0] == m_pCurrent[nCore]);
		RemoveTask (m_pCurrent[nCore], nCore);
		delete m_pCurrent[nCore];
		m_pCurrent[nCore] = 0;
	}

	s_pThis = 0;
}

void CScheduler::Yield (void)
{
#ifdef ARM_ALLOW_MULTI_CORE
	unsigned nCore = CMultiCoreSupport::ThisCore ();
#else
	unsigned nCore = 0;
#endif

	m_SpinLock.Acquire ();

	while ((m_nCurrent[nCore] = GetNextTask (nCore)) == MAX_TASKS)	// no task is ready
	{
		m_SpinLock.Release ();

		assert (m_nTasks[nCore] > 0);

		m_SpinLock.Acquire ();
	}

	assert (m_nCurrent[nCore] < MAX_TASKS);
	CTask *pNext = m_pTask[nCore][m_nCurrent[nCore]];
	assert (pNext != 0);
	if (m_pCurrent[nCore] == pNext)
	{
		m_SpinLock.Release ();

		return;
	}
	
	TTaskRegisters *pOldRegs = m_pCurrent[nCore]->GetRegs ();
	m_pCurrent[nCore] = pNext;
	TTaskRegisters *pNewRegs = m_pCurrent[nCore]->GetRegs ();

	m_SpinLock.Release ();

	assert (pOldRegs != 0);
	assert (pNewRegs != 0);
	TaskSwitch (pOldRegs, pNewRegs);
}

void CScheduler::Sleep (unsigned nSeconds)
{
	// be sure the clock does not run over taken as signed int
	const unsigned nSleepMax = 1800;	// normally 2147 but to be sure
	while (nSeconds > nSleepMax)
	{
		usSleep (nSleepMax * 1000000);

		nSeconds -= nSleepMax;
	}

	usSleep (nSeconds * 1000000);
}

void CScheduler::MsSleep (unsigned nMilliSeconds)
{
	if (nMilliSeconds > 0)
	{
		usSleep (nMilliSeconds * 1000);
	}
}

void CScheduler::usSleep (unsigned nMicroSeconds)
{
	if (nMicroSeconds > 0)
	{
#ifdef ARM_ALLOW_MULTI_CORE
		unsigned nCore = CMultiCoreSupport::ThisCore ();
#else
		unsigned nCore = 0;
#endif

		unsigned nTicks = nMicroSeconds * (CLOCKHZ / 1000000);

		unsigned nStartTicks = CTimer::Get ()->GetClockTicks ();

		assert (m_pCurrent[nCore] != 0);
		assert (m_pCurrent[nCore]->GetState () == TaskStateReady);
		m_pCurrent[nCore]->SetWakeTicks (nStartTicks + nTicks);
		m_pCurrent[nCore]->SetState (TaskStateSleeping);

		Yield ();
	}
}

void CScheduler::AddTask (CTask *pTask, unsigned nCore)
{
	assert (pTask != 0);
	assert (nCore < SCHEDULER_CORES);

	m_SpinLock.Acquire ();

	unsigned i;
	for (i = 0; i < m_nTasks[nCore]; i++)
	{
		if (m_pTask[nCore][i] == 0)
		{
			m_pTask[nCore][i] = pTask;

			m_SpinLock.Release ();

			return;
		}
	}

	if (m_nTasks[nCore] >= MAX_TASKS)
	{
		CLogger::Get ()->Write (FromScheduler, LogPanic,
					"System limit of tasks exceeded (core %u)", nCore);
	}

	m_pTask[nCore][m_nTasks[nCore]++] = pTask;

	m_SpinLock.Release ();
}

void CScheduler::RemoveTask (CTask *pTask, unsigned nCore)	// called with spin lock acquired
{
	for (unsigned i = 0; i < m_nTasks[nCore]; i++)
	{
		if (m_pTask[nCore][i] == pTask)
		{
			m_pTask[nCore][i] = 0;

			if (i == m_nTasks[nCore]-1)
			{
				m_nTasks[nCore]--;
			}

			return;
		}
	}

	assert (0);
}

void CScheduler::BlockTask (CTask **ppTask)
{
#ifdef ARM_ALLOW_MULTI_CORE
	unsigned nCore = CMultiCoreSupport::ThisCore ();
#else
	unsigned nCore = 0;
#endif

	assert (ppTask != 0);
	*ppTask = m_pCurrent[nCore];

	assert (m_pCurrent[nCore] != 0);
	assert (m_pCurrent[nCore]->GetState () == TaskStateReady);
	m_pCurrent[nCore]->SetState (TaskStateBlocked);

	Yield ();
}

void CScheduler::WakeTask (CTask **ppTask)
{
	assert (ppTask != 0);
	CTask *pTask = *ppTask;

	*ppTask = 0;

#ifdef NDEBUG
	if (   pTask == 0
	    || pTask->GetState () != TaskStateBlocked)
	{
		CLogger::Get ()->Write (FromScheduler, LogPanic, "Tried to wake non-blocked task");
	}
#else
	assert (pTask != 0);
	assert (pTask->GetState () == TaskStateBlocked);
#endif

	pTask->SetState (TaskStateReady);
}

void CScheduler::BlockTaskToList (CTask *pTaskList[])
{
	assert (pTaskList != 0);

	unsigned i;
	for (i = 0; i < SCHEDULER_TASKLIST_SIZE; i++)
	{
		if (pTaskList[i] == 0)
		{
			break;
		}
	}
	assert (i < SCHEDULER_TASKLIST_SIZE);

#ifdef ARM_ALLOW_MULTI_CORE
	unsigned nCore = CMultiCoreSupport::ThisCore ();
#else
	unsigned nCore = 0;
#endif

	pTaskList[i] = m_pCurrent[nCore];

	assert (m_pCurrent[nCore] != 0);
	assert (m_pCurrent[nCore]->GetState () == TaskStateReady);
	m_pCurrent[nCore]->SetState (TaskStateBlocked);
}

void CScheduler::WakeTasksFromList (CTask *pTaskList[])
{
	assert (pTaskList != 0);

	for (unsigned i = 0; i < SCHEDULER_TASKLIST_SIZE; i++)
	{
		if (pTaskList[i] != 0)
		{
			WakeTask (&pTaskList[i]);
		}
	}
}

unsigned CScheduler::GetNextTask (unsigned nCore)	// called with spin lock acquired
{
	unsigned nTask = m_nCurrent[nCore] < MAX_TASKS ? m_nCurrent[nCore] : 0;

	unsigned nTicks = CTimer::Get ()->GetClockTicks ();

	for (unsigned i = 1; i <= m_nTasks[nCore]; i++)
	{
		if (++nTask >= m_nTasks[nCore])
		{
			nTask = 0;
		}

		CTask *pTask = m_pTask[nCore][nTask];
		if (pTask == 0)
		{
			continue;
		}

		switch (pTask->GetState ())
		{
		case TaskStateReady:
			return nTask;

		case TaskStateBlocked:
			continue;

		case TaskStateSleeping:
			if ((int) (pTask->GetWakeTicks () - nTicks) > 0)
			{
				continue;
			}
			pTask->SetState (TaskStateReady);
			return nTask;

		case TaskStateTerminated:
			RemoveTask (pTask, nCore);
			delete pTask;
			return MAX_TASKS;

		default:
			assert (0);
			break;
		}
	}

	return MAX_TASKS;
}

CScheduler *CScheduler::Get (void)
{
	assert (s_pThis != 0);
	return s_pThis;
}
