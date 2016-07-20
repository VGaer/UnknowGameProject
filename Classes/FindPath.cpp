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
	
		//��Ѱ·ԭ�ز���ʱ
		if (result.size() == 0){
			monster->IstrackNoresult = true;
			return;
		}
		else{
			monster->IstrackNoresult = false;
		}
		
		for (int i = 0, size = result.size(); i < size; i++)
		{
			auto vertex = graph->getGraphVertexByVertexId(result.at(i));
			auto pos = Vec2(vertex->getVertex_posx(), vertex->getVertex_posy());
			//��������
			pos.y = graph->getMap()->getMapSize().height * graph->getMap()->getTileSize().height - pos.y;
			Vec2 vec = pos - monster->getPosition();
			//log("%f,,,%f",pos.x,pos.y);

			//����Ҫ�����λ������monster�������������
			monster->getAnimBase()->setCurDirection(pos);

			auto delay = DelayTime::create(i * 0.5);
			// ����ת����
			auto call = CallFunc::create([=](){
				monster->getAnimBase()->playMoveAnim();
			});
			if (vec.x == 0 || vec.y == 0)//ֱ����·,���ݹ�����ٶ�
			{
				auto move = MoveTo::create(monster->monsdata.moveSpeed, pos);
				monster->runAction(Sequence::create(delay, call, move, NULL));
			}
			//�Խ�����,���ݹ�����ٶȣ�
			else
			{
				auto move = MoveTo::create(sqrt(2.0) * monster->monsdata.moveSpeed, pos);
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