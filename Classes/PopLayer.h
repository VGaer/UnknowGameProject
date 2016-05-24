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
	void setTitle(const string& title, Color3B color = Color3B::WHITE, int fontsize = 20, string font = "fonts/arial.ttf");
	// padding��paddingTop�����������ߵĿհ��� 
	void setContentText(const string& text, int fontsize = 20, int padding = 50, int paddintTop = 100, string font = "fonts/arial.ttf", Color3B color = Color3B::WHITE);
	void setCallbackFunc(Ref* target, SEL_CallFuncN callfun);
	bool addButton(const char* normalImage, const char* selectedImage, Vec2 pos, int tag = 0);
	void popBack();
	void flashLabel(Label* pSender);
	virtual void onEnter();
	virtual void onExit();
	bool isZero();
private:
	void buttonCallback(Ref* pSender);
	// �����������ߵĿհ���  
	int m_contentPadding;
	int m_contentPaddingTop;

	Ref* m_callbackListener;
	SEL_CallFuncN m_callback;

	//CC_SYNTHESIZE_RETAINΪ�궨�壺���˴�����set/get���⣬������retain�������ڴ���Դ��
	CC_SYNTHESIZE_RETAIN(Menu*, m__pMenu, MenuButton);
	CC_SYNTHESIZE_RETAIN(Sprite*, m__sfBackGround, SpriteBackGround);
	CC_SYNTHESIZE_RETAIN(Scale9Sprite*, m__s9BackGround, Sprite9BackGround);
	CC_SYNTHESIZE_RETAIN(Label*, m__ltTitle, LabelTitle);
	CC_SYNTHESIZE_RETAIN(Label*, m__ltContentText, LabelContentText);


};