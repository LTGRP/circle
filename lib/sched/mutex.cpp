//
// mutex.cpp
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
#include <circle/sched/mutex.h>
#include <assert.h>

CMutex::CMutex (void)
:	m_bLocked (FALSE),
	m_SpinLock (TASK_LEVEL)
{
	for (unsigned i = 0; i < SCHEDULER_TASKLIST_SIZE; i++)
	{
		m_pTaskList[i] = 0;
	}
}

CMutex::~CMutex (void)
{
	assert (!m_bLocked);
}

void CMutex::Lock (void)
{
	m_SpinLock.Acquire ();

	while (m_bLocked)
	{
		CScheduler::Get ()->BlockTaskToList (m_pTaskList);

		m_SpinLock.Release ();

		CScheduler::Get ()->Yield ();

		m_SpinLock.Acquire ();
	}

	m_bLocked = TRUE;

	m_SpinLock.Release ();
}

void CMutex::Unlock (void)
{
	m_SpinLock.Acquire ();

	assert (m_bLocked);
	m_bLocked = FALSE;

	CScheduler::Get ()->WakeTasksFromList (m_pTaskList);

	m_SpinLock.Release ();
}
