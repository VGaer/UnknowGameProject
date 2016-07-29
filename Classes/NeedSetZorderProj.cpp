#include "NeedSetZorderProj.h"


NeedSetZorderProj* NeedSetZorderProj::getInstance()
{
	static NeedSetZorderProj* instance = NULL;
	if (instance == NULL)
	{
		instance = new NeedSetZorderProj();
	}
	return instance;
}

Vector<Node*>& NeedSetZorderProj::getNeedSetZorderProjVec()
{
	return m_needsetzorderprojvec;
}

