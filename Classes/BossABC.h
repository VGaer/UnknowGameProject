#ifndef __BossABC_H__
#define __BossABC_H__

class BossABC : public Entity
{
public:
	virtual bool GetCanBeAttacked() = 0;
	virtual bool cmd_hurt(float damage) = 0;
private:
};


#endif