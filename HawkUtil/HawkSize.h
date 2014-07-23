#ifndef HAWK_SIZE_H
#define HAWK_SIZE_H

#include "HawkMath.h"

namespace Hawk
{
	/************************************************************************/
	/* 尺寸表示类，用来表示带宽高的属性                                     */
	/************************************************************************/
	template <class T> class HawkSize
	{
		//属性定义
		DEFINE_PROPERTY(T,	Width,	Width);
		DEFINE_PROPERTY(T,	Height,	Height);

	public:
		inline HawkSize(T tW = 0, T tH = 0) : Width(tW), Height(tH) 
		{
		};

		inline void Set(T tW = 0, T tH = 0)
		{
			Width  = tW;
			Height = tH;
		}

		inline HawkSize<T>& operator = (const HawkSize& oSz)
		{
			if (this != &oSz)
			{
				Width  = oSz.Width;
				Height = oSz.Height;
			}
			return *this;
		}

		inline HawkSize<T>& operator = (T tVal)
		{
			Width  = tVal;
			Height = tVal;
			return *this;
		}

		inline Bool operator == (const HawkSize& oSz) const
		{
			return (Width == oSz.Width && Height == oSz.Height);
		}

		inline Bool operator != (const HawkSize& oSz) const
		{
			return (Width != oSz.Width || Height != oSz.Height );
		}

		inline HawkSize<T> operator + (const HawkSize& oSz) const
		{
			return HawkSize<T>(Width + oSz.Width, Height + oSz.Height);
		}

		inline HawkSize<T> operator - (const HawkSize& oSz) const
		{
			return HawkSize<T>(Width - oSz.Width, Height - oSz.Height);
		}

		inline HawkSize<T> operator * (const Float fScale) const
		{
			return HawkSize<T>(Width * fScale, Height * fScale);
		}

		inline HawkSize<T> operator / ( const Float fScale ) const
		{
			HawkAssert(fScale != 0);
			Float fInv = 1.0f / fScale;
			return HawkSize<T>(Width * fInv,Height * fInv);
		}

		inline HawkSize<T>& operator += ( const HawkSize& oSz )
		{
			Width  += oSz.Width;
			Height += oSz.Height;
			return *this;
		}

		inline HawkSize<T>& operator += (T tVal)
		{
			Width  += tVal;
			Height += tVal;
			return *this;
		}

		inline HawkSize<T>& operator -= ( const HawkSize& oSz )
		{
			Width  -= oSz.Width;
			Height -= oSz.Height;
			return *this;
		}

		inline HawkSize<T>& operator -= ( T tVal )
		{
			Width  -= tVal;
			Height -= tVal;
			return *this;
		}

		inline HawkSize<T>& operator *= ( const Float fScale )
		{
			Width  *= fScale;
			Height *= fScale;
			return *this;
		}

		inline HawkSize<T>& operator /= ( const Float fScale )
		{
			HawkAssert(fScale != 0);
			Float fInv = 1.0f / fScale;
			Float fW   = (Float)Width * fInv;
			Float fH   = (Float)Height * fInv;
			Width      = (T)fW;
			Height     = (T)fH;
			return *this;
		}
	};

	//简易类型定义
	typedef	 HawkSize<Int32>  IntSize;
	typedef	 HawkSize<Float>  FloatSize;
}
#endif

