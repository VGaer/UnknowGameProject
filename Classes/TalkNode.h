#pragma once
#include <iostream>
#include "cocos2d.h"
USING_NS_CC;
using namespace std;
class TalkNode
{
public:
	TalkNode() {
		tnode = new TalkNode();
	}
	string content;
	void init(Label* node);
	const string& getContent() { return content; };
	string getContentByLength(int length);
	int contentLeght;
	int getContentLength();
	void logic(float dt);
	int wordCount;
	TalkNode* tnode;
};