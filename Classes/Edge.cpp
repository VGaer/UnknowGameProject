#include "Edge.h"

Edge::Edge(Vec2 startVertexId, Vec2 endVertexId, float weight)
{
	this->startVertexId = startVertexId;
	this->endVertexId = endVertexId;
	this->weight = weight;
}

float Edge::getEdgeWeight()
{
	return weight;
}

Vec2 Edge::getEdgeStartVertexId()
{
	return startVertexId;
}

Vec2 Edge::getEdgeEndVertexId()
{
	return endVertexId;
}