#pragma once
#include "accessories.h"
class box :
	public accessories
{
public:
	box();
	~box();

	virtual void eventP(player &);

};

