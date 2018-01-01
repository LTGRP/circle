//
// semaphore.h
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
#ifndef _circle_sched_semaphore_h
#define _circle_sched_semaphore_h

#include <circle/sched/scheduler.h>
#include <circle/sched/task.h>
#include <circle/spinlock.h>
#include <circle/types.h>

class CSemaphore
{
public:
	CSemaphore (unsigned nInit = 1);
	~CSemaphore (void);

	void Down (void);
	void Up (void);

	boolean DownTryLock (void);		// returns TRUE if semaphore has been locked

private:
	volatile unsigned m_nCount;

	CTask *m_pTaskList[SCHEDULER_TASKLIST_SIZE];

	CSpinLock m_SpinLock;
};

#endif
