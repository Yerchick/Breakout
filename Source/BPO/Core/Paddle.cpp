// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.


#include "Core/Paddle.h"

DEFINE_LOG_CATEGORY_STATIC(LogPaddle, All, All)

using namespace Breakout;

Paddle::Paddle(const Settings& settings)
{
	for (uint32 i = 0; i < settings.paddle.width; ++i) {
		UE_LOG(LogPaddle, Display, TEXT("Creating paddle. Adding Tail at:%d"), settings.paddle.startPosition.x - i);
		m_links.AddTail(Position{ settings.paddle.startPosition.x - i , settings.paddle.startPosition.y });
	}
}

void Paddle::move(const Input& input)
{
	//UE_LOG(LogPaddle, Display, TEXT("Moving paddle"));
	TPositionPtr* removing;
	Position whereTo{ 0,0 };
	TPositionPtr* adding;
	if (input.x > 0) {
		removing = m_links.GetTail();
		whereTo = m_links.GetHead()->GetValue();
		adding = m_links.GetHead()->GetNextNode();
	} else {
		removing = m_links.GetHead();
		whereTo = m_links.GetTail()->GetValue();
		adding = m_links.GetTail();
	}
	UE_LOG(LogPaddle, Display, TEXT("Moving paddle. Head at: %d, Tail at: %d. Removing: %d, where to: %d, adding: %d"), m_links.GetHead()->GetValue().x, m_links.GetTail()->GetValue().x, removing->GetValue().x, whereTo.x, adding->GetValue().x);
	m_links.RemoveNode(removing);
	if (input.x > 0) {
		m_links.InsertNode(whereTo, adding);
		m_links.GetHead()->GetValue() += Position(input.x, 0);
	} else {
		m_links.AddTail(Position{ adding->GetValue().x + FMath::RoundToInt32(input.x) , adding->GetValue().y });
	}
}

