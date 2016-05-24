#include "QuestList.h"
#include "PopManager.h"
QuestList::QuestList() :
	m__s9BackGround(NULL)
	, m__sfBackGround(NULL)
	, m_ScrollView(NULL)
	, m__ltContentText(NULL)
	, m__ltTitle(NULL)
	, m_contentPadding(0)
	, m_contentPaddingTop(0)
	, m_callbackListener(NULL)
	, m_callback(NULL)
	, m__Item(NULL)
{}

QuestList::~QuestList() {
	CC_SAFE_RELEASE(m__s9BackGround);
	CC_SAFE_RELEASE(m__sfBackGround);
	CC_SAFE_RELEASE(m_ScrollView);
	CC_SAFE_RELEASE(m__ltContentText);
	CC_SAFE_RELEASE(m__ltTitle);
	CC_SAFE_RELEASE(m__Item);
}

bool QuestList::init() {
	if (!Layer::init()) {
		return false;
	}
	this->setContentSize(Size::ZERO);

	// 初始化需要的 Menu  
	DBMenu* menu = DBMenu::create();
	menu->setPosition(Vec2::ZERO);
	setMenuItems(menu);

	// 初始化需要的 view 
	ScrollView* view = ScrollView::create();
	view->setPosition(Vec2::ZERO);
	view->setBounceEnabled(true);
	view->setDirection(ScrollView::Direction::VERTICAL);
	setScView(view);

	setTouchEnabled(true);
}

QuestList * QuestList::create(const char * backgroundImage)
{
	QuestList* vl = QuestList::create();
	vl->setSpriteBackGround(Sprite::create(backgroundImage));
	vl->setSprite9BackGround(Scale9Sprite::create(backgroundImage));
	return vl;
}

void QuestList::setTitle(const string& title, Color3B color, int fontsize, string font) {
	Label* ltfTitle = Label::create(title, font, fontsize);
	ltfTitle->setColor(color);
	setLabelTitle(ltfTitle);
}

void QuestList::setContentText(const string& text, int fontsize, int padding, int paddingTop, string font, Color3B color) {
	Label* ltf = Label::create(text, font, fontsize);
	ltf->setColor(color);
	setLabelContentText(ltf);
	m_contentPadding = padding;
	m_contentPaddingTop = paddingTop;
}

void QuestList::setCallbackFunc(Ref *target, SEL_CallFuncN callfun) {
	m_callbackListener = target;
	m_callback = callfun;
}

bool QuestList::addItem(const char *normalImage, const char *selectedImage, const string& title, const string& ins, int tag, string font) {
	// 创建图片菜单按钮  
	MenuItemImage* itemImage = MenuItemImage::create(normalImage, selectedImage, this, menu_selector(QuestList::menuCallback));
	itemImage->setTag(tag);
	// 添加文字说明并设置位置  
	Size imenu = itemImage->getContentSize();
	setTitle(title, Color3B::YELLOW, 20);
	auto ttf = getLabelTitle();
	ttf->setAnchorPoint(Vec2(0.5, 1));
	ttf->setPosition(Vec2(imenu.width / 2, imenu.height));
	itemImage->addChild(ttf);

	setContentText(ins, 15);
	auto ltf = getLabelContentText();
	ltf->setAnchorPoint(Vec2(0, 0.5));
	ltf->setPosition(Vec2(0, imenu.height / 2));
	ltf->setHorizontalAlignment(TextHAlignment::CENTER);
	itemImage->addChild(ltf);
	getMenuItems()->addChild(itemImage);
	return true;
}

bool QuestList::addButton(const char * normalImage, const char * selectedImage, Vec2 pos, int tag)
{
	// 创建图片菜单按钮  
	Button* menuImage = Button::create(normalImage, selectedImage);
	menuImage->addTouchEventListener(CC_CALLBACK_2(QuestList::buttonCallback,this));
	menuImage->setTag(tag);
	menuImage->setAnchorPoint(Vec2(1, 0.5));
	menuImage->setPosition(pos);
	if (isZero())
		getSpriteBackGround()->addChild(menuImage);
	else
		getSprite9BackGround()->addChild(menuImage);
	return true;
}


void QuestList::menuCallback(Ref* pSender) {
	Node* node = dynamic_cast<Node*>(pSender);
	log("touch tag: %d", node->getTag());
	if (m_callback && m_callbackListener) {
		//执行调用层的回调函数，传递node参数
		(m_callbackListener->*m_callback)(node);
	}
}

void QuestList::buttonCallback(Ref * pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED) {
		Sequence* a = Sequence::create(CallFunc::create([&]() {
			static_cast<PopLayer*>(PopManager::getInstance()->getLayerByTag(1)->layer)->popBack();
		}), DelayTime::create(0.3), CallFunc::create([&]() {
			PopManager::getInstance()->setPopped(1, false); }), NULL);
		runAction(a);
	}
}

void QuestList::onEnter() {
	Layer::onEnter();

	getSpriteBackGround()->setAnchorPoint(Vec2(1, 0));
	getSprite9BackGround()->setAnchorPoint(Vec2(1, 0));
	Size contentSize;
	// 设定好参数，在运行时加载  
	if (isZero()) {
		contentSize = getSpriteBackGround()->getTexture()->getContentSize();
		getSpriteBackGround()->setPosition(Vec2(22, 0));
		this->addChild(getSpriteBackGround(), 0, 0);		
		getScView()->setContentSize(contentSize);
		getScView()->setInnerContainerSize(Size(contentSize.width, contentSize.height*2));
		getScView()->addChild(getMenuItems());
		getSpriteBackGround()->addChild(getScView());
	}
	else {
		Scale9Sprite *background = getSprite9BackGround();
		background->setContentSize(getContentSize());
		background->setPosition(Vec2(22, 0));
		this->addChild(background, 0, 0);
		contentSize = getContentSize();
		getScView()->setContentSize(contentSize);
		getScView()->setInnerContainerSize(Size(contentSize.width, contentSize.height * 2));
		getScView()->addChild(getMenuItems());
		getSprite9BackGround()->addChild(getScView());
	}


	// 添加按钮，并设置其位置  

	//float btnHeight = contentSize.height / (getMenuItems()->getChildrenCount() - 1);

	Vector<Node*> vecArray = getMenuItems()->getChildren();
	//Ref* pObj = NULL;
	int i = 0;
	for (auto& e : vecArray) {
		Node* node = dynamic_cast<Node*>(e);
		node->setAnchorPoint(Vec2(0.5, 1));
		node->setPosition(contentSize.width/2, getScView()->getInnerContainerSize().height - i);
		i = i + node->getContentSize().height;
	}

	// 弹出效果  
	Action* popLayer = Sequence::create(ScaleTo::create(0.0, 0.0),
		ScaleTo::create(0.06, 1.05),
		ScaleTo::create(0.08, 0.95),
		ScaleTo::create(0.08, 1.0), NULL);
	this->runAction(popLayer);
}

void QuestList::onExit() {
	log("Quest on exit.");
	Layer::onExit();
}
