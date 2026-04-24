
#include "BjDealing.h"

static bool SameSame(int one, int two, int three)
{
	if (one == two && one == three)
		return (true);
	return (false);
}

static int Get213(DealQueue &one, DealQueue &two, DealQueue &opp)
{
	std::vector<int> nums = {one.card->number, two.card->number, opp.card->number};
	std::sort(nums.begin(), nums.end());
	if (SameSame(one.card->suit, two.card->suit, opp.card->suit) &&
		SameSame(one.card->number, two.card->number, opp.card->number))
		return (5);
	if (SameSame(one.card->suit, two.card->suit, opp.card->suit))
	{
		if (nums[0] + 1 == nums[1] && nums[0] + 2 == nums[2])
			return (4);
		if (nums[0] == 0 && nums[1] == 11 && nums[2] == 12)
			return (4);
	}
	if (SameSame(one.card->number, two.card->number, opp.card->number))
		return (3);
	if (nums[0] + 1 == nums[1] && nums[0] + 2 == nums[2])
			return (2);
	if (nums[0] == 0 && nums[1] == 11 && nums[2] == 12)
		return (2);
	if (SameSame(one.card->suit, two.card->suit, opp.card->suit))
		return (1);
	return (0);
}

std::tuple<int, int> CheckInitialCards(DealQueue &one, DealQueue &two, DealQueue &opp)
{
	std::tuple<int, int> ret = {0, 0};
	if (one.card->number == two.card->number)
	{
		if (one.card->suit == two.card->suit)
			std::get<0>(ret) = 3;
		else if (one.card->suit == 0 && two.card->suit == 3)
			std::get<0>(ret) = 2;
		else if (one.card->suit == 1 && two.card->suit == 2)
			std::get<0>(ret) = 2;
		else if (one.card->suit == 3 && two.card->suit == 0)
			std::get<0>(ret) = 2;
		else if (one.card->suit == 2 && two.card->suit == 1)
			std::get<0>(ret) = 2;
		else
			std::get<0>(ret) = 1;
	}
	std::get<1>(ret) = Get213(one, two, opp);
	return (ret);
}

bool CheckSplit(DealQueue &one, DealQueue &two)
{
	if (one.card->value == two.card->value)
		return (true);
	return (false);
}

bool CheckBJ(DealQueue &one, DealQueue &two)
{
	if (one.card->value == 10 && two.card->value == 0)
		return (true);
	if (two.card->value == 10 && one.card->value == 0)
		return (true);
	return (false);
}

std::vector<DealQueue> GetStackCards(int stack, std::vector<DealQueue> &used, DealQueue &curr)
{
	std::vector<DealQueue> ret;
	if (curr.stack == stack)
		ret.push_back(curr);
	for (int i = 0; i < used.size(); i++)
	{
		if (used[i].stack == stack)
			ret.push_back(used[i]);
	}
	return (ret);
}
