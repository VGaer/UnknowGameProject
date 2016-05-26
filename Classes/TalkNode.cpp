#include "TalkNode.h"

void TalkNode::init(Label* node)
{
	content = node->getString();
	contentLeght = 0;

	int length = content.length();
	int i = 0;
	while (i < length)
	{
		char ch = getContent()[i];
		//重点在这里  
		//中文在ASCII码中是-127~0  
		if (ch > -127 && ch< 0)
		{
			//这里为什么+＝3呢  
			//因为一个中文占3个字节  
			i += 3;
		}
		else
		{
			i++;
		}
		contentLeght++;
	}
	wordCount = length;
}
//获取内容的总长度  
int TalkNode::getContentLength()
{
	return contentLeght;
}

void TalkNode::logic(float dt)
{
	if (wordCount > tnode->getContentLength())
	{
		return;
	}

	wordCount++;
	Label* label = Label::create();
	label->setString(tnode->getContentByLength(wordCount));
}

//返回所需长度的字符串  
string TalkNode::getContentByLength(int length)
{
	if (length >= contentLeght)
	{
		return getContent();
	}
	int i = 0;
	int index = 0;
	while (index < length)
	{
		char ch = getContent()[i];
		//这里上面说过了  
		if (ch > -127 && ch < 0)
		{
			i += 3;
		}
		else
		{
			i++;
		}
		index++;
	}

	//截取strng  
	string str = content.substr(0, i);
	return str;
}