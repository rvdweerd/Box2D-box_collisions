/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	Game.cpp																			  *
*	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "Box.h"
#include <algorithm>
#include <sstream>
#include <typeinfo>
#include <functional>
#include <iterator>
#include <map>

void DefinePCFunctions(PCFUNC& SetToWhite)
{
	SetToWhite = [](Box* b)
	{
		if (b == nullptr)
		{
			int k = 0;
		}

		b->SetColorToWhite();
		b->SetAction(Box::PostCollisionBehavior::NoAction);
		//return std::move(m_boxPtrs);
	};

}

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	world( { 0.0f,0.0f } ),
	pepe( gfx )
{
	pepe.effect.vs.cam.SetPos( { 0.0,0.0f } );
	pepe.effect.vs.cam.SetZoom( 1.0f / boundarySize );
	
	std::generate_n( std::back_inserter( boxPtrs ),nBoxes,[this]() {
		return Box::Spawn( boxSize,bounds,world,rng );
	} );

	bool WhiteExists = false;
	for (auto i =0; i<nBoxes;i++)
	{
		if (boxPtrs[i]->GetColorTrait().GetColor().dword == Colors::White.dword)
		{
			WhiteExists = true;
			break;
		}
	}
	if (!WhiteExists)
	{
		boxPtrs.push_back(  Box::SpawnWhite(boxSize,bounds,world,rng) );
	}
	DefinePCFunctions(SetToWhite);
	Destruct = [&](Box* b)
	{
		if (b == nullptr)
		{
			int k = 0;
		}

		for (auto it = boxPtrs.begin(); it != boxPtrs.end(); ++it)
		{
			if (it->get() == b)
			{
				boxPtrs.erase(it);
				break;
			}
		}
	};
	SplitIntoFour = [&](Box* b)
	{
		if (b == nullptr)
		{
			int k = 0;
		}
		float currSize = b->GetSize();
		Vec2 currPos = b->GetPosition();
		if (currSize >= boxSize / 18)
		{
			boxPtrs.push_back(Box::SpawnPos({ currPos.x + (currSize / 2),currPos.y + (currSize / 2) }, currSize / 3.5f, bounds, world, rng));
			boxPtrs.push_back(Box::SpawnPos({ currPos.x + (currSize / 2),currPos.y - (currSize / 2) }, currSize / 3.5f, bounds, world, rng));
			boxPtrs.push_back(Box::SpawnPos({ currPos.x - (currSize / 2),currPos.y + (currSize / 2) }, currSize / 3.5f, bounds, world, rng));
			boxPtrs.push_back(Box::SpawnPos({ currPos.x - (currSize / 2),currPos.y - (currSize / 2) }, currSize / 3.5f, bounds, world, rng));
		}
		for (auto it = boxPtrs.begin(); it != boxPtrs.end(); ++it)
		{
			if (it->get() == b)
			{
				boxPtrs.erase(it);
				break;
			}
		}
		while (!instructQueue.empty())
		{
			instructQueue.pop();
		}
	};

	class Listener : public b2ContactListener
	{
	public:
		Game* m_game;
		Listener(Game* game)
			:
			m_game(game)
		{
		}
		void BeginContact( b2Contact* contact ) override
		{
			const b2Body* bodyPtrs[] = { contact->GetFixtureA()->GetBody(),contact->GetFixtureB()->GetBody() };
			if( bodyPtrs[0]->GetType() == b2BodyType::b2_dynamicBody &&
				bodyPtrs[1]->GetType() == b2BodyType::b2_dynamicBody )
			{
				Box* boxPtrs[] = { 
					reinterpret_cast<Box*>(bodyPtrs[0]->GetUserData()),
					reinterpret_cast<Box*>(bodyPtrs[1]->GetUserData())
				};
				auto& tid0 = typeid(boxPtrs[0]->GetColorTrait());
				auto& tid1 = typeid(boxPtrs[1]->GetColorTrait());

				std::stringstream msg;
				msg << "Collision between " << tid0.name() << " and " << tid1.name() << std::endl;
				OutputDebugStringA( msg.str().c_str() );

				if (boxPtrs[0]->GetColorTrait().GetColor().dword != boxPtrs[1]->GetColorTrait().GetColor().dword )
				{
					m_game->instructQueue.push({ boxPtrs[0],m_game->SplitIntoFour });
				}
				else if (boxPtrs[0]->GetColorTrait().GetColor().dword == Colors::White.dword || boxPtrs[1]->GetColorTrait().GetColor().dword == Colors::White.dword)
				{
					m_game->instructQueue.push({ boxPtrs[0],m_game->SetToWhite });
						//PostCollisionFunctions::SetToWhite });
					m_game->instructQueue.push({ boxPtrs[1],m_game->SetToWhite });
				}				
			}
		}
	};
	static Listener mrLister(this);
	world.SetContactListener( &mrLister );
	
}

void Game::Go()
{
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	const float dt = ft.Mark();
	world.Step( dt,8,3 );
	while (!instructQueue.empty())
	{
		auto func = instructQueue.front().second;
		auto pActiveBox = instructQueue.front().first;
		instructQueue.pop();
		func(pActiveBox);
		
	}
}

void Game::ComposeFrame()
{
	for( const auto& p : boxPtrs )
	{
		p->Draw( pepe );
	}
}