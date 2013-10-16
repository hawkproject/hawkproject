#include "HawkProfiler.h"

#ifdef PLATFORM_LINUX

namespace Hawk
{
	struct cpu_stat
	{
		cpu_stat() { memset(this, 0, sizeof(*this)); }

		int user;
		int nice;
		int system;
		int idle;
		int iowait;
		int irq;
		int softirq;
		int stealstolen;
		int guest;		
	};

	struct proc_stat
	{
		proc_stat() { memset(this, 0, sizeof(*this)); }

		int           pid;						/** The process id. **/
		char          exName [MAX_PATH];		/** The filename of the executable **/
		char          state;					/** R is running, S is sleeping, D is sleeping in an uninterruptible wait,Z is zombie, T is traced or stopped **/
		int           ppid;						/** The pid of the parent. **/
		int           pgrp;						/** The pgrp of the process. **/
		int           session;					/** The session id of the process. **/
		int           tty;						/** The tty the process uses **/
		int           tpgid;					/** (too long) **/
		unsigned int  flags;					/** The flags of the process. **/
		unsigned int  minflt;					/** The number of minor faults **/
		unsigned int  cminflt;					/** The number of minor faults with childs **/
		unsigned int  majflt;					/** The number of major faults **/
		unsigned int  cmajflt;					/** The number of major faults with childs **/
		int           utime;					/** user mode jiffies **/
		int           stime;					/** kernel mode jiffies **/
		int			  cutime;					/** user mode jiffies with childs **/
		int           cstime;					/** kernel mode jiffies with childs **/
		int           counter;					/** process's next timeslice **/
		int           priority;					/** the standard nice value, plus fifteen **/
		unsigned int  timeout;					/** The time in jiffies of the next timeout **/
		unsigned int  itrealvalue;				/** The time before the next SIGALRM is sent to the process **/
		int           starttime;				/** Time the process started after system boot **/
		unsigned int  vsize;					/** Virtual memory size **/
		unsigned int  rss;						/** Resident Set Size **/
		unsigned int  rlim;						/** Current limit in bytes on the rss **/
		unsigned int  startcode;				/** The address above which program text can run **/
		unsigned int  endcode;					/** The address below which program text can run **/
		unsigned int  startstack;				/** The address of the start of the stack **/
		unsigned int  kstkesp;					/** The current value of ESP **/
		unsigned int  kstkeip;					/** The current value of EIP **/
		int			  signal;					/** The bitmap of pending signals **/
		int           blocked;					/** The bitmap of blocked signals **/
		int           sigignore;				/** The bitmap of ignored signals **/
		int           sigcatch;					/** The bitmap of catched signals **/
		unsigned int  wchan;					/** (too long) **/
	};

	Bool get_cpu_stat_info(cpu_stat* pInfo)
	{
		HawkDiskFile df;
		if (df.Open("/proc/stat"))
		{
			Char sLine[DEFAULT_SIZE] = {0};
			df.ReadLine(sLine, DEFAULT_SIZE);
			if (sLine[0] == 'c' && sLine[1] == 'p' && sLine[2] == 'u' && sLine[3] == ' ' && sLine[4] == ' ')
			{
				sscanf (&sLine[5], "%d %d %d %d %d %d %d %d %d",
					&(pInfo->user),
					&(pInfo->nice),
					&(pInfo->system),
					&(pInfo->idle),
					&(pInfo->iowait),
					&(pInfo->irq),
					&(pInfo->softirq),
					&(pInfo->stealstolen),
					&(pInfo->guest));

				return true;
			}			
		}
		return false;
	}

