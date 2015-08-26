#include "PopChoiceDlg.h"

Scene* PopChoiceDlg::createScene()
{
	auto scene = Scene::create();
	auto layer = PopChoiceDlg::create();
	scene->addChild(layer);
	return scene;
}

bool PopChoiceDlg::init()
{
	if (!LayerColor::initWithColor(ccc4(25, 25, 25, 125))) 
		return false;

	auto visibleSize = Director::getInstance()->getVisibleSize();
	//auto origin = Director::getInstance()->getVisibleOrigin();

	addBgImg();

	//获得背景sprite的大小
	m_size = m_bgSprite->getContentSize();
	auto yesItem = MenuItemImage::create(
		"popdlg/popy.png",
		"popdlg/popy.png",
		CC_CALLBACK_1(PopChoiceDlg::yesButton, this));

	auto noItem = MenuItemImage::create(
		"popdlg/popn.png",
		"popdlg/popn.png",
		CC_CALLBACK_1(PopChoiceDlg::noButton, this));

	// create menu, it's an autorelease object
	auto menu = Menu::create(yesItem, noItem, NULL);
	menu->alignItemsHorizontallyWithPadding(5);
	menu->setPosition(ccp(visibleSize.width / 2, visibleSize.height * 0.42));

	this->addChild(menu, 1);

	addTitle();
	addContent();

	//保证是模态对话框
	auto touchlistenter = EventListenerTouchOneByOne::create();
	touchlistenter->setSwallowTouches(true);
	touchlistenter->onTouchBegan = CC_CALLBACK_2(PopChoiceDlg::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistenter, this);

	return true;
}

void PopChoiceDlg::addBgImg()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	m_bgSprite = Sprite::create("popdlg/popbg.png");
	m_bgSprite->setPosition(ccp(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(m_bgSprite);
}

bool PopChoiceDlg::onTouchBegan(Touch* touch, Event* event)
{
	return true;
}

void PopChoiceDlg::yesButton(Ref* pSender)
{
	this->removeFromParentAndCleanup(true);
}

void PopChoiceDlg::noButton(Ref* pSender)
{
	this->removeFromParentAndCleanup(true);
}

void PopChoiceDlg::addTitle()
{
	m_title = LabelBMFont::create("Tips", "fonts/bitmapFontTest3.fnt");
	m_title->setPosition(ccp(m_size.width / 2, m_size.height - m_title->getContentSize().height * 1.5));
	m_bgSprite->addChild(m_title);
}

void PopChoiceDlg::addContent()
{
	m_content = LabelTTF::create("Hello! Welcome to our game!", "fonts/Marker Felt.ttf", 24);
	m_content->setColor(Color3B(0, 0, 0));
	m_content->setPosition(ccp(m_size.width / 2, m_size.height * 0.33));
	m_content->setDimensions(CCSize(this->m_size.width - 60, this->m_size.height - 100));
	m_content->setHorizontalAlignment(kCCTextAlignmentLeft);
	m_bgSprite->addChild(m_content);
}

void PopChoiceDlg::setTitle(const char* titleStr)
{
	m_title->setString(titleStr);
}

void PopChoiceDlg::setContent(const char* contentStr)
{
	m_content->setString(contentStr);
}

