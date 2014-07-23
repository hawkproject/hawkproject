#ifndef HAWK_STREAMCOMPRESS_H
#define HAWK_STREAMCOMPRESS_H

#include "HawkOctetsStream.h"

namespace Hawk
{
	/************************************************************************/
	/* ��ѹ���ͽ�ѹ�������װ                                               */
	/************************************************************************/
	class UTIL_API HawkStreamCompress : public HawkRefCounter
	{
	public:
		//��ѹ����ѹ����
		HawkStreamCompress();

		//��ѹ����ѹ����
		virtual ~HawkStreamCompress();

	public:
		//��ѹ���ӿ�
		virtual Bool Compress(HawkOctets& xOctets);

		//����ѹ�ӿ�
		virtual Bool Uncompress(HawkOctets& xOctets);

	protected:
		//������ָ��
		PVoid		  m_pStream[2];
		//��ǰ�����������ݴ洢
		OctetsStream  m_sStream;
		//��ǰ���������ݴ洢
		HawkOctets    m_sOctets;
	};
}
#endif
