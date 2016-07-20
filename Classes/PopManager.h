#pragma once

#include "Pop.h"
class Pop;

class PopManager {
public:
	static PopManager* getInstance();
public:
	// »ñÈ¡µ¯³ö²ãÈÝÆ÷
	map<string,Pop*>& getPopsMap();
private:
	map<string,Pop*> m_popMap;
};