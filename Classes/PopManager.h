#pragma once

#include "Pop.h"
class Pop;

class PopManager {
public:
	static PopManager* getInstance();
public:
	// ��ȡ����������
	map<string,Pop*>& getPopsMap();
private:
	map<string,Pop*> m_popMap;
};