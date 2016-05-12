#include "Edge.h"

Edge::Edge(Vec2 startId, Vec2 endId, float w)
{
	startVertexId = startId;
	endVertexId = endId;
	weight = w;
}