
#include "strategy.h"

#include <iostream>

Action GetAction(const Player &self,
                 const Player &enemy,
                 const std::vector<Action> &self_actions,
                 const std::vector<Action> &enemy_actions)
{
    Action action;
    char input;
    char expl[128];
    while (true)
    {
        // print at most last 3 rounds' actions
        for (unsigned n = 3; n > 0; --n)
        {
            if (self_actions.size() >= n && enemy_actions.size() >= n)
            {
                printf("-%d round actions: %c - %c\n",
                       n,
                       *(self_actions.rbegin() + n - 1),
                       *(enemy_actions.rbegin() + n - 1));
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
