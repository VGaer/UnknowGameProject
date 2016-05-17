#include "Vertex.h"

Vertex::Vertex(Vec2 id, Vec2 point)
{
	vId = id;
	x = point.x;
	y = point.y;
	isFill = false;
}

Vec2 Vertex::getId()
{
	return vId;
}
void Vertex::removeOutEdgeByVertexId(Vec2 endVertexId)
{
	for (int i = 0, size = outEdges.size(); i < size; i++)
	{
		auto edge = outEdges.at(i);
		if (edge->getEdgeEndVertexId() == endVertexId)
		{
			outEdges.erase(outEdges.begin() + i);
			delete edge;
			edge = NULL;
			return;
		}
	}
}

bool Vertex::get_isFill()
{
	return isFill;
}

float Vertex::get_cost()
{
	return cost;
}

float Vertex::get_heuristic()
{
	return heuristic;
}

void Vertex::setVertex_outEdge(Edge* edge)
{
	this->outEdges.push_back(edge);
}

void Vertex::set_isFill(bool b)
{
	isFill = b;
}

void Vertex::removeVertexAllEdges()
{
	for (int i = this->outEdges.size(); i > 0; i--)
	{
		auto edge = this->outEdges.back();
		outEdges.pop_back();
		delete edge;
		edge = NULL;
	}
}

int Vertex::getVertexOutEdgeSize()
{
	return outEdges.size();
}

//如果i不符合要求，返回Vec2(-1,-1)
Vec2 Vertex::getVertex_ithOutEdge_EndvertexId(int i)
{
	if (outEdges.size() > 0 && (i >= 0 && i < outEdges.size()))
	{
		return outEdges.at(i)->getEdgeEndVertexId();
	}
	else
	{
		return Vec2(-1, -1);
	}
}

float Vertex::getVertex_ithOutEdge_Weight(int i)
{
	if (outEdges.size() > 0 && (i >= 0 && i < outEdges.size()))
	{
		return outEdges.at(i)->getEdgeWeight();
	}
	else
	{
		return 0.0f;
	}
}

float Vertex::getVertex_posx()
{
	return x;
}

float Vertex::getVertex_posy()
{
	return y;
}

void Vertex::setcost(float c)
{
	cost = c;
}

void Vertex::setheuristic(float h)
{
	heuristic = h;
}

void Vertex::setVertexFrontVertexId(Vec2 v)
{
	frontVertexId = v;
}

Vec2 Vertex::getVertexFrontVertexId()
{
	return frontVertexId;
}

bool Vertex::getIsinclose()
{
	return isInclose;
}

void Vertex::setVertexInclose(bool b)
{
	isInclose = b;
}

bool Vertex::getIsinopen()
{
	return isInopen;
}

void Vertex::setVertexopen(bool b)
{
	isInopen = b;
}