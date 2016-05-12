#ifndef _Edge_H_
#define _Edge_H_

#include "cocos2d.h"
USING_NS_CC;

class Edge
{
public:
	Edge(Vec2 startVertexId, Vec2 endVertexId, float weight);
	float weight;
	Vec2 startVertexId;
	Vec2 endVertexId;
};

#endif