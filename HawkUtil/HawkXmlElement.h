#ifndef HAWK_XMLELEMENT_H
#define HAWK_XMLELEMENT_H

#include "HawkXmlAttribute.h"

namespace Hawk
{
	/************************************************************************/
	/* XML节点类型封装定义                                                  */
	/************************************************************************/
	template<class StringType> class HawkXmlElement : public HawkRefCounter
	{
	public:
		//节点默认构造函数
		HawkXmlElement(const StringType& sTag = StringType())
		{						
			Level  = 0;
			Parent = 0;			
			Tag    = sTag;
			Value.clear();
			Children.clear();
			Attributes.clear();			
		};

		//节点析构
		virtual ~HawkXmlElement()
		{
			Parent = 0;
			Children.clear();
			Attributes.clear();					
		};

		//节点赋值操作符
		HawkXmlElement<StringType>& operator = (const HawkXmlElement<StringType>& sEle)
		{
			if (this != &sEle)
			{
				Clear();

				Tag	  = sEle.Tag;
				Level = sEle.Level;				
				Value = sEle.Value;

				for (UInt32 i=0;i<sEle.Attributes.size();i++)
					Attributes.push_back(sEle.Attributes[i]);

				for (UInt32 i=0;i<sEle.Children.size();i++)
					Children.push_back(sEle.Children[i]);
			}
			return *this;
		}

	public:
		//属性类型定义
		typedef HawkXmlAttribute<StringType> AttributeType;

		//节点类型定义
		typedef HawkXmlElement<StringType>	 ElementType;	

	public:
		//根据名字获取属性指针
		AttributeType* GetAttribute(const StringType& sKey)
		{
			for (Size_t i=0;i<Attributes.size();i++)
			{
				if (Attributes[i].Key == sKey) 
				{
					return &Attributes[i];
				}
			}
			return 0;
		}

		//根据索引获取属性指针
		AttributeType* GetAttribute(Int32 iIdx)
		{
			if(iIdx>=0 && iIdx<Attributes.size())
				return &Attributes[iIdx];

			return 0;
		}

		//添加属性对象
		Bool AddAttribute(const AttributeType& sAttr)
		{
			AttributeType* pAttr = GetAttribute(sAttr.Key);
			if (!pAttr)
			{
				Attributes.push_back(sAttr);
				return true;
			}
			return false;
		}

		//插入属性对象
		Bool InsertAttribute(const AttributeType& sAttr,Int32 iIdx = 0)
		{
			if (!GetAttribute(sAttr.Key) && iIdx >= 0)
			{
				if (iIdx >= Attributes.size())
					return AddAttribute(sAttr);

				typename vector<AttributeType>::iterator it = Attributes.begin();
				for (Size_t i=0;i<Attributes.size() && it != Attributes.end();i++,it++)
				{
					if (i == iIdx)
					{
						Attributes.insert(it,sAttr);
						break;
					}
				}
				return true;
			}
			return false;
		}

		//根据名字删除属性
		Bool RemoveAttribute(const StringType& sKey)
		{
			typename vector<AttributeType>::iterator it = Attributes.begin();
			while (it != Attributes.end())
			{
				if((*it).Key == sKey)
				{
					Attributes.erase(it);
					return true;
				}
				it++;
			}
			return false;
		}

		//根据索引删除属性
		Bool RemoveAttribute(Int32 iIdx)
		{
			if (iIdx<0 || iIdx>=Attributes.size())
				return false;

			AttributeType* pAttr = GetAttribute(iIdx);
			if (pAttr)
			{
				return RemoveAttribute(pAttr->Key);
			}
			return false;
		}

		//获取子节点属性
		AttributeType* GetChildAttribute(const StringType& sChildTag,const StringType& sKey)
		{
			for (Size_t i=0;i<Children.size();i++)
			{
				if (Children[i].Tag == sChildTag) 
				{
					for (Size_t j=0;j<Children[i].Attributes.size();j++)
					{
						if (Children[i].Attributes[j].Key == sKey) 
						{
							return &Children[i].Attributes[j];
						}
					}
					break;
				}
			}
			return 0;
		}

		//获取属性格式
		Int32 GetAttributeNum() const
		{
			return (Int32)Attributes.size();
		}

		//删除所有属性
		void RemoveAllAttribute()
		{
			Attributes.clear();
		}

	public:
		//获取子节点个数
		Int32 GetChildrenNum(Bool bRecursion = false) const
		{
			Size_t iSize = Children.size();
			if (bRecursion)
			{
				for (Size_t i=0;i<iSize;i++)
				{
					iSize += Children[i].GetChildrenNum(bRecursion);
				}
			}
			return  (Int32)iSize;
		}

		//根据节点名获取子节点
		ElementType* GetChildren(const StringType& sTag)
		{
			for (Size_t i=0;i<Children.size();i++)
			{
				if (Children[i].Tag == sTag)
				{
					return &Children[i];
				}
			}
			return 0;
		}

		//根据节点索引获取子节点
		ElementType* GetChildren(Int32 iIdx)
		{
			if(iIdx>=0 && iIdx<GetChildrenNum())
			{
				return &Children[iIdx];
			}
			return 0;
		}

		//添加子节点
		ElementType* AddChildren(const StringType& sTag)
		{			
			Children.push_back(ElementType(sTag));
			ElementType* pEle = &Children[Children.size()-1];
			pEle->Level  = Level + 1;
			pEle->Parent = this;
			return pEle;
		}

		//插入子节点
		ElementType* InsertChildren(const StringType& sTag,Int32 iIdx = 0)
		{
			if (iIdx >= 0)
			{
				if (iIdx >= Children.size())
					return AddChildren(sTag);

				typename vector<ElementType>::iterator it = Children.begin();
				for (Size_t i=0;i<Children.size();i++,it++)
				{
					if (i != iIdx) 
						continue;

					ElementType* pEle = Children.insert(it,ElementType(sTag));
					pEle->SetLevel(Level+1);
					pEle->SetParent(this);
					return pEle;
				}
			}
			return 0;
		}

		//根据索引移除子节点
		Bool RemoveChildren(Int32 iIdx)
		{
			if (iIdx < 0 || iIdx >= Children.size())
				return false;

			typename vector<ElementType>::iterator it = Children.begin();
			for (Size_t i=0;i<Children.size();i++,it++)
			{
				if (i == iIdx)
				{
					Children.erase(it);
					return true;
				}
			}
			return false;
		}

		//移除子节点
		Bool RemoveChildren(ElementType* pChild)
		{
			if (!pChild) 
				return false;

			typename vector<ElementType>::iterator it = Children.begin();
			for (Size_t i=0;i<Children.size();i++,it++)
			{
				if (pChild == &Children[i])
				{
					Children.erase(it);
					return true;
				}
			}
			return false;
		}

		//移除所有子节点
		void RemoveAllChildren()
		{
			Children.clear();
		}

	public:
		//重置节点数据
		void Clear()
		{
			Level  = 0;
			Parent = 0;
			Tag.clear();
			Value.clear();
			Children.clear();
			Attributes.clear();
		}

		//设置标签
		void SetTag(const StringType& sTag)
		{
			Tag = sTag;
		}

		//获取标签
		StringType GetTag() const
		{
			return Tag;
		}

		//设置值
		void SetValue(const StringType& sVal)
		{
			Value = sVal;
		}

		//获取值
		StringType GetValue() const
		{
			return Value;
		}

		//获取层级
		Int32 GetLevel() const
		{
			return Level;
		}

		//节点转换为XML格式化字符串
		StringType ToString() const
		{
			StringType sTmp;

			for (Int32 i = 0;i<Level;i++)
				HawkStringUtil::Append<StringType,AString>(sTmp,"\t");

			HawkStringUtil::Append<StringType,AString>(sTmp,"<");
			HawkStringUtil::Append<StringType,StringType>(sTmp,Tag);

			for (Size_t i = 0;i<Attributes.size();i++)
			{
				HawkStringUtil::Append<StringType,AString>(sTmp," ");
				HawkStringUtil::Append<StringType,StringType>(sTmp,Attributes[i].Key);
				HawkStringUtil::Append<StringType,AString>(sTmp," = \"");
				HawkStringUtil::Append<StringType,StringType>(sTmp,Attributes[i].Value);
				HawkStringUtil::Append<StringType,AString>(sTmp,"\"");
			}

			if (Value.size() <=0 && Children.size() <=0) 
			{
				HawkStringUtil::Append<StringType,AString>(sTmp," />\r\n");
			}
			else
			{
				if (Children.size()>0)
				{
					HawkStringUtil::Append<StringType,AString>(sTmp,">\r\n");
				}

				if(Value.size()>0)
				{
					HawkStringUtil::Append<StringType,AString>(sTmp,">\r\n");
					for (Int32 i = 0;i<Level+1;i++)
						HawkStringUtil::Append<StringType,AString>(sTmp,"\t");

					HawkStringUtil::Append<StringType,StringType>(sTmp,Value);
					HawkStringUtil::Append<StringType,AString>(sTmp,"\r\n");
				}

				for (Size_t i = 0;i<Children.size();i++)
				{
					HawkStringUtil::Append<StringType,StringType>(sTmp,Children[i].ToString());
				}

				for (Int32 i = 0;i< Level;i++)
				{
					HawkStringUtil::Append<StringType,AString>(sTmp,"\t");
				}

				HawkStringUtil::Append<StringType,AString>(sTmp,"</");
				HawkStringUtil::Append<StringType,StringType>(sTmp,Tag);
				HawkStringUtil::Append<StringType,AString>(sTmp,">\r\n");
			}
			return sTmp;
		}		

	public:
		//标签
		StringType			  Tag;
		//节点值
		StringType			  Value;
		//节点层次
		Int32				  Level;
		//父节点
		ElementType*		  Parent;		
		//属性列表
		vector<AttributeType> Attributes;
		//子节点列表
		vector<ElementType>	  Children;
	};

	//XML节点不同编码的定义
	typedef HawkXmlElement<AString>		AXmlElement;
	typedef HawkXmlElement<UString>		UXmlElement;
	typedef HawkXmlElement<WString>		WXmlElement;
}
#endif
