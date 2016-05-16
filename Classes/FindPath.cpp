#include "FindPath.h"
#include "Graph.h"

void FindPath::run(Vec2 startId, Vec2 endId)
{
	auto graph = Graph::getInstance();
	std::vector<Vec2> result;
	if (graph->findPath(startId, endId))
	{
		monster->stopAllActions();
		result = graph->getPathResult();
		for (int i = 0, size = result.size(); i < size; i++)
		{
			auto vertex = graph->getGraphVertexByVertexId(result.at(i));
			auto pos = Vec2(vertex->getVertex_posx(), vertex->getVertex_posy());
			pos.y = graph->getMap()->getMapSize().height * graph->getMap()->getTileSize().height - pos.y;
			Vec2 vec = pos - monster->getPosition();
			auto delay = DelayTime::create(i * 0.5);
			// ����ת����
			auto call = CallFunc::create([=](){
				MoveDir moveD = enum_static;
				if (vec.x != 0)
				{
					if (vec.x > 0) moveD = enum_right;
					else moveD = enum_left;
				}
				else if (vec.y != 0)
				{
					if (vec.y > 0) moveD = enum_up;
					else moveD = enum_down;
				}
				//monster->playRunAnimaWithDir(moveD);
			});
			if (vec.x == 0 || vec.y == 0)//ֱ����,0.6f��Ϊ�˴��ƽ���¶Խ����ߵ��ٶȣ�0.1f����ͣ��Ӧ��ûʲô����
			{
				auto move = MoveTo::create(0.6f, pos);
				monster->runAction(Sequence::create(delay, call, move, NULL));
			}
			//�Խ����� 
			else
			{
				auto move = MoveTo::create(0.7f, pos);
				monster->runAction(Sequence::create(delay, call, move, NULL));
			}
			
			break;//ֻȡ��һ��
		}
	}	
}

void FindPath::bindMonster(Monster* mon)
{
	this->monster = mon;
}