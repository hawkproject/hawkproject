#include "HawkParamVector.h"
#include "HawkStringUtil.h"
#include "HawkLoggerManager.h"

namespace Hawk
{
	HawkParamVector::HawkParamVector() : m_iSize(0)
	{
	}

	HawkParamVector::~HawkParamVector()
	{
		ClearParams();
	}

	void HawkParamVector::ClearParams()
	{
		HolderVector::iterator it = m_vParam.begin();
		for (;it!=m_vParam.end();it++)
		{
			BaseHolder* pHolder = *it;
			HAWK_RELEASE(pHolder);
		}
		m_vParam.clear();
		m_iSize = 0;
	}

	HawkParamVector& HawkParamVector::operator = (const HawkParamVector& sParam)
	{
		if (this != &sParam)
		{
			Copy(sParam);
		}
		return *this;
	}

	Int32  HawkParamVector::Copy(const HawkParamVector& sParam)
	{
		if (this != &sParam)
		{
			//清空队列数据
			m_iSize = 0;

			//逐次复制
			for (UInt32 i=0;i<sParam.m_iSize;i++)
			{				
				A_Exception(sParam.m_vParam[i] != 0);

				//已存在Holder可直接复制
				if (i < m_vParam.size())
				{
					sParam.m_vParam[i]->Clone(m_vParam[i]);
					m_iSize ++;
				}
				//不存在Holder即新建
				else
				{
					BaseHolder* pHolder = 0;
					sParam.m_vParam[i]->Clone(pHolder);	
					
					HawkAssert(pHolder != 0);
					m_vParam.push_back(pHolder);
					m_iSize ++;
				}
			}
		}		

		return m_iSize;
	}
	
	UInt32 HawkParamVector::Size() const
	{
		return m_iSize;
	}

	void HawkParamVector::Clear()
	{
		m_iSize = 0;
	}
}
