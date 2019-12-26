
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

class MachineStrat : public IStrategy
{
public:
    Action get_action(const Player &self, const Player &enemy) override
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

    void on_round_end(Action self_action, Action enemy_action) override {}

    void on_game_over(GameOutcome outcome) override {}
};

IStrategy *IStrategy_new()
{
    return new MachineStrat();
}
