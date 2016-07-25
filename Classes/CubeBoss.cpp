#include "CubeBoss.h"
#include "Graph.h"
#include "CCShake.h"
#include "MonsterRemoteskill.h"


void CubeBossFindPath::run(Vec2 startId, Vec2 endId)
{
	auto graph = Graph::getInstance();
	std::vector<Vec2> result;
	if (graph->findPath(startId, endId))
	{
		result = graph->getPathResult();

		if (result.size() == 0)
		{
			//寻路无结果
		}

		for (int i = 0, size = result.size(); i < size; i++)
		{
			auto vertex = graph->getGraphVertexByVertexId(result.at(i));
			auto pos = Vec2(vertex->getVertex_posx(), vertex->getVertex_posy());
			//换算坐标
			pos.y = graph->getMap()->getMapSize().height * graph->getMap()->getTileSize().height - pos.y;


			
			if (m_cubeboss)
			{
				JumpTo* jumpto = JumpTo::create(m_cubeboss->jumptime, pos, m_cubeboss->jumpheight, 1);
				CallFunc* callfun = CallFunc::create([&](){
					


					m_cubeboss->setJumping(false);

					if (m_cubeboss->getParent())
					{
						CCShake* shake = CCShake::create(0.1f, 20);
						//瓦片地图runaction没有震动效果,所以获取瓦片地图的父亲node
						if (m_cubeboss->getParent()->getParent())
							m_cubeboss->getParent()->getParent()->runAction(shake);
					}
				});
				Sequence* sequence = Sequence::create(jumpto,callfun,NULL);
				m_cubeboss->runAction(sequence);
				m_cubeboss->m_timecounter->start();
				m_cubeboss->caljumpNums++;
				m_cubeboss->setJumping(true);
			}

			break;
		}
		
	}


}

void CubeBossFindPath::bindBoss(CubeBoss* boss)
{
	m_cubeboss = boss;
}


bool CubeBoss::init(const std::string name)
{
	bindSprite(Sprite::create("cubeeyeboss.png"));
	this->setAnchorPoint(Vec2(0.5, 1 / 3.0f));

	m_cubebossfindpath = new CubeBossFindPath();
	m_cubebossfindpath->bindBoss(this);

	m_timecounter = TimeCounter::create();
	this->addChild(m_timecounter);
	
	jumptime = 0.7f;
	jumpheight = 250.0f;

	this->scheduleUpdate();

	this->schedule(schedule_selector(CubeBoss::JumpUpdate), 0.75f);

	m_player = Player::getInstance();

	return true;
}

CubeBoss* CubeBoss::create(const std::string name)
{
	auto boss = new CubeBoss();
	if (boss && boss->init(name))
	{
		boss->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(boss);
		boss = NULL;
	}
	return boss;
}

void CubeBoss::update(float dt)
{
	IsColliWithPlayer();

	//ControlPlayerMove();
}

