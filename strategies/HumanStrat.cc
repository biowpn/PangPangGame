
#include "strategy.h"

#include <iostream>
#include <vector>

class HumanStrat : public IStrategy
{
public:
    Action get_action(const Player &self, const Player &enemy) override
    {
        Action action;
        char input;
        char expl[128];
        while (true)
        {
            // print at most last 3 rounds' actions
            for (unsigned n = 3; n > 0; --n)
            {
                if (m_self_actions.size() >= n && m_enemy_actions.size() >= n)
                {
                    printf("-%d round actions: %c - %c\n",
                           n,
                           *(m_self_actions.rbegin() + n - 1),
                           *(m_enemy_actions.rbegin() + n - 1));
                }
            }
            printf("You have %d chi; enemy has %d chi\n",
                   self.qi,
                   enemy.qi);
            printf("You can%s reflect; enemy can%s reflect\n",
                   (self.can_reflect ? "" : "not"),
                   (enemy.can_reflect ? "" : "not"));

            std::cout << "Enter your action: ";
            std::cin >> input;
            action = Action(input);

            if (is_action_valid(action, self, expl))
            {
                break;
            }
            else
            {
                std::cout << "invalid action: " << expl << std::endl;
            }
        }
        return action;
    }

    void on_round_end(Action self_action, Action enemy_action) override
    {
        printf("You chose %c; enemy chose %c\n", self_action, enemy_action);
        m_self_actions.push_back(self_action);
        m_self_actions.push_back(enemy_action);
    }

    void on_game_over(GameOutcome outcome) override
    {
        switch (outcome)
        {
        case GameOutcome::Win:
            std::cout << "You win!" << std::endl;
            break;
        case GameOutcome::Lose:
            std::cout << "You lose!" << std::endl;
            break;
        case GameOutcome::Draw:
            std::cout << "Draw" << std::endl;
            break;
        default:
            break;
        }
        m_self_actions.clear();
        m_enemy_actions.clear();
    }

private:
    std::vector<Action> m_self_actions;
    std::vector<Action> m_enemy_actions;
};

IStrategy *IStrategy_new()
{
    return new HumanStrat();
}
