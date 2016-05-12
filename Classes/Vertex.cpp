#include "Vertex.h"

Vertex::Vertex(Vec2 id, Vec2 Point)
{
	vId = id;
	x = Point.x;
	y = Point.y;
	isFill = false;
}

Vec2 Vertex::getId()
{
	return vId;
}

void Vertex::removeOutEdgesByVertexId(Vec2 endVertexId)
{
	for (int i = 0, size = outEdges.size(); i < size; i++)
	{
		auto edge = outEdges.at(i);
		if (edge->endVertexId == endVertexId)
		{
			outEdges.erase(outEdges.begin() + i);
			delete edge;
			edge = NULL;
			return;
		}
	}
}