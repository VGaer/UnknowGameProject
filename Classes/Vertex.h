#ifndef _Vertex_H_
#define _Vertex_H_

#include "Edge.h"
#include "vector"
#include "cocos2d.h"
USING_NS_CC;

class Vertex
{
public:
	Vertex(Vec2 id, Vec2 point);
	float x;
	float y;	
	float cost;
	float heuristic;
	bool isFill;
	Vec2 frontVertexId;
	Vec2 blackVertexId;
	std::vector <Edge*> outEdges;	
	Vec2 getId();
	void removeOutEdgesByVertexId(Vec2 endVertexId);
private:	
	Vec2 vId;
};

#endif