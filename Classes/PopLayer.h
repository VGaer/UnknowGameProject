#pragma once
#include "cocos2d.h"  
#include "ui\UIScale9Sprite.h" 
#include "string"
USING_NS_CC;
using namespace ui;
using namespace std;

class PopLayer :public Layer
{
public:
	PopLayer();
	~PopLayer();
	virtual bool init();
	CREATE_FUNC(PopLayer);
	virtual bool onTouchBegan(Touch* _touch, Event* _event);
	static PopLayer* create(const char* backgroundImage);
	void setTitle(const string& title, Color3B color = Color3B::WHITE, int fontsize = 20, string font = "arial.ttf");
	// padding和paddingTop文字内容两边的空白区 
	void setContentText(const string& text, int fontsize = 20, int padding = 40, int paddintTop = 800, string font = "arial.ttf", Color3B color = Color3B::RED);
	void setCallbackFunc(Ref* target, SEL_CallFuncN callfun);
	bool addButton(const char* normalImage, const char* selectedImage, Vec2 pos, int tag = 0);
	void popBack();
	void Talking();
	void Talking(string& sender);
	void showFont(float dt);
	virtual void onEnter();
	virtual void onExit();
	bool isZero();
private:
	string temp;

	void buttonCallback(Ref* pSender);
	// 文字内容两边的空白区  
	int m_contentPadding;
	int m_contentPaddingTop;
	// 0~3来获得一个中文字符  
	int n;

	Ref* m_callbackListener;
	SEL_CallFuncN m_callback;

	//CC_SYNTHESIZE_RETAIN为宏定义：除了创建了set/get以外，还用了retain来保留内存资源。
	CC_SYNTHESIZE_RETAIN(Menu*, m__pMenu, MenuButton);
	CC_SYNTHESIZE_RETAIN(Sprite*, m__sfBackGround, SpriteBackGround);
	CC_SYNTHESIZE_RETAIN(Scale9Sprite*, m__s9BackGround, Sprite9BackGround);
	CC_SYNTHESIZE_RETAIN(Label*, m__ltTitle, LabelTitle);
	CC_SYNTHESIZE_RETAIN(Label*, m__ltContentText, LabelContentText);


};