void CubeBoss::JumpUpdate(float dt)
{
	if (m_player != NULL)
	{
		if (caljumpNums < 4 && isLasering == false)
		{
			Vec2 start = m_player->tiledCoordForPosition(this->getPosition());
			Vec2 end = m_player->tiledCoordForPosition(m_player->getPosition());//玩家位置
			m_cubebossfindpath->run(start, end);
			//设置OneOnLand = false;
			OneOnLand = false;

			//跳跃了，去掉跳跃时所占用的瓦片的障碍
			//此BOSS所在地图barrier只存放障碍物瓦片块。
			TMXLayer* barrier = m_parrent->getLayer("barrier");
			int tileGid = barrier->getTileGIDAt(start);
			if (tileGid > 0)
			{
				barrier->removeTileAt(start);
			}
			else
			{
				;
			}

		}
		//当caljumpNums == 4的时候，即已经跳跃了4次,这次释放激光
		else if (caljumpNums >= 4)
		{
			isLasering = true;
			//重置
			caljumpNums = 0;

			//如果是踩在barrier层的normal,正常四方向角度发射激光，否则斜45度发射激光
			if (m_parrent)
			{
				auto twocolortiled = m_parrent->getLayer("twocolortiled");
				Vec2 tiledcoord = m_player->tiledCoordForPosition(this->getPosition());
				int tileGid = twocolortiled->getTileGIDAt(tiledcoord);
				if (tileGid > 0)
				{
					Value prop = m_parrent->getPropertiesForGID(tileGid);
					ValueMap proValueMap = prop.asValueMap();
					if (proValueMap.find("lasernormal") != proValueMap.end())
					{
						std::string laser = proValueMap.at("lasernormal").asString();
						if (laser == "true")
						{
							MonsterRemoteskillDur_NoMove* skillu = MonsterRemoteskillDur_NoMove::createWithName_width_height_damage_duration_diretion_attackNums_attackNumsInter("eyecubebosslaser1.png", "eyecubebosslaser", 900, 64, 50, 2.0f, "up", 5, 0.5f);
							MonsterRemoteskillDur_NoMove* skilld = MonsterRemoteskillDur_NoMove::createWithName_width_height_damage_duration_diretion_attackNums_attackNumsInter("eyecubebosslaser1.png", "eyecubebosslaser", 900, 64, 50, 2.0f, "down", 5, 0.5f);
							MonsterRemoteskillDur_NoMove* skilll = MonsterRemoteskillDur_NoMove::createWithName_width_height_damage_duration_diretion_attackNums_attackNumsInter("eyecubebosslaser1.png", "eyecubebosslaser", 900, 64, 50, 2.0f, "left", 5, 0.5f);
							MonsterRemoteskillDur_NoMove* skillr = MonsterRemoteskillDur_NoMove::createWithName_width_height_damage_duration_diretion_attackNums_attackNumsInter("eyecubebosslaser1.png", "eyecubebosslaser", 900, 64, 50, 2.0f, "right", 5, 0.5f);
				
							m_parrent->addChild(skillu);
							skillu->setPosition(this->getPosition() + Vec2(0, this->getContentSize().height / 2 - 150));
							m_parrent->addChild(skilld);
							skilld->setPosition(this->getPosition() + Vec2(0, -this->getContentSize().height / 2 + 195));
							m_parrent->addChild(skilll);
							skilll->setPosition(this->getPosition() + Vec2(-this->getContentSize().width / 2 + 165, 0));
							m_parrent->addChild(skillr);
							skillr->setPosition(this->getPosition() + Vec2(this->getContentSize().width / 2 - 165, 0));
						
							m_timecounter->start();
						}
					}

					if (proValueMap.find("laserhard") != proValueMap.end())
					{
						std::string laser = proValueMap.at("laserhard").asString();
						if (laser == "true")
						{
							MonsterRemoteskillDur_NoMove* skillu = MonsterRemoteskillDur_NoMove::createWithName_width_height_damage_duration_diretion_attackNums_attackNumsInter("eyecubebosslaser1.png", "eyecubebosslaser", 900, 64, 50, 2.0f, "upright", 5, 0.5f);
							MonsterRemoteskillDur_NoMove* skilld = MonsterRemoteskillDur_NoMove::createWithName_width_height_damage_duration_diretion_attackNums_attackNumsInter("eyecubebosslaser1.png", "eyecubebosslaser", 900, 64, 50, 2.0f, "downleft", 5, 0.5f);
							MonsterRemoteskillDur_NoMove* skilll = MonsterRemoteskillDur_NoMove::createWithName_width_height_damage_duration_diretion_attackNums_attackNumsInter("eyecubebosslaser1.png", "eyecubebosslaser", 900, 64, 50, 2.0f, "leftup", 5, 0.5f);
							MonsterRemoteskillDur_NoMove* skillr = MonsterRemoteskillDur_NoMove::createWithName_width_height_damage_duration_diretion_attackNums_attackNumsInter("eyecubebosslaser1.png", "eyecubebosslaser", 900, 64, 50, 2.0f, "rightdown", 5, 0.5f);

							m_parrent->addChild(skillu);
							skillu->setPosition(this->getPosition() + Vec2(0, this->getContentSize().height / 2 - 150));
							m_parrent->addChild(skilld);
							skilld->setPosition(this->getPosition() + Vec2(0, -this->getContentSize().height / 2 + 195));
							m_parrent->addChild(skilll);
							skilll->setPosition(this->getPosition() + Vec2(-this->getContentSize().width / 2 + 165, 0));
							m_parrent->addChild(skillr);
							skillr->setPosition(this->getPosition() + Vec2(this->getContentSize().width / 2 - 165, 0));

							m_timecounter->start();
						}
					}
				}
			}

			
		}


		if (isLasering == true)
		{
			//大于激光技能持续时间, 留0.5f让玩家攻击boss
			if (m_timecounter->getCurTime() > 2.5f)
				isLasering = false;
		}
	}
}

void CubeBoss::setParent(TMXTiledMap* parent)
{
	m_parrent = parent;

	m_parrent->getLayer("twocolortiled")->setVisible(false);

}

CubeBoss::CubeBoss()
{
	m_player = NULL;
	m_parrent = NULL;
	isJumping = false;
	isLasering = false;
	caljumpNums = 0;
	OneOnLand = false;
}

