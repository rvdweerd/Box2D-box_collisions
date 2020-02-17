#pragma once
#include <functional>
#include <vector>
#include "Box.h"

//#ifndef PCFUNC
//#define PCFUNC std::function<std::vector<std::unique_ptr<Box>>(Box*, std::vector<std::unique_ptr<Box>>)>
//#endif

namespace PostCollisionFunctions
{
	std::function<std::vector<std::unique_ptr<Box>>(Box*, std::vector<std::unique_ptr<Box>>)> Destruct = [](Box* b, std::vector<std::unique_ptr<Box>> bPrs)
	{
		//auto it0 = std::find_if(bPtrs.begin(), bPtrs.end(), [b](auto bPtr) {return (bPtr.get() == b); });
		for (auto it = bPrs.begin(); it != bPrs.end(); ++it)
		{
			if (it->get() == b)
			{
				bPrs.erase(it);
				break;
			}
		}
		return std::move(bPrs);
	};
	std::function<std::vector<std::unique_ptr<Box>>(Box*, std::vector<std::unique_ptr<Box>>)> SetToWhite = [](Box* b, std::vector<std::unique_ptr<Box>> bP)
	{
		b->SetColorToWhite(); 
		b->SetAction(Box::PostCollisionBehavior::NoAction);
		return std::move(bP);
	};

}