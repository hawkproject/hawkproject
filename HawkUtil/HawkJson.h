#ifndef HAWK_JSON_H
#define HAWK_JSON_H

#include "HawkDiskFile.h"

namespace Hawk
{
	/************************************************************************/
	/* JSON格式数据解析(UTF8编码)											*/
	/************************************************************************/
	class UTIL_API HawkJson : public HawkRefCounter
	{
	public:
		//构造
		HawkJson();

		//析构
		virtual ~HawkJson();

	public:
		//值类型定义
		enum
		{
			JSON_NONE = 0,
			JSON_BOOL,
			JSON_INT,
			JSON_FLOAT,
			JSON_STRING,
			JSON_OBJECT,
			JSON_ARRAY,				
		};

		//数组类型和对象类型定义
		struct  JsonVal;
		typedef vector<JsonVal*>		JsonArr;
		typedef map<UString, JsonVal*>	JsonObj;

		//值
		struct JsonVal
		{
			UInt8	 Type;
			Bool	 BoolVal;
			Int32	 IntVal;
			Float	 FloatVal;
			UString	 StrVal;
			JsonObj  ObjVal;
			JsonArr	 ArrVal;

			JsonVal() : Type(JSON_NONE), BoolVal(false), IntVal(0), FloatVal(0) {};
		};

	public:
		//解析数据
		virtual Bool   Parse(const AString& sFile);

		//解析数据
		virtual Bool   Parse(const UChar* pData, Int32 iSize);

		//保存到文件
		virtual Bool   SaveToFile(const AString& sFile);

		//清除数据
		virtual Bool   Clear();

		//获取根对象
		const JsonObj* GetRoot() const;

	protected:
		//释放JsonVal对象
		virtual Bool   FreeJsonVal(JsonVal* pVal);

	protected:
		//根对象
		JsonObj  m_sRoot;
	};
}
#endif
