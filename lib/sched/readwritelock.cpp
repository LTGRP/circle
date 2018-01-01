//
// readwritelock.cpp
//
// Circle - A C++ bare metal environment for Raspberry Pi
// Copyright (C) 2017  R. Stange <rsta2@o2online.de>
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
#include <circle/sched/readwritelock.h>
#include <assert.h>

#define WRITE_LOCK	(1U << 31)

CReadWriteLock::CReadWriteLock (void)
:	m_nLock (0),
	m_SpinLock (TASK_LEVEL)
{
	for (unsigned i = 0; i < SCHEDULER_TASKLIST_SIZE; i++)
	{
		m_pTaskList[i] = 0;
	}
}

CReadWriteLock::~CReadWriteLock (void)
{
	assert (m_nLock == 0);
}

void CReadWriteLock::ReadLock (void)
{
	m_SpinLock.Acquire ();

	m_nLock++;

	while (m_nLock >= WRITE_LOCK)
	{
		CScheduler::Get ()->BlockTaskToList (m_pTaskList);

		m_SpinLock.Release ();

		CScheduler::Get ()->Yield ();

		m_SpinLock.Acquire ();
	}

	m_SpinLock.Release ();
}

void CReadWriteLock::ReadUnlock (void)
{
	m_SpinLock.Acquire ();

	m_nLock--;

	CScheduler::Get ()->WakeTasksFromList (m_pTaskList);

	m_SpinLock.Release ();
}

void CReadWriteLock::WriteLock (void)
{
	m_SpinLock.Acquire ();

	m_nLock |= WRITE_LOCK;

	while ((m_nLock & ~WRITE_LOCK) != 0)
	{
		CScheduler::Get ()->BlockTaskToList (m_pTaskList);

		m_SpinLock.Release ();

		CScheduler::Get ()->Yield ();

		m_SpinLock.Acquire ();
	}

	m_SpinLock.Release ();
}

void CReadWriteLock::WriteUnlock (void)
{
	m_SpinLock.Acquire ();

	m_nLock &= ~WRITE_LOCK;

	CScheduler::Get ()->WakeTasksFromList (m_pTaskList);

	m_SpinLock.Release ();
}
