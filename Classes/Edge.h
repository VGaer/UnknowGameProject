#ifndef  __Edge_H__
#define  __Edge_H__

#include "cocos2d.h"
USING_NS_CC;

class Edge
{
public:
	Edge(Vec2 startVertexId, Vec2 endVertexId, float weight);
	float getEdgeWeight();
	Vec2 getEdgeStartVertexId();
	Vec2 getEdgeEndVertexId();
private:
	float weight;
	Vec2 startVertexId;
	Vec2 endVertexId;
};

#endif
