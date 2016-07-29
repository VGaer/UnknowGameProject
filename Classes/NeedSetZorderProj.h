#ifndef __NeedSetZorderProj_H__
#define __NeedSetZorderProj_H__

#include "Entity.h"

class NeedSetZorderProj :public Entity
{
public:
	static NeedSetZorderProj* getInstance();
	Vector<Node*>& getNeedSetZorderProjVec();
private:
	Vector<Node*> m_needsetzorderprojvec;
};

#endif