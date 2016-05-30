#pragma once
#include "cocos2d.h"  
#include "ui\UIScale9Sprite.h" 
#include "ui\UIButton.h"
#include "string"
#include "DBMenu.h"
#include "PopLayer.h"
#include "ui\UIScrollView.h"
#include "QuestDispatcher.h"
USING_NS_CC;
using namespace ui;
using namespace std;

class QuestList : public PopLayer {
public:
	QuestList();
	~QuestList();
	virtual bool init();
	static QuestList* create(const char* backgroundImage);
	CREATE_FUNC(QuestList);
public:

	virtual void setTitle(const string& title, Color3B color = Color3B::YELLOW, int fontsize = 20, string font = "arial.ttf");
	// padding和paddingTop文字内容两边的空白区 
	virtual void setContentText(const string& text, int fontsize = 20, int padding = 50, int paddintTop = 100, string font = "arial.ttf", Color3B color = Color3B::YELLOW);
	virtual void setCallbackFunc(Ref* target, SEL_CallFuncN callfun);
	void setQuestTag(int sender);
	int getQuestTag() { return questTag; }
	bool addItem(const char *normalImage, const char *selectedImage, const string& title, const string& ins, int tag = 0, string font = "arial.ttf");
	virtual bool addButton(const char* normalImage, const char* selectedImage, Vec2 pos, int tag = 0);
	void setBtnPos(float durtime = 0);
	virtual void onEnter();
	virtual void onExit();

private:
	int questTag;
	Size contentSize;
	void menuCallback(Ref* pSender);
	void buttonCallback(Ref* pSender, Widget::TouchEventType type);

	// 文字内容两边的空白区  
	int m_contentPadding;
	int m_contentPaddingTop;

	Ref* m_callbackListener;
	SEL_CallFuncN m_callback;

	CC_SYNTHESIZE_RETAIN(DBMenu*, m__Item, MenuItems);
	CC_SYNTHESIZE_RETAIN(ScrollView*, m_ScrollView, ScView);
	CC_SYNTHESIZE_RETAIN(Sprite*, m__sfBackGround, SpriteBackGround);
	CC_SYNTHESIZE_RETAIN(Scale9Sprite*, m__s9BackGround, Sprite9BackGround);
	CC_SYNTHESIZE_RETAIN(Label*, m__ltTitle, LabelTitle);
	CC_SYNTHESIZE_RETAIN(Label*, m__ltContentText, LabelContentText);
};

