// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BPO_Types.h"

namespace Breakout
{
class  Paddle
{
public:
	Paddle(const Settings& settings);

	void move(const Input& input);
	
	const TPositionList& links() const { return m_links; }
	const TPositionPtr* body() const { return m_links.GetHead(); }
	const Position head() const { return m_links.GetHead()->GetValue(); }
	const Position tail() const { return Position(-1, 0) + m_links.GetTail()->GetValue(); }

private:
	TPositionList m_links;

};

} //namespace Breakout