#include "Graph.h"
#include "cmath"
#include "cocos2d.h"

Graph* Graph::getInstance()
{
	static Graph* graph = NULL;
	if (graph == NULL)
		graph = new Graph();
	return graph;
}

void Graph::init(Vec2 center)
{
	TMXLayer* barrier = m_map->getLayer("barrier");
	std::vector<Vertex*> points;
	addVertex(center, positionForTiledCoord(center));
	points.push_back(vertices[center]);
	while (!points.empty())
	{
		auto vId = points.front()->getId();
		Vec2 newVec;
		// 左顶点
		if (vId.x > 0)
		{
			newVec.x = vId.x - 1, newVec.y = vId.y;
			if (barrier->getTileGIDAt(newVec) == 0)
			{				
				addVertex(newVec, positionForTiledCoord(newVec));
				addEdgesForVertex(vId, newVec);
				if (!vertices[newVec]->isFill)
				{
					points.push_back(vertices[newVec]);
					vertices[newVec]->isFill = true;
				}			
			}
		}
		// 右顶点
		if (vId.x < m_map->getMapSize().width - 1)
		{
			newVec.x = vId.x + 1, newVec.y = vId.y;
			if (barrier->getTileGIDAt(newVec) == 0)
			{
				addVertex(newVec, positionForTiledCoord(newVec));
				addEdgesForVertex(vId, newVec);
				if (!vertices[newVec]->isFill)
				{
					points.push_back(vertices[newVec]);
					vertices[newVec]->isFill = true;
				}
			}
		}
		// 上顶点
		if (vId.y > 0)
		{
			newVec.x = vId.x, newVec.y = vId.y - 1;
			if (barrier->getTileGIDAt(newVec) == 0)
			{
				addVertex(newVec, positionForTiledCoord(newVec));
				addEdgesForVertex(vId, newVec);
				if (!vertices[newVec]->isFill)
				{
					points.push_back(vertices[newVec]);
					vertices[newVec]->isFill = true;
				}
			}
		}
		// 下顶点
		if (vId.y < m_map->getMapSize().height - 1)
		{
			newVec.x = vId.x, newVec.y = vId.y + 1;
			if (barrier->getTileGIDAt(newVec) == 0)
			{
				addVertex(newVec, positionForTiledCoord(newVec));
				addEdgesForVertex(vId, newVec);
				if (!vertices[newVec]->isFill)
				{
					points.push_back(vertices[newVec]);
					vertices[newVec]->isFill = true;
				}
			}
		}
		points.erase(points.begin());
	}
}

void Graph::addEdgesForVertex(Vec2 startId, Vec2 endId)
{		
	Edge* edge = new Edge(startId, endId, 1);
	vertices[startId]->outEdges.push_back(edge);
}

void Graph::addVertex(Vec2 vertexId, Vec2 point)
{
	if (vertices.find(vertexId) == vertices.end())
	{
		auto vertex = new Vertex(vertexId, point);
		vertices[vertexId] = vertex;
	}
}

void Graph::removeVertex(Vec2 vertexId)
{
	if (vertices.find(vertexId) != vertices.end())
	{
		auto vertex = vertices[vertexId];
		for (int i = 0, size = vertex->outEdges.size(); i < size; i++)
		{
			auto endVertex = vertices[vertex->outEdges.at(i)->endVertexId];
			endVertex->removeOutEdgesByVertexId(vertexId);
		}
		vertices.erase(vertexId);
		delete vertex;
		vertex = NULL;
	}
}

bool Graph::findPath(Vec2 startId, Vec2 endId)
{
	if (vertices.find(endId) == vertices.end())
		return false;
	for (auto it = vertices.begin(); it != vertices.end(); it++)
	{
		auto& vertex = (*it).second;
		vertex->cost = 0xFFFFFFF;
		vertex->heuristic = 0xFFFFFFF;
		vertex->frontVertexId = NONE_ID;
	}
	auto startVertex = vertices[startId];
	startVertex->cost = 0;
	startVertex->heuristic = 0;
	targetId = endId;
	std::vector <Vertex*> Q;
	Q.push_back(startVertex);
	while (Q.size())
	{
		auto vertex = findMinHeurisVertex(Q);
		if (vertex->getId() == targetId)
			break;
		Vec2 vertexId = vertex->getId();
		for (int i = 0, size = vertex->outEdges.size(); i < size; i++)
		{
			auto edge = vertex->outEdges.at(i);
			Vec2 endVertexId = edge->endVertexId;
			if (relax(vertexId, endVertexId, edge->weight))
			{
				Q.push_back(vertices.at(endVertexId));
			}
		}
	}
	path.clear();
	Vec2 curId = endId;
	while (curId != NONE_ID && curId != startId)
	{
		path.push_back(curId);
		auto vertex = vertices.at(curId);
		if (vertex->frontVertexId == startId)
		{
			vertices.at(startId)->blackVertexId = curId;
			return true;
		}
		curId = vertex->frontVertexId;
	}
	return false;
}

Vertex* Graph::findMinHeurisVertex(std::vector <Vertex*>& Q)
{
	Vertex* minVertex = Q.front();
	int pos = 0;
	for (int i = 1, size = Q.size(); i < size; i++)
	{
		auto vertex = Q.at(i);
		if (vertex->heuristic < minVertex->heuristic)
		{
			minVertex = vertex;
			pos = i;
		}
	}
	Q.erase(Q.begin() + pos);
	return minVertex;
}

bool Graph::relax(Vec2 startId, Vec2 endId, float weight)
{
	auto v1 = vertices[startId];
	auto v2 = vertices[endId];
	float F = v1->cost + weight;
	float G = getDistance(endId, targetId);
	float H = F + G;
	if (H < v2->heuristic)
	{
		v2->cost = F;
		v2->heuristic = H;
		v2->frontVertexId = startId;
		return true;
	}
	return false;
}

float Graph::getDistance(Vec2 startId, Vec2 endId)
{
	auto v1 = vertices[startId];
	auto v2 = vertices[endId];
	float xDelta = v2->x - v1->x;
	float yDelta = v2->y - v1->y;
	float distance = sqrt(xDelta * xDelta + yDelta * yDelta);
	return distance;	
}

std::vector<Vec2> Graph::getPathResult()
{
	std::vector<Vec2> result;
	for (int i = 0, size = path.size(); i < size; i++)
	{
		result.push_back(path.at(size - 1 - i));
	}
	return result;
}

Vec2 Graph::positionForTiledCoord(Vec2 pos)
{
	Size mapTiledNum = m_map->getMapSize();
	Size tiledSize = m_map->getTileSize();
	Size visiblesize = Director::getInstance()->getVisibleSize();
	int x = pos.x * tiledSize.width + tiledSize.width / 2;
	int y = pos.y * tiledSize.height + tiledSize.height / 2;
	return Vec2(x, y);
}

void Graph::setTildMap(TMXTiledMap* map)
{
	this->m_map = map;
	Size mapSize = map->getMapSize();
	columnNum = mapSize.width;
	rowNum = mapSize.height;
}

TMXTiledMap* Graph::getMap()
{
	return m_map;
}