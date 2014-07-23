#ifndef HAWK_XMLELEMENT_H
#define HAWK_XMLELEMENT_H

#include "HawkXmlAttribute.h"

namespace Hawk
{
	/************************************************************************/
	/* XML�ڵ����ͷ�װ����                                                  */
	/************************************************************************/
	template<class StringType> class HawkXmlElement : public HawkRefCounter
	{
	public:
		//�ڵ�Ĭ�Ϲ��캯��
		HawkXmlElement(const StringType& sTag = StringType())
		{						
			Level  = 0;
			Parent = 0;			
			Tag    = sTag;
			Value.clear();
			Children.clear();
			Attributes.clear();			
		};

		//�ڵ�����
		virtual ~HawkXmlElement()
		{
			Parent = 0;
			Children.clear();
			Attributes.clear();					
		};

		//�ڵ㸳ֵ������
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
		//�������Ͷ���
		typedef HawkXmlAttribute<StringType> AttributeType;

		//�ڵ����Ͷ���
		typedef HawkXmlElement<StringType>	 ElementType;	

	public:
		//�������ֻ�ȡ����ָ��
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

		//����������ȡ����ָ��
		AttributeType* GetAttribute(Int32 iIdx)
		{
			if(iIdx>=0 && iIdx<Attributes.size())
				return &Attributes[iIdx];

			return 0;
		}

		//������Զ���
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

		//�������Զ���
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

		//��������ɾ������
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

		//��������ɾ������
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

		//��ȡ�ӽڵ�����
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

		//��ȡ���Ը�ʽ
		Int32 GetAttributeNum() const
		{
			return (Int32)Attributes.size();
		}

		//ɾ����������
		void RemoveAllAttribute()
		{
			Attributes.clear();
		}

	public:
		//��ȡ�ӽڵ����
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

		//���ݽڵ�����ȡ�ӽڵ�
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

		//���ݽڵ�������ȡ�ӽڵ�
		ElementType* GetChildren(Int32 iIdx)
		{
			if(iIdx>=0 && iIdx<GetChildrenNum())
			{
				return &Children[iIdx];
			}
			return 0;
		}

		//����ӽڵ�
		ElementType* AddChildren(const StringType& sTag)
		{			
			Children.push_back(ElementType(sTag));
			ElementType* pEle = &Children[Children.size()-1];
			pEle->Level  = Level + 1;
			pEle->Parent = this;
			return pEle;
		}

		//�����ӽڵ�
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

		//���������Ƴ��ӽڵ�
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

		//�Ƴ��ӽڵ�
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

		//�Ƴ������ӽڵ�
		void RemoveAllChildren()
		{
			Children.clear();
		}

	public:
		//���ýڵ�����
		void Clear()
		{
			Level  = 0;
			Parent = 0;
			Tag.clear();
			Value.clear();
			Children.clear();
			Attributes.clear();
		}

		//���ñ�ǩ
		void SetTag(const StringType& sTag)
		{
			Tag = sTag;
		}

		//��ȡ��ǩ
		StringType GetTag() const
		{
			return Tag;
		}

		//����ֵ
		void SetValue(const StringType& sVal)
		{
			Value = sVal;
		}

		//��ȡֵ
		StringType GetValue() const
		{
			return Value;
		}

		//��ȡ�㼶
		Int32 GetLevel() const
		{
			return Level;
		}

		//�ڵ�ת��ΪXML��ʽ���ַ���
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
		//��ǩ
		StringType			  Tag;
		//�ڵ�ֵ
		StringType			  Value;
		//�ڵ���
		Int32				  Level;
		//���ڵ�
		ElementType*		  Parent;		
		//�����б�
		vector<AttributeType> Attributes;
		//�ӽڵ��б�
		vector<ElementType>	  Children;
	};

	//XML�ڵ㲻ͬ����Ķ���
	typedef HawkXmlElement<AString>		AXmlElement;
	typedef HawkXmlElement<UString>		UXmlElement;
	typedef HawkXmlElement<WString>		WXmlElement;
}
#endif
