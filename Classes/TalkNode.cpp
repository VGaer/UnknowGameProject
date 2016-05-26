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
		//�ص�������  
		//������ASCII������-127~0  
		if (ch > -127 && ch< 0)
		{
			//����Ϊʲô+��3��  
			//��Ϊһ������ռ3���ֽ�  
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
//��ȡ���ݵ��ܳ���  
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

//�������賤�ȵ��ַ���  
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
		//��������˵����  
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

	//��ȡstrng  
	string str = content.substr(0, i);
	return str;
}