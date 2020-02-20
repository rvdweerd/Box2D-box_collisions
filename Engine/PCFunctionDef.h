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
		//for (auto it = g.boxPtrs.begin(); it != g.boxPtrs.end(); ++it)
		//{
		//	if (it->get() == b)
		//	{
		//		g.boxPtrs.erase(it);
		//		break;
		//	}
		//}
		b->MarkForDeath();
	}));
}
std::unique_ptr<PCFUNC> MakePCFUNCptr_SplitIntoFour(Game& g)
{
	return std::move(std::make_unique<PCFUNC>([&](Box* b)
	{
		float currSize = b->GetSize();
		Vec2 currPos = b->GetPosition();
		if (currSize >= g.boxSize / 4)
		{
			const float f= float(std::sqrt(2)/2);
			const float l = f * b->GetSize(); // distance from enter of parent box to center of child boxes
			Vec2 pos0 = b->GetPosition(); // center of box in world coord
			float rot = PI / 4 + b->GetAngle(); // angle of boxes + 45 degrees (to get from parent box center to child box center)
			const Vec2 d_pos1 = {  l * cos(rot) ,  l * sin(rot) }; // center of 1st (of 4) child box relative to parent box center in world coord
			rot += PI / 2;
			const Vec2 d_pos2 = { l * cos(rot) ,  l * sin(rot) };
			rot += PI / 2;
			const Vec2 d_pos3 = { l * cos(rot) ,  l * sin(rot) };
			rot += PI / 2;
			const Vec2 d_pos4 = { l * cos(rot) ,  l * sin(rot) };
			g.boxPtrs.push_back(std::make_unique<Box>(b->GetColorTrait().Clone(), g.world, pos0 + d_pos1, b->GetSize()/2,   b->GetAngle(), b->GetVelocity(), b->GetAngularVelocity()));
			g.boxPtrs.push_back(std::make_unique<Box>(b->GetColorTrait().Clone(), g.world, pos0 + d_pos2, b->GetSize() / 2, b->GetAngle(), b->GetVelocity(), b->GetAngularVelocity()));
			g.boxPtrs.push_back(std::make_unique<Box>(b->GetColorTrait().Clone(), g.world, pos0 + d_pos3, b->GetSize() / 2, b->GetAngle(), b->GetVelocity(), b->GetAngularVelocity()));
			g.boxPtrs.push_back(std::make_unique<Box>(b->GetColorTrait().Clone(), g.world, pos0 + d_pos4, b->GetSize() / 2, b->GetAngle(), b->GetVelocity(), b->GetAngularVelocity()));
			b->MarkForDeath();
		}
	}));
}

void InitPCFunctionSet(Game& g)
{
	g.pFuncs.insert({ "SetToWhite",		MakePCFUNCptr_SetToWhite(g) });
	g.pFuncs.insert({ "Destruct",		MakePCFUNCptr_Destruct(g) });
	g.pFuncs.insert({ "SplitIntoFour",	MakePCFUNCptr_SplitIntoFour(g) });
}