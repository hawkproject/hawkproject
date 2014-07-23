#ifndef HAWK_MATRIX2_H
#define HAWK_MATRIX2_H

#include "HawkGeoDef.h"

namespace Hawk
{
	/************************************************************************/
	/* ��ά�����������                                                     */
	/************************************************************************/	
	class GEO_API HawkMatrix2
	{
	public:
		//����
		HawkMatrix2();

		HawkMatrix2(Float fM00,Float fM01,Float fM10,Float fM11);

		HawkMatrix2(const HawkMatrix2& oMat2);

		//����ָ��
		const Float* Ptr() const;

		Float* Ptr();

		//����������
		Float& operator()(int iRow, int iCol);

		const Float& operator()(int iRow, int iCol) const;

		Float* operator[] (int iRow);

		const Float* operator[] (int iRow) const;

		HawkMatrix2& operator = (const HawkMatrix2 &oMat2);

		HawkMatrix2& operator = (const Float& fScale);

		Bool operator == (const HawkMatrix2& oMat2) const;

		Bool operator != (const HawkMatrix2& oMat2) const;

		HawkMatrix2  operator + (const HawkMatrix2& oMat2) const;

		HawkMatrix2& operator += (const HawkMatrix2& oMat2);

		HawkMatrix2  operator - (const HawkMatrix2& oMat2) const;

		HawkMatrix2& operator -= (const HawkMatrix2& oMat2);

		HawkMatrix2  operator * (const HawkMatrix2& oMat2) const;

		HawkMatrix2& operator *= (const HawkMatrix2& oMat2);

		HawkMatrix2  operator * (const Float& fScale) const;

		HawkMatrix2& operator *= (const Float& fScale);

		//�����ҳ�����(��Ӧopengl)
		HawkVector2D operator * (const HawkVector2D& oVec2) const;

		//�ж��Ƿ�Ϊ��λ����
		Bool  IsIdentity() const;
		
		//��ȡת�þ���
		void  GetTranspose( HawkMatrix2& oMat2 ) const;

		//�����
		void  SetZero();
		
		//��������ʽ
		Float GetDeterminant() const;

		//��������:����� == ת�þ���
		Bool  IsOrthogonalMatrix() const;		

		//�����������(ת�ã����棬ģ���㣬�����ˣ���λ��......)
		//��λ��
		HawkMatrix2& MakeIdentity();

		//��ȡת�þ���
		HawkMatrix2  GetTranspose() const;

		//ת��
		HawkMatrix2& MakeTranspose();

		//��ȡ�����
		HawkMatrix2	 GetInverse() const;
		
		//�������
		HawkMatrix2& MakeInverse();

		//����������
		HawkMatrix2& SetScale(Float fX, Float fY);

		//����������
		HawkMatrix2& SetScale(const HawkVector2D& oVec2);

		//����������
		HawkMatrix2& SetScale(Float fScale);

		//������ת��
		HawkMatrix2& SetRotation(Float fRadian);

		//�����
		HawkVector2D GetRow(Int32 iRow);

		//�����
		HawkVector2D GetColumn(Int32 iCol);
		
		//������
		void   SetRow(Int32 iRow,const HawkVector2D& oVec2);

		//������
		void   SetColumn(Int32 iCol,const HawkVector2D& oVec2);

	public:
		//��������
		union
		{
			struct
			{
				Float M00,M01;
				Float M10,M11;
			};
			Float M[4];
		};
	};
}
#endif
