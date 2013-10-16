#include "HawkTask.h"

namespace Hawk
{
	HawkTask::HawkTask(PThreadRoutine pRoutine, PVoid pParam) : m_pRoutine(pRoutine), m_pParam(pParam)
	{
	}

	HawkTask::~HawkTask()
	{
	}

	PVoid HawkTask::GetParam()
	{
		return m_pParam;
	}

	PVoid HawkTask::Run(void* pArgs)
	{
		if(m_pRoutine)
			return (*m_pRoutine)(pArgs? pArgs : m_pParam);
		
		return 0;
	}

	Int32 HawkTask::Close()
	{
		return 0;
	}
}