	Bool get_proc_stat_info(UInt32 iPid,  proc_stat* pInfo)
	{
		Char sFile[MINI_BUF_SIZE] = {0};
		sprintf(sFile,"/proc/%u/stat",iPid);
		HawkDiskFile df;
		if (df.Open(sFile))
		{
			Char sStat[DEFAULT_SIZE] = {0};
			df.Read(sStat, DEFAULT_SIZE);

			sscanf (sStat, "%u", &(pInfo->pid));
			Char* s = strchr (sStat, '(') + 1;
			Char* t = strchr (sStat, ')');
			strncpy (pInfo->exName, s, t - s);
			pInfo->exName [t - s] = ' ';

			sscanf (t + 2, "%c %d %d %d %d %d %u %u %u %u %u %d %d %d %d %d %d %u %u %d %u %u %u %u %u %u %u %u %d %d %d %d %u",
				&(pInfo->state),
				&(pInfo->ppid),
				&(pInfo->pgrp),
				&(pInfo->session),
				&(pInfo->tty),
				&(pInfo->tpgid),
				&(pInfo->flags),
				&(pInfo->minflt),
				&(pInfo->cminflt),
				&(pInfo->majflt),
				&(pInfo->cmajflt),
				&(pInfo->utime),
				&(pInfo->stime),
				&(pInfo->cutime),
				&(pInfo->cstime),
				&(pInfo->counter),
				&(pInfo->priority),
				&(pInfo->timeout),
				&(pInfo->itrealvalue),
				&(pInfo->starttime),
				&(pInfo->vsize),
				&(pInfo->rss),
				&(pInfo->rlim),
				&(pInfo->startcode),
				&(pInfo->endcode),
				&(pInfo->startstack),
				&(pInfo->kstkesp),
				&(pInfo->kstkeip),
				&(pInfo->signal),
				&(pInfo->blocked),
				&(pInfo->sigignore),
				&(pInfo->sigcatch),
				&(pInfo->wchan));

			return true;
		}
		return false;
	}

	UInt32 get_processor_number()
	{
		static UInt32 iNumber = 0;
		if (!iNumber)
		{
			iNumber = (UInt32)sysconf(_SC_NPROCESSORS_ONLN);
		}		
		return iNumber;
	}	

	UInt64 get_phys_mem()
	{
		static UInt64 iPhysMem = 0;
		if (!iPhysMem)
		{
			struct sysinfo sInfo;
			memset(&sInfo, 0, sizeof(sInfo));
			if (sysinfo(&sInfo) == 0)
				iPhysMem = sInfo.totalram;
		}
		return iPhysMem;
	}

	UInt32 get_cpu_usage()
	{
		static UInt32 iPid = 0;
		if (iPid == 0) 
			iPid = getpid();

		static UInt64 last_proc_time = 0;
		static UInt64 last_cpu_time  = 0;

		cpu_stat cs;
		if (!get_cpu_stat_info(&cs))
			return 0;

		proc_stat ps;
		if (!get_proc_stat_info(iPid,&ps))
			return 0;

		if (last_proc_time == 0 || last_cpu_time == 0)
		{
			last_proc_time = ps.utime + ps.stime + ps.cutime + ps.cstime;
			last_cpu_time  = cs.user + cs.nice + cs.system + cs.idle + cs.iowait + cs.irq + cs.softirq + cs.stealstolen + cs.guest;		
			return 0;
		}

		UInt64	 proc_time = ps.utime + ps.stime + ps.cutime + ps.cstime;
		UInt64   cpu__time = cs.user + cs.nice + cs.system + cs.idle + cs.iowait + cs.irq + cs.softirq + cs.stealstolen + cs.guest;

		UInt64	 proc_time_delta = proc_time - last_proc_time;		
		UInt64   cpu_time_delta  = cpu__time - last_cpu_time;

		if (cpu_time_delta == 0)
			return 0;

		return get_processor_number() * 100 *( proc_time_delta) / cpu_time_delta;
	}

	UInt64 get_memory_usage()
	{
		static UInt32 iPid = 0;
		if (iPid == 0) 
			iPid = getpid();

		Char sFile[MINI_BUF_SIZE] = {0};
		sprintf(sFile,"/proc/%u/status",iPid);
		HawkDiskFile df;
		if (df.Open(sFile))
		{
			Char sLine[DEFAULT_SIZE] = {0};
			while (df.ReadLine(sLine, DEFAULT_SIZE) > 0)
			{
				if (sLine[0] == 'V' && sLine[1] == 'm' && sLine[2] == 'R' && sLine[3] == 'S' && sLine[4] == 'S' && sLine[5] == ':')
				{
					int iMem = atoi(&sLine[6]);
					return iMem * 1024;
				}
				
				memset(sLine, 0, DEFAULT_SIZE);
			}
		}
		return 0;
	}
}
#endif
