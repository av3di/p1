#ifndef _SKINWEIGHT_H_
#define _SKINWEIGHT_H_

#include "Header.h"

class Skinweight
{
public:
	std::vector<float> weights; // 1st element is 1st joint that it is attached to, then weight, next joint, etc.
	Skinweight();
	~Skinweight();
};

#endif;