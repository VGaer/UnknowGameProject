#ifndef __Entity_H__
#define __Entity_H__

#include "cocos2d.h"
USING_NS_CC;

class Entity : public Node
{
public:
	void bindSprite(Sprite* sprite);
	Sprite* getSprite();
private:
	Sprite* m_sprite;
};
#endif