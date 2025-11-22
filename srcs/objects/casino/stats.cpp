
#include "stats.h"

uint64_t chips = 99;

uint64_t GetPlayerChips()
{
	return (chips);
}

void AddToPlayerChips(int64_t amount)
{
	if (amount < 0 && (amount * (-1) > chips))
	{
		chips = 0;
		return ;
	}
	chips += amount;
}

void SetPlayerChips(uint64_t amount)
{
	chips = amount;
}
