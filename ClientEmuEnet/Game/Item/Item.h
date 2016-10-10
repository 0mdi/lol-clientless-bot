#pragma once
#include <vector>

class Item
{
public:
	Item(int itemId)
		: m_itemId(itemId)
		, m_stack(1)
	{}

	int getItemId()
	{
		return m_itemId;
	}

	void addStack()
	{
		if (m_stack >= 5)
			return;

		++m_stack;
	}

	void removeStack()
	{
		--m_stack;
	}

	uint8_t getStack()
	{
		return m_stack;
	}

private:
	int m_itemId;
	uint8_t m_stack;
};