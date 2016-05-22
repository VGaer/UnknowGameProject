#include "PopLayer.h"

PopLayer::PopLayer() :
	m__pMenu(NULL)
	, m_contentPadding(0)
	, m_contentPaddingTop(0)
	, m_callbackListener(NULL)
	, m_callback(NULL)
	, m__sfBackGround(NULL)
	, m__s9BackGround(NULL)
	, m__ltContentText(NULL)
	, m__ltTitle(NULL)
{}

PopLayer::~PopLayer() {
	//安全释放，与CC_SYNTHESIZE_RETAIN中的CC_SAFE_RETAIN对应
	CC_SAFE_RELEASE(m__pMenu);
	CC_SAFE_RELEASE(m__sfBackGround);
	CC_SAFE_RELEASE(m__ltContentText);
	CC_SAFE_RELEASE(m__ltTitle);
	CC_SAFE_RELEASE(m__s9BackGround);
}

bool PopLayer::init() {
	if (!Layer::init()) {
		return false;
	}
	this->setContentSize(Size::ZERO);

	// 初始化需要的 Menu  
	Menu* menu = Menu::create();
	menu->setPosition(Vec2::ZERO);
	setMenuButton(menu);

	setTouchEnabled(true);
	return true;
}

bool PopLayer::onTouchBegan(Touch * _touch, Event * _event) {
	log("PopLayer touch");
	return true;
}

PopLayer* PopLayer::create(const char *backgroundImage) {
	PopLayer* ml = PopLayer::create();
	ml->setSpriteBackGround(Sprite::create(backgroundImage));
	ml->setSprite9BackGround(Scale9Sprite::create(backgroundImage));
	return ml;
}

void PopLayer::setTitle(const string& title, Color3B color, int fontsize, string font) {
	Label* ltfTitle = Label::create(title, font, fontsize);
	ltfTitle->setAnchorPoint(Vec2(0.5, 1));
	ltfTitle->setColor(color);
	setLabelTitle(ltfTitle);
}

void PopLayer::setContentText(const string& text, int fontsize, int padding, int paddingTop, string font, Color3B color) {
	Label* ltf = Label::create(text, font, fontsize);
	ltf->setColor(color);
	ltf->setAnchorPoint(Vec2(0.5, 1));
	setLabelContentText(ltf);
	m_contentPadding = padding;
	m_contentPaddingTop = paddingTop;
}

void PopLayer::setCallbackFunc(Ref *target, SEL_CallFuncN callfun) {
	m_callbackListener = target;
	m_callback = callfun;
}

bool PopLayer::addButton(const char *normalImage, const char *selectedImage, Vec2 pos, int tag) {
	// 创建图片菜单按钮  
	MenuItemImage* menuImage = MenuItemImage::create(normalImage, selectedImage, this, menu_selector(PopLayer::buttonCallback));
	menuImage->setTag(tag);
	menuImage->setPosition(pos);

	getMenuButton()->addChild(menuImage);
	return true;
}

void PopLayer::buttonCallback(Ref* pSender) {
	Node* node = dynamic_cast<Node*>(pSender);
	log("touch tag: %d", node->getTag());
	if (m_callback && m_callbackListener) {
		//执行调用层的回调函数，传递node参数
		(m_callbackListener->*m_callback)(node);
	}
}

void PopLayer::onEnter() {
	Layer::onEnter();

	Size winSize = Director::sharedDirector()->getWinSize();
	Vec2 pCenter = Vec2(winSize.width / 2, winSize.height / 2);
	getSpriteBackGround()->setAnchorPoint(Vec2(0.5, 0));
	getSprite9BackGround()->setAnchorPoint(Vec2(0.5, 0));
	Size contentSize;
	// 设定好参数，在运行时加载  
	if (isZero()) {
		getSpriteBackGround()->setPosition(Vec2(0, 100));
		this->addChild(getSpriteBackGround(), 0, 0);
		contentSize = getSpriteBackGround()->getTexture()->getContentSize();
		getSpriteBackGround()->addChild(getMenuButton());
	}
	else {
		Scale9Sprite *background = getSprite9BackGround();
		background->setContentSize(getContentSize());
		background->setPosition(Vec2(0, 100));
		this->addChild(background, 0, 0);
		contentSize = getContentSize();
		getSprite9BackGround()->addChild(getMenuButton());
	}


	// 添加按钮，并设置其位置  
	
	float btnWidth = contentSize.width / (getMenuButton()->getChildrenCount() - 1);

	Vector<Node*> vecArray = getMenuButton()->getChildren();
	//Ref* pObj = NULL;
	int i = 1;
	for (auto& e : vecArray) {
		Node* node = dynamic_cast<Node*>(e);
		node->setAnchorPoint(Vec2(0, 1));
		node->setPosition(node->getPosition());
		i++;
	}

	// 显示对话框标题  
	if (getLabelTitle()) {
		getLabelTitle()->setPosition(Vec2(contentSize.width / 2 + 10, contentSize.height - 35));
		if(isZero())
			getSpriteBackGround()->addChild(getLabelTitle());
		else
			getSprite9BackGround()->addChild(getLabelTitle());
	}

	// 显示文本内容  
	if (getLabelContentText()) {
		Label* ltf = getLabelContentText();
		ltf->setPosition(Vec2(contentSize.width / 2 + 10, contentSize.height - 35 - getLabelTitle()->getContentSize().height - 10));
		ltf->setDimensions(contentSize.width - m_contentPadding * 2, contentSize.height - m_contentPaddingTop);
		ltf->setHorizontalAlignment(kCCTextAlignmentCenter);
		if(isZero())
			getSpriteBackGround()->addChild(ltf);
		else
			getSprite9BackGround()->addChild(ltf);
	}

	// 弹出效果  
	Action* popLayer = Sequence::create(ScaleTo::create(0.0, 0.0),
		ScaleTo::create(0.06, 1.05),
		ScaleTo::create(0.08, 0.95),
		ScaleTo::create(0.08, 1.0), NULL);
	this->runAction(popLayer);
}

void PopLayer::popBack()
{
	Action* popLayer = Sequence::create(ScaleTo::create(0.0, 1.0),
		ScaleTo::create(0.06, 0.95),
		ScaleTo::create(0.08, 1.05),
		ScaleTo::create(0.08, 0.0), NULL);
	this->runAction(popLayer);
}

void PopLayer::onExit() {
	log("popup on exit.");
	Layer::onExit();
}

bool PopLayer::isZero()
{
	if (getContentSize().equals(Size::ZERO))
		return true;
	else
		return false;
}