TMXTiledMap* CubeBoss::getParent()
{
	return m_parrent;
}

void CubeBoss::setJumping(bool s)
{
	isJumping = s;
}

bool CubeBoss::getJumping()
{
	return isJumping;
}

bool CubeBoss::IsColliWithPlayer()
{
	//箱子在地上
	if (isJumping == false)
	{
		//OneOnLand重新设置成false在boss执行run寻路跳跃时设置
		if (OneOnLand == true)
			return false;

		if (OneOnLand == false)
			OneOnLand = true;
		if (m_player)
		{
			if (m_parrent)
			{
				//此BOSS所在地图barrier只存放障碍物瓦片块。
				TMXLayer* barrier = m_parrent->getLayer("barrier");
				int tileGid = barrier->getTileGIDAt(m_player->tiledCoordForPosition(this->getPosition()));
				if (tileGid > 0)
				{
					;
				}
				//如果此时无障碍物，添加障碍物
				else
				{
					int tileGid;
					for (int i = 0; i < m_parrent->getMapSize().width; i++)
					{
						bool s = false;
						for (int j = 0; j < m_parrent->getMapSize().height; j++)
						{
							tileGid = barrier->getTileGIDAt(Vec2(i, j));
							if (tileGid > 0)
							{
								s = true;
								break;
							}
						}
						if (s == true)
							break;
					}
					//设置障碍物
					barrier->setTileGID(tileGid, m_player->tiledCoordForPosition(this->getPosition()));
				}
			}


			if (m_player->tiledCoordForPosition(this->getPosition()) == m_player->tiledCoordForPosition(m_player->getPosition()))
			{
				if (m_player->m_hp > 0)
				{
					//一砸就是生是死
					m_player->m_hp -= m_player->m_hp;
					return true;
				}		
			}
		}
	}
	else
	{
		if (m_player)
		{
			float curtime = m_timecounter->getCurTime();
			//跳跃的速度大约取个平均值
			float speed = jumpheight * 2 / jumptime;
			//下降运动,判断是否压倒具有高度的主角
			if (curtime > jumptime / 2)
			{
				float playerheight = m_player->getContentSize().height;

				curtime -= jumptime / 2;

				float bosscurheight = jumpheight - speed * curtime;

				//如果在同一个格子,且boss当前高度小于主角高度
				if (m_player->tiledCoordForPosition(this->getPosition()) == m_player->tiledCoordForPosition(m_player->getPosition()))
				{
					if (bosscurheight <= playerheight)
					{
						if (m_player->m_hp > 0)
						{
							//还在空中的碰到主角的头没完全砸到地面，扣25
							m_player->m_hp -= 25;
							return true;
						}
					}
				}

			}
			//上升运动，上升不用写判断
			else
			{
				;
			}
		}
	}
}

//CubeBoss死后移除记得得让m_player->IscanMove = true;
void CubeBoss::ControlPlayerMove()
{
	if (m_player)
	{	
		Rect rect = this->getBoundingBox();
		//此怪物时当场正方体的,伪立体，用2/3Contensize的高跟主角的
		//Boundingbox判断
		rect.setRect(rect.getMinX(), rect.getMinY(), this->getContentSize().width, this->getContentSize().height * 2.0f / 3.0f);
		int dir = m_player->getPlayerDir();
		//由于主角锚点定位0.5 0,水平和下直接boundingbox判断就好，上的话要特殊调整boundingbox
		if (dir != enum_up)
		{
			if (m_player->getBoundingBox().intersectsRect(rect))
			{
				//正方体非跳跃状态,主角跟正方体碰撞了，主角不能移动
				if (this->isJumping == false)
					m_player->IscanMove = false;
				else
					m_player->IscanMove = true;
			}
			else
			{
				m_player->IscanMove = true;
			}
		}
		else
		{
			Rect playerboundingbox = m_player->getBoundingBox();
			//伪立体的，把boundingbox的height设得非常小即可,
			playerboundingbox.setRect(playerboundingbox.getMinX(),playerboundingbox.getMinY(),m_player->getContentSize().width,m_player->getContentSize().height * 1.0f / 20.0f);

			if (playerboundingbox.intersectsRect(rect))
			{
				//正方体非跳跃状态,主角跟正方体碰撞了，主角不能移动
				if (this->isJumping == false)
					m_player->IscanMove = false;
				else
					m_player->IscanMove = true;
			}
			else
			{
				m_player->IscanMove = true;
			}
		}
		
	}
}
