#ifndef __Vertex_H__
#define __Vertex_H__

#include "cocos2d.h"
#include "Edge.h"
#include "vector"
USING_NS_CC;

class Vertex
{
public:
	Vertex(Vec2 id, Vec2 point);
	Vec2 getId();
	void removeOutEdgeByVertexId(Vec2 endVertexId);
	bool get_isFill();
	float get_cost();
	float get_heuristic();
	void setVertex_outEdge(Edge* edge);
	void set_isFill(bool b);
	void removeVertexAllEdges();
	int getVertexOutEdgeSize();
	Vec2 getVertex_ithOutEdge_EndvertexId(int i);
	float getVertex_ithOutEdge_Weight(int i);
	float getVertex_posx();
	float getVertex_posy();
	void setcost(float c);
	void setheuristic(float h);
	void setVertexFrontVertexId(Vec2 v);
	Vec2 getVertexFrontVertexId();
	bool getIsinclose();
	void setVertexInclose(bool b);
	bool getIsinopen();
	void setVertexopen(bool b);
private:
	float x;
	float y;
	float cost;
	float heuristic;
	bool isFill;
	Vec2 frontVertexId;
	Vec2 backVertexId;
	std::vector<Edge*> outEdges;
	Vec2 vId;
	bool isInclose;
	bool isInopen;
};

#endif 