#pragma once

class Game;
std::unique_ptr<PCFUNC> MakePCFUNCptr_SetToWhite(Game& g)
{
	return std::move(std::make_unique<PCFUNC>([&](Box* b)
	{
		//Box::ColorTrait* temp = b->GetColorTrait.release();
		//delete temp;
		//b->GetColorTrait = std::make_unique<WhiteTrait>();
		
		b->SetColorToWhite();
	}));
}
std::unique_ptr<PCFUNC> MakePCFUNCptr_Destruct(Game& g)
{
	return std::move(std::make_unique<PCFUNC>([&](Box* b)
	{

		for (auto it = g.boxPtrs.begin(); it != g.boxPtrs.end(); ++it)
		{
			if (it->get() == b)
			{
				g.boxPtrs.erase(it);
				break;
			}
		}
	}));
}
std::unique_ptr<PCFUNC> MakePCFUNCptr_SplitIntoFour(Game& g)
{
	return std::move(std::make_unique<PCFUNC>([&](Box* b)
	{
		float currSize = b->GetSize();
		Vec2 currPos = b->GetPosition();
		if (currSize >= g.boxSize / 2)
		{
			g.boxPtrs.push_back(Box::SpawnPos({ currPos.x + (currSize / 2),currPos.y + (currSize / 2) }, currSize / 3.5f, g.bounds, g.world, g.rng));
			g.boxPtrs.push_back(Box::SpawnPos({ currPos.x + (currSize / 2),currPos.y - (currSize / 2) }, currSize / 3.5f, g.bounds, g.world, g.rng));
			g.boxPtrs.push_back(Box::SpawnPos({ currPos.x - (currSize / 2),currPos.y + (currSize / 2) }, currSize / 3.5f, g.bounds, g.world, g.rng));
			g.boxPtrs.push_back(Box::SpawnPos({ currPos.x - (currSize / 2),currPos.y - (currSize / 2) }, currSize / 3.5f, g.bounds, g.world, g.rng));

			for (auto it = g.boxPtrs.begin(); it != g.boxPtrs.end(); ++it)
			{
				if (it->get() == b)
				{
					g.boxPtrs.erase(it);
					break;
				}
			}
		}
	}));
}

void InitPCFunctionSet(Game& g)
{
	g.pFuncs.insert({ "SetToWhite", MakePCFUNCptr_SetToWhite(g) });
	g.pFuncs.insert({ "Destruct", MakePCFUNCptr_Destruct(g) });
	g.pFuncs.insert({ "SplitIntoFour", MakePCFUNCptr_SplitIntoFour(g) });
}