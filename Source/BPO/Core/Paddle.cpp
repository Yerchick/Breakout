// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.


#include "Core/Paddle.h"

using namespace Breakout;

Paddle::Paddle(const Settings& settings)
{
	m_links.AddHead(Position{ settings.paddle.startPosition.x, settings.paddle.startPosition.y });
	for (uint32 i = 0; i < settings.paddle.width; ++i) {
		for (uint32 y = 0; y < settings.paddle.height; ++y) {
			m_links.AddTail(Position{ settings.paddle.startPosition.x - i , settings.paddle.startPosition.y+y });
		}
	}
}

void Paddle::move(const Input& input)
{

}

