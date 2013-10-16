#include "HawkHash.h"

#define rot(x,k) (((x)<<(k)) ^ ((x)>>(32-(k))))

#define mix(a,b,c) \
{ \
	a -= c;  a ^= rot(c, 4);  c += b; \
	b -= a;  b ^= rot(a, 6);  a += c; \
	c -= b;  c ^= rot(b, 8);  b += a; \
	a -= c;  a ^= rot(c,16);  c += b; \
	b -= a;  b ^= rot(a,19);  a += c; \
	c -= b;  c ^= rot(b, 4);  b += a; \
}

#define final(a,b,c) \
{ \
	c ^= b; c -= rot(b,14); \
	a ^= c; a -= rot(c,11); \
	b ^= a; b -= rot(a,25); \
	c ^= b; c -= rot(b,16); \
	a ^= c; a -= rot(c,4);  \
	b ^= a; b -= rot(a,14); \
	c ^= b; c -= rot(b,24); \
}

namespace Hawk
{
	//初始化静态变量
	UInt32 HawkHash::m_iCpuEndian = HawkHash::CPU_UNKNOWN_ENDIAN;

	UInt32 HawkHash::CheckCpuEndian()
	{
		if (m_iCpuEndian == CPU_LITTLE_ENDIAN)
			return CPU_LITTLE_ENDIAN;

		if (m_iCpuEndian == CPU_BIG_ENDIAN)
			return CPU_BIG_ENDIAN;

		//重新计算
		union
		{
			UInt32 a;
			UInt8  b;           
		}c;
		c.a = 1;

		if (c.b == 1)
		{
			m_iCpuEndian = CPU_LITTLE_ENDIAN;
		}
		else 
		{
			m_iCpuEndian = CPU_BIG_ENDIAN;
		}
		return m_iCpuEndian;
	} 

	UInt32 HawkHash::CalcHash(const void* pData, Size_t iSize, UInt32 iInitval)
	{
		if (!m_iCpuEndian)
			CheckCpuEndian();

		if (m_iCpuEndian == CPU_LITTLE_ENDIAN)
		{
			return HashLittleEndian(pData, iSize, iInitval);
		}
		else if (m_iCpuEndian == CPU_BIG_ENDIAN)
		{
			return HashBigEndian(pData, iSize, iInitval);
		}

		HawkAssert(false && "CpuEndian Error.");
		return 0;
	}

	UInt32 HawkHash::HashLittleEndian(const void* key,Size_t length, UInt32 initval)
	{
		UInt32 a,b,c;
		union { const void* ptr; Size_t i; } u;

		a = b = c = 0xdeadbeef + ((UInt32)length) + initval;

		u.ptr = key;
		if ((u.i & 0x3) == 0) 
		{
			const UInt32* k = (const UInt32*)key;

			while (length > 12)
			{
				a += k[0];
				b += k[1];
				c += k[2];
				mix(a,b,c);
				length -= 12;
				k += 3;
			}

			switch(length)
			{
			case 12: c+=k[2]; b+=k[1]; a+=k[0]; break;
			case 11: c+=k[2]&0xffffff; b+=k[1]; a+=k[0]; break;
			case 10: c+=k[2]&0xffff; b+=k[1]; a+=k[0]; break;
			case 9 : c+=k[2]&0xff; b+=k[1]; a+=k[0]; break;
			case 8 : b+=k[1]; a+=k[0]; break;
			case 7 : b+=k[1]&0xffffff; a+=k[0]; break;
			case 6 : b+=k[1]&0xffff; a+=k[0]; break;
			case 5 : b+=k[1]&0xff; a+=k[0]; break;
			case 4 : a+=k[0]; break;
			case 3 : a+=k[0]&0xffffff; break;
			case 2 : a+=k[0]&0xffff; break;
			case 1 : a+=k[0]&0xff; break;
			case 0 : return c;
			}
		}
		else if ((u.i & 0x1) == 0) 
		{
			const UInt16* k  = (const UInt16*)key;
			const UInt8*  k8 = 0;

			while (length > 12)
			{
				a += k[0] + (((UInt32)k[1])<<16);
				b += k[2] + (((UInt32)k[3])<<16);
				c += k[4] + (((UInt32)k[5])<<16);
				mix(a,b,c);
				length -= 12;
				k += 6;
			}

			k8 = (const UInt8 *)k;
			switch(length)
			{
			case 12: c+=k[4]+(((UInt32)k[5])<<16);
				b+=k[2]+(((UInt32)k[3])<<16);
				a+=k[0]+(((UInt32)k[1])<<16);
				break;
			case 11: c+=((UInt32)k8[10])<<16;
			case 10: c+=k[4];
				b+=k[2]+(((UInt32)k[3])<<16);
				a+=k[0]+(((UInt32)k[1])<<16);
				break;
			case 9 : c+=k8[8]; 
			case 8 : b+=k[2]+(((UInt32)k[3])<<16);
				a+=k[0]+(((UInt32)k[1])<<16);
				break;
			case 7 : b+=((UInt32)k8[6])<<16;
			case 6 : b+=k[2];
				a+=k[0]+(((UInt32)k[1])<<16);
				break;
			case 5 : b+=k8[4];
			case 4 : a+=k[0]+(((UInt32)k[1])<<16);
				break;
			case 3 : a+=((UInt32)k8[2])<<16;
			case 2 : a+=k[0];
				break;
			case 1 : a+=k8[0];
				break;
			case 0 : return c;
			}
		} 
		else 
		{
			const UInt8* k = (const UInt8*)key;

			while (length > 12)
			{
				a += k[0];
				a += ((UInt32)k[1])<<8;
				a += ((UInt32)k[2])<<16;
				a += ((UInt32)k[3])<<24;
				b += k[4];
				b += ((UInt32)k[5])<<8;
				b += ((UInt32)k[6])<<16;
				b += ((UInt32)k[7])<<24;
				c += k[8];
				c += ((UInt32)k[9])<<8;
				c += ((UInt32)k[10])<<16;
				c += ((UInt32)k[11])<<24;
				mix(a,b,c);
				length -= 12;
				k += 12;
			}

			switch(length)
			{
			case 12: c+=((UInt32)k[11])<<24;
			case 11: c+=((UInt32)k[10])<<16;
			case 10: c+=((UInt32)k[9])<<8;
			case 9 : c+=k[8];
			case 8 : b+=((UInt32)k[7])<<24;
			case 7 : b+=((UInt32)k[6])<<16;
			case 6 : b+=((UInt32)k[5])<<8;
			case 5 : b+=k[4];
			case 4 : a+=((UInt32)k[3])<<24;
			case 3 : a+=((UInt32)k[2])<<16;
			case 2 : a+=((UInt32)k[1])<<8;
			case 1 : a+=k[0];
				break;
			case 0 : return c;
			}
		}
		final(a,b,c);
		return c;
	}

	UInt32 HawkHash::HashBigEndian(const void* key, Size_t length, UInt32 initval)
	{
		UInt32 a,b,c;
		union { const void *ptr; Size_t i; } u;

		a = b = c = 0xdeadbeef + ((UInt32)length) + initval;

		u.ptr = key;
		if ((u.i & 0x3) == 0)
		{
			const UInt32* k = (const UInt32*)key;

			while (length > 12)
			{
				a += k[0];
				b += k[1];
				c += k[2];
				mix(a,b,c);
				length -= 12;
				k += 3;
			}

			switch(length)
			{
			case 12: c+=k[2]; b+=k[1]; a+=k[0]; break;
			case 11: c+=k[2]&0xffffff00; b+=k[1]; a+=k[0]; break;
			case 10: c+=k[2]&0xffff0000; b+=k[1]; a+=k[0]; break;
			case 9 : c+=k[2]&0xff000000; b+=k[1]; a+=k[0]; break;
			case 8 : b+=k[1]; a+=k[0]; break;
			case 7 : b+=k[1]&0xffffff00; a+=k[0]; break;
			case 6 : b+=k[1]&0xffff0000; a+=k[0]; break;
			case 5 : b+=k[1]&0xff000000; a+=k[0]; break;
			case 4 : a+=k[0]; break;
			case 3 : a+=k[0]&0xffffff00; break;
			case 2 : a+=k[0]&0xffff0000; break;
			case 1 : a+=k[0]&0xff000000; break;
			case 0 : return c;
			}
		} 
		else 
		{
			const UInt8* k = (const UInt8*)key;

			while (length > 12)
			{
				a += ((UInt32)k[0])<<24;
				a += ((UInt32)k[1])<<16;
				a += ((UInt32)k[2])<<8;
				a += ((UInt32)k[3]);
				b += ((UInt32)k[4])<<24;
				b += ((UInt32)k[5])<<16;
				b += ((UInt32)k[6])<<8;
				b += ((UInt32)k[7]);
				c += ((UInt32)k[8])<<24;
				c += ((UInt32)k[9])<<16;
				c += ((UInt32)k[10])<<8;
				c += ((UInt32)k[11]);
				mix(a,b,c);
				length -= 12;
				k += 12;
			}

			switch(length)
			{
			case 12: c+=k[11];
			case 11: c+=((UInt32)k[10])<<8;
			case 10: c+=((UInt32)k[9])<<16;
			case 9 : c+=((UInt32)k[8])<<24;
			case 8 : b+=k[7];
			case 7 : b+=((UInt32)k[6])<<8;
			case 6 : b+=((UInt32)k[5])<<16;
			case 5 : b+=((UInt32)k[4])<<24;
			case 4 : a+=k[3];
			case 3 : a+=((UInt32)k[2])<<8;
			case 2 : a+=((UInt32)k[1])<<16;
			case 1 : a+=((UInt32)k[0])<<24;
				break;
			case 0 : return c;
			}
		}
		final(a,b,c);
		return c;
	}
}
