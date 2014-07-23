#ifndef HAWK_CALLBACK_H
#define HAWK_CALLBACK_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* �ص�����Ļ���,����ص�����(����)									*/
	/************************************************************************/
	class UTIL_API HawkCallback : public HawkRefCounter
	{
	public:
		//Invokeִ��ʱ���ò���
		DEFINE_PROPERTY(PVoid, m_pParams, Params);

	public:
		//�ص�������
		HawkCallback() : m_pParams(0) 
		{
		};

		//�ص���������
		virtual ~HawkCallback() 
		{
		};

	public:
		//�ص����󴥷�
		virtual Int32 Invoke(void* pArgs = 0) 
		{
			return 0; 
		}
	};
}
#endif
