
/**
 * Somewhat "cleverer" machine strategy, compared to MachineStrat0
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
        // Some best plays are obvious
        if (self.qi >= 5)
        {
            return Action::Attack5;
        }
        else if (self.qi >= 3 && !enemy.can_reflect)
        {
            return Action::Attack3;
        }
        else if (self.qi == 4 && enemy.qi == 0)
        {
            return Action::Absorb;
        }
        else if (self.qi == 2 && enemy.qi == 0 && !enemy.can_reflect)
        {
            return Action::Absorb;
        }
        else if (self.qi == 0 && enemy.qi == 0)
        {
            return Action::Absorb;
        }

        // Others are not, use a little bit randomneses
        int t = randrange(0, 100);

        if (self.qi >= 3) // Advantage
        {
            if (enemy.qi >= 3)
            {
                if (t < 20)
                {
                    return Action::Absorb;
                }
                else if (t < 70)
                {
                    return Action((int)Action::Attack1 + (self.qi - 1));
                }
                else
                {
                    if (self.can_reflect)
                    {
                        return Action::Reflect;
                    }
                    else
                    {
                        return Action((int)Action::Attack1 + (self.qi - 1));
                    }
                }
            }
            else if (enemy.qi > 0 && enemy.can_reflect)
            {
                if (t < 15)
                {
                    return Action::Absorb;
                }
                else if (t < 75)
                {
                    return Action((int)Action::Attack1 + (self.qi - 1));
                }
                else
                {
                    return Action::Defend;
                }
            }
            else if (enemy.can_reflect)
            {
                if (t < 50)
                {
                    return Action::Absorb;
                }
                else
                {
                    return Action((int)Action::Attack1 + (self.qi - 1));
                }
            }
        }
        else if (enemy.qi >= 3) // Disadvantage
        {
            if (self.can_reflect && self.qi > 0)
            {
                if (t < 50)
                {
                    return Action::Reflect;
                }
                else if (t < 75)
                {
                    return Action((int)Action::Attack1 + (self.qi - 1));
                }
                else
                {
                    return Action::Absorb;
                }
            }
            else if (self.can_reflect)
            {
                if (t < 50)
                {
                    return Action::Reflect;
                }
                else
                {
                    return Action::Absorb;
                }
            }
            else if (self.qi > 0)
            {
                if (t < 50)
                {
                    return Action((int)Action::Attack1 + (self.qi - 1));
                }
                else
                {
                    return Action::Absorb;
                }
            }
            else
            {
                return Action::Absorb;
            }
        }
        else // Stand-off
        {
            std::vector<Action> choices{Action::Absorb};
            if (enemy.qi > 0)
            {
                choices.push_back(Action::Defend);
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
            return *randchoice(choices.begin(), choices.end());
        }

        return Action::Absorb;
    }

    void on_round_end(Action self_action, Action enemy_action) override {}

    void on_game_over(GameOutcome outcome) override {}
};

IStrategy *IStrategy_new()
{
    return new MachineStrat();
}
