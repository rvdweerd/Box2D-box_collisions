#pragma once
//#include <functional>
//#include <vector>
//#include "Box.h"

#ifndef PCFUNC
#define PCFUNC std::function<std::vector<std::unique_ptr<Box>>(Box*, std::vector<std::unique_ptr<Box>>)>
#endif

namespace PostCollisionFunctions
{
	static PCFUNC Destruct = [](Box* b, std::vector<std::unique_ptr<Box>> bPrs)
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
	static PCFUNC SetToWhite = [](Box* b, std::vector<std::unique_ptr<Box>> bP)
	{
		b->SetColorToWhite();
		b->SetAction(Box::PostCollisionBehavior::NoAction);
		return std::move(bP);
	};
	//static PCFUNC SplitIntoFour = [](Box* b, std::vector<std::unique_ptr<Box>> bP, const Game& g)
	//{
	//	struct BoxSplitData
	//	{
	//		Vec2 pos;
	//		float size;
	//	};
	//	std::vector<BoxSplitData> newBoxes;

	//	float currSize = b->GetSize();
	//	Vec2 currPos = b->GetPosition();
	//	if (currSize >= Game::boxSize / 18)
	//	{
	//		newBoxes.push_back({ {currPos.x + (currSize / 2),currPos.y + (currSize / 2)},currSize / 3.5f });
	//		newBoxes.push_back({ {currPos.x + (currSize / 2),currPos.y - (currSize / 2)},currSize / 3.5f });
	//		newBoxes.push_back({ {currPos.x - (currSize / 2),currPos.y + (currSize / 2)},currSize / 3.5f });
	//		newBoxes.push_back({ {currPos.x - (currSize / 2),currPos.y - (currSize / 2)},currSize / 3.5f });
	//	}

	//	//for (auto e : newBoxes)
	//	//{
	//	//	boxPtrs.push_back(Box::SpawnPos(e.pos, e.size, bounds, world, rng));
	//	//}
	//	//auto p = boxPtrs[0]->GetColorTrait().GetColor();
	//	//std::generate_n(std::back_inserter(boxPtrs), 4*count, [this]() {
	//	//	return Box::Spawn(boxSize / 4, bounds, world, rng);
	//	//	});
	//	return std::move(bP);

	//};
}
