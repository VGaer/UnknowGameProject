#include "AppDelegate.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"
#include "StartGameScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate()
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
	//set OpenGL context attributions,now can only set six attributions:
	//red,green,blue,alpha,depth,stencil
	GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };

	GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
	// initialize director
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview) {
		glview = GLViewImpl::createWithRect("HelloCpp", Rect(0, 0, 960, 640));
		director->setOpenGLView(glview);
	}

	director->getOpenGLView()->setDesignResolutionSize(960, 640, ResolutionPolicy::SHOW_ALL);

	// turn on display FPS
	director->setDisplayStats(false);

	// set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0 / 60);

	FileUtils::getInstance()->addSearchPath("res");

	// create a scene. it's an autorelease object
	//auto scene = HelloWorld::createScene();
	//auto scene = GameScene::createSceneWithId(2);
	auto scene = StartGameScene::createScene();

	// run
	director->runWithScene(scene);

	//初始化背景音乐
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sound/startgamebgm.mp3");
	//初始化音效
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/Blip.wav");


	//初始化音效
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/Jswordsound.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/fireboom.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/lasersound.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/Kskill.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/hitedbybone.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/playerhited.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/monhitedbyPlayerBaseskill.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/getthings.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/levelsup.wav");
	
	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();

	// if you use SimpleAudioEngine, it must be pause
	 CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();

	// if you use SimpleAudioEngine, it must resume here
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
