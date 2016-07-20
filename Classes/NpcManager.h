#ifndef _NpcManager_H_
#define _NpcManager_H_

#include "NPC.h"
class NPC;

class NpcManager
{
public:
	static NpcManager* getInstance();
public:
	// »ñÈ¡NPCÈÝÆ÷
	Vector<NPC*>& getNpcsVec();
private:
	Vector<NPC*> m_npcVector;
};

#endif