#pragma once
#include "accessories.h"
class ladder :
	public accessories
{
public:
	ladder();
	~ladder();
	virtual void eventP(player &);
};

