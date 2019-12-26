
/**
 * Very stupid Machine strategy, always take a uniform-random valid choice
 */

#include "strategy.h"

#include <random>

int randrange(int lower, int upper)
{
    return lower + rand() % (upper - lower);
}

template <class It>
It randchoice(It first, It last)
{
    return std::next(first, randrange(0, last - first));
}

Action GetAction(const Player &self, const Player &enemy, const std::vector<Action> &self_actions,
                 const std::vector<Action> &enemy_actions)
{
    std::vector<Action> choices{Action::Absorb};
    if (enemy.qi > 0)
    {
        if (enemy.qi < 3)
        {
            choices.push_back(Action::Defend);
        }
        if (self.can_reflect)
        {
            choices.push_back(Action::Reflect);
        }
    }
    if (self.qi >= 1)
    {
        choices.push_back(Action::Attack1);
    }
    if (self.qi >= 2)
    {
        choices.push_back(Action::Attack2);
    }
    if (self.qi >= 3)
    {
        choices.push_back(Action::Attack3);
    }
    if (self.qi >= 4)
    {
        choices.push_back(Action::Attack4);
    }
    if (self.qi >= 5)
    {
        choices.push_back(Action::Attack5);
    }
    return *randchoice(choices.begin(), choices.end());
}
