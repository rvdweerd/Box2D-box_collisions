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
#include "PCFunctionDef.h"

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
	InitPCFunctionSet(*this);
	
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
				//if (boxPtrs[0]->GetColorTrait() == boxPtrs[1]->GetColorTrait())
				//if (tid0 == tid1)
				//{
					//msg << "Destruct!!";
					//m_game->instructions.push_back({ boxPtrs[0],*m_game->pFuncs["Destruct"] });
					//boxPtrs[0]->MarkForDeath();
					//m_game->instructions.push_back({ boxPtrs[1],*m_game->pFuncs["Destruct"] });
				//}
				//else
				//{
				//	//msg << "Multiply!!";
				//	//m_game->instructions.push_back({ boxPtrs[0],*m_game->pFuncs["SplitIntoFour"] });
				//	//m_game->instructions.push_back({ boxPtrs[1],*m_game->pFuncs["SplitIntoFour"] });
				//}
				OutputDebugStringA(msg.str().c_str());

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
	world.Step( 1.0f/40.0f,8,3 );

	while (!wnd.kbd.KeyIsEmpty())
	{
		auto keyevent = wnd.kbd.ReadKey();
		if (keyevent.IsRelease())
		{
			if (keyevent.GetCode() == VK_SPACE)
			{
 				auto func = std::move(this->pFuncs["SplitIntoFour"]);
				func->operator()(boxPtrs[0].get());
				this->pFuncs["SplitIntoFour"] = std::move(func);
			}
		}
	}
	boxPtrs.erase(std::remove_if(boxPtrs.begin(), boxPtrs.end(), [](auto& b) {return b->HasToDie(); }), boxPtrs.end());
	//for (auto p : instructions)
	//{
	//	auto func = p.second;
	//	auto pActiveBox = p.first;
	//	func(pActiveBox);
	//}
	//instructions.clear();
}

void Game::ComposeFrame()
{
	for( const auto& p : boxPtrs )
	{
		p->Draw( pepe );
	}
}