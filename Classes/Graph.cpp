#include "Graph.h"

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
	this->addVertex(center, positionForTiledCoord(center));
	points.push_back(vertices[center]);
	//先设置vectices[center]->isFill(true),放入points的都要设置为true，不然会被重复放进去一次
	vertices[center]->set_isFill(true);
	while (!points.empty())
	{
		auto vId = points.front()->getId();
		Vec2 newVec;
		//左顶点
		if (vId.x > 0)
		{
			newVec.x = vId.x - 1; newVec.y = vId.y;
			//无碰撞
			if (barrier->getTileGIDAt(newVec) == 0)
			{
				addVertex(newVec, positionForTiledCoord(newVec));
				addEdgesForVertex(vId, newVec,10);
				if (!vertices[newVec]->get_isFill())
				{
					points.push_back(vertices[newVec]);
					vertices[newVec]->set_isFill(true);
				}
			}
		}
		//右顶点
		if (vId.x < m_map->getMapSize().width - 1)
		{
			newVec.x = vId.x + 1; newVec.y = vId.y;
			if (barrier->getTileGIDAt(newVec) == 0)
			{
				addVertex(newVec,positionForTiledCoord(newVec));
				addEdgesForVertex(vId, newVec,10);
				if (!vertices[newVec]->get_isFill())
				{
					points.push_back(vertices[newVec]);
					vertices[newVec]->set_isFill(true);
				}
			}
		}
		//上顶点
		if (vId.y > 0)
		{
			newVec.x = vId.x, newVec.y = vId.y - 1;
			if (barrier->getTileGIDAt(newVec) == 0)
			{
				addVertex(newVec, positionForTiledCoord(newVec));
				addEdgesForVertex(vId, newVec,10);
				if (!vertices[newVec]->get_isFill())
				{
					points.push_back(vertices[newVec]);
					vertices[newVec]->set_isFill(true);
				}
			}
		}
		//下顶点
		if (vId.y < m_map->getMapSize().height - 1)
		{
			newVec.x = vId.x; newVec.y = vId.y + 1;
			if (barrier->getTileGIDAt(newVec) == 0)
			{
				addVertex(newVec, positionForTiledCoord(newVec));
				addEdgesForVertex(vId, newVec,10);
				if (!vertices[newVec]->get_isFill())
				{
					points.push_back(vertices[newVec]);
					vertices[newVec]->set_isFill(true);
				}
			}
		}
		//左上
		if (vId.x > 0 && vId.y > 0)
		{
			newVec.x = vId.x - 1; newVec.y = vId.y - 1;
			if (barrier->getTileGIDAt(newVec) == 0)
			{
				addVertex(newVec, positionForTiledCoord(newVec));
				addEdgesForVertex(vId, newVec, sqrt(200));
				if (!vertices[newVec]->get_isFill())
				{
					points.push_back(vertices[newVec]);
					vertices[newVec]->set_isFill(true);
				}
			}
		}
		//右下
		if (vId.x < m_map->getMapSize().width - 1 && vId.y < m_map->getMapSize().height - 1)
		{
			newVec.x = vId.x + 1; newVec.y = vId.y + 1;
			if (barrier->getTileGIDAt(newVec) == 0)
			{
				addVertex(newVec, positionForTiledCoord(newVec));
				addEdgesForVertex(vId, newVec, sqrt(200));
				if (!vertices[newVec]->get_isFill())
				{
					points.push_back(vertices[newVec]);
					vertices[newVec]->set_isFill(true);
				}
			}
		}
		//左下
		if (vId.x > 0 && vId.y < m_map->getMapSize().height - 1)
		{
			newVec.x = vId.x - 1; newVec.y = vId.y + 1;
			if (barrier->getTileGIDAt(newVec) == 0)
			{
				addVertex(newVec, positionForTiledCoord(newVec));
				addEdgesForVertex(vId, newVec, sqrt(200));
				if (!vertices[newVec]->get_isFill())
				{
					points.push_back(vertices[newVec]);
					vertices[newVec]->set_isFill(true);
				}
			}
		}
		//右上
		if (vId.x < m_map->getMapSize().width - 1 && vId.y > 0)
		{
			newVec.x = vId.x + 1; newVec.y = vId.y - 1;
			if (barrier->getTileGIDAt(newVec) == 0)
			{
				addVertex(newVec, positionForTiledCoord(newVec));
				addEdgesForVertex(vId, newVec, sqrt(200));
				if (!vertices[newVec]->get_isFill())
				{
					points.push_back(vertices[newVec]);
					vertices[newVec]->set_isFill(true);
				}
			}
		}
		points.erase(points.begin());
	}
}
void Graph::addEdgesForVertex(Vec2 startId, Vec2 endId, float weight)
{
	Edge* edge = new Edge(startId, endId, weight);
	vertices[startId]->setVertex_outEdge(edge);
}
void Graph::addVertex(Vec2 vertexId, Vec2 point)
{
	if (vertices.find(vertexId) == vertices.end())
	{
		auto vertex = new Vertex(vertexId,point);
		vertices[vertexId] = vertex;
	}
}
void Graph::removeVertex(Vec2 vertexId)
{
	if (vertices.find(vertexId) != vertices.end())
	{
		auto vertex = vertices[vertexId];
		//删掉此点关联到的所有点的关联回此点的边。
		for (int i = 0, size = vertex->getVertexOutEdgeSize(); i < size; i++)
		{
			auto endvertexId = vertex->getVertex_ithOutEdge_EndvertexId(i);
			if (endvertexId != Vec2(-1, -1))
			{
				auto endVertex = vertices[endvertexId];
				endVertex->removeOutEdgeByVertexId(vertexId);
			}
		}
		vertices.erase(vertexId);
		//删除此点前，先释放此点的边
		vertex->removeVertexAllEdges();
		//删除此点
		delete vertex;
		vertex = NULL;
	}
}

bool Graph::findPath(Vec2 startId, Vec2 endId)
{
	//加上判断startId,即怪物位置那个顶点是否为空，不会因为初始化怪物在障碍物的方块而造成奔溃
	if (vertices.find(startId) == vertices.end())
		return false;
	if (vertices.find(endId) == vertices.end())
		return false;
	for (auto it = vertices.begin(); it != vertices.end(); it++)
	{
		auto& vertex = (*it).second;
		vertex->setVertexInclose(false);
		vertex->setVertexopen(false);
	}
	auto startVertex = vertices[startId];
	startVertex->setcost(0.0f);
	startVertex->setheuristic(0.0f);
	targetId = endId;
	std::vector<Vertex*> openQ;
	openQ.push_back(startVertex);
	//标记为close
	startVertex->setVertexInclose(true);
	while (openQ.size())
	{
		auto vertex = findMinHeurisVertex(openQ);
		//标记为close
		vertex->setVertexInclose(true);

		if (vertex->getId() == targetId)
			break;
		Vec2 vertexId = vertex->getId();
		for (int i = 0, size = vertex->getVertexOutEdgeSize(); i < size; i++)
		{
			Vec2 endVertexId = vertex->getVertex_ithOutEdge_EndvertexId(i);
			
			if (vertices[endVertexId]->getIsinclose()){
				;
			}
			else if (vertices[endVertexId]->getIsinopen()){
				relax(vertexId, endVertexId, vertex->getVertex_ithOutEdge_Weight(i));
			}
			else{
				float G = vertex->get_cost() + vertex->getVertex_ithOutEdge_Weight(i);
				float H = getDistance(endVertexId, targetId);
				float F = G + H;
				vertices[endVertexId]->setcost(G);
				vertices[endVertexId]->setheuristic(F);
				vertices[endVertexId]->setVertexFrontVertexId(vertex->getId());
				//放进open表
				vertices[endVertexId]->setVertexopen(true);
				openQ.push_back(vertices[endVertexId]);
			}
		}
	}

	path.clear();
	Vec2 curId = endId;
	while (curId != NONE_ID && curId != startId)
	{
		path.push_back(curId);
		auto vertex = vertices.at(curId);
		if (vertex->getVertexFrontVertexId() == startId)
		{
			//vertices.at(startId)->
			return true;
		}
		curId = vertex->getVertexFrontVertexId();
	}
	return false;
}

bool Graph::relax(Vec2 startId, Vec2 endId, float weight)
{
	auto v1 = vertices[startId];
	auto v2 = vertices[endId];
	float G = v1->get_cost() + weight;
	float H = getDistance(endId,targetId);
	float F = G + H;
	if (H < v2->get_heuristic())
	{
		v2->setcost(G);
		v2->setheuristic(F);
		v2->setVertexFrontVertexId(startId);
		return true;
	}
	return false;
}

float Graph::getDistance(Vec2 startId, Vec2 endId)
{
	auto v1 = vertices[startId];
	auto v2 = vertices[endId];
	float xDelta = v2->getVertex_posx() - v1->getVertex_posx();
	float yDelta = v2->getVertex_posy() - v1->getVertex_posy();
	float distance = sqrt(xDelta * xDelta + yDelta * yDelta);
	return distance;
}

Vertex* Graph::findMinHeurisVertex(std::vector < Vertex*>& Q)
{
	Vertex* minVertex = Q.front();
	int pos = 0;
	for (int i = 1,size = Q.size(); i < size; i++)
	{
		auto vertex = Q.at(i);
		if (vertex->get_heuristic() < minVertex->get_heuristic())
		{
			minVertex = vertex;
			pos = i;
		}
	}
	Q.erase(Q.begin() + pos);
	return minVertex;
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

Vec2 Graph::positionForTiledCoord(Vec2 pos)
{
	Size tiledSize = m_map->getTileSize();
	int x = pos.x * tiledSize.width + tiledSize.width / 2;
	int y = pos.y * tiledSize.height + tiledSize.height / 2; 
	return Vec2(x, y);
}

Vec2 Graph::tiledCoordForPosition(Vec2 pos)
{
	Size mapTiledNum = m_map->getMapSize();
	Size tiledSize = m_map->getTileSize();
	Size visiblesize = Director::getInstance()->getVisibleSize();

	int x = (int)(pos.x / tiledSize.width);
	int y = (int)(m_map->getMapSize().height * m_map->getTileSize().height - pos.y) / m_map->getTileSize().height;

	return Vec2(x, y);
}

Vertex* Graph::getGraphVertexByVertexId(Vec2 vertexId)
{
	if (vertices.find(vertexId) != vertices.end())
	{
		return vertices[vertexId];
	}
	else
	{
		return NULL;
	}
}