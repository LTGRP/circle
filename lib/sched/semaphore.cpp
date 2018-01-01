//
// semaphore.cpp
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
#include <circle/sched/semaphore.h>
#include <assert.h>

CSemaphore::CSemaphore (unsigned nInit)
:	m_nCount (nInit),
	m_SpinLock (IRQ_LEVEL)
{
	assert (m_nCount >= 0);

	for (unsigned i = 0; i < SCHEDULER_TASKLIST_SIZE; i++)
	{
		m_pTaskList[i] = 0;
	}
}

CSemaphore::~CSemaphore (void)
{
	assert (m_nCount > 0);
}

void CSemaphore::Down (void)
{
	m_SpinLock.Acquire ();

	while (m_nCount == 0)
	{
		CScheduler::Get ()->BlockTaskToList (m_pTaskList);

		m_SpinLock.Release ();

		CScheduler::Get ()->Yield ();

		m_SpinLock.Acquire ();
	}

	m_nCount--;

	m_SpinLock.Release ();
}

void CSemaphore::Up (void)
{
	m_SpinLock.Acquire ();

	m_nCount++;

	CScheduler::Get ()->WakeTasksFromList (m_pTaskList);

	m_SpinLock.Release ();
}

boolean CSemaphore::DownTryLock (void)
{
	m_SpinLock.Acquire ();

	if (m_nCount == 0)
	{
		m_SpinLock.Release ();

		return FALSE;
	}

	m_nCount--;

	m_SpinLock.Release ();

	return TRUE;
}
