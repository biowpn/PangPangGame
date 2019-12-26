
/**
 * Machine strategy that analyzes historical moves.
 */

#include "strategy.h"

#include <algorithm>
#include <array>
#include <cstring>
#include <iostream>
#include <random>
#include <unordered_map>

int randrange(int lower, int upper)
{
    return lower + rand() % (upper - lower);
}

template <class It>
It randchoice(It first, It last)
{
    return std::next(first, randrange(0, last - first));
}

template <class It, class It_w>
It randchoice(It first, It last, It_w weight_first)
{
    int sum_weight = 0;
    It_w weight_first2 = weight_first;
    for (It first2 = first; first2 != last; ++first2)
    {
        sum_weight += *weight_first2++;
    }

    int t = randrange(0, sum_weight);
    for (; first != last; ++first)
    {
        t -= *weight_first++;
        if (t < 0)
        {
            break;
        }
    }
    return first;
}

// use an unsigned int to represent a game state
void to_state(unsigned int &state, const Player &self, const Player &enemy)
{
    char buf[4];
    buf[0] = self.qi;
    buf[1] = self.can_reflect;
    buf[2] = enemy.qi;
    buf[3] = enemy.can_reflect;
    state = *(int *)(buf);
}

// decode game state from an unsigned int
void from_state(unsigned int state, Player &self, Player &enemy)
{
    char buf[4];
    memcpy(buf, &state, 4);
    self.qi = buf[0];
    self.can_reflect = buf[1];
    enemy.qi = buf[2];
    enemy.can_reflect = buf[3];
}

// find index in action_map for action @param a
unsigned get_action_index(Action a)
{
    switch (a)
    {
    case Action::Absorb:
        return 0;
    case Action::Attack1:
    case Action::Attack2:
    case Action::Attack3:
    case Action::Attack4:
    case Action::Attack5:
        return (int)a - (int)Action::Attack1 + 1;
    case Action::Defend:
        return 6;
    case Action::Reflect:
        return 7;
    default:
        return 8;
    }
}

// get best response given enemy action and self state
Action get_best_response(Action a_enemy, const Player &self)
{
    switch (a_enemy)
    {
    case Action::Absorb:
        return self.qi > 0 ? Action::Attack1 : Action::Absorb;
    case Action::Defend:
        return self.qi >= 3 ? Action::Attack3 : Action::Absorb;
    case Action::Reflect:
        return self.qi >= 5 ? Action::Attack5 : Action::Absorb;
    case Action::Attack1:
        return self.can_reflect ? Action::Reflect : (self.qi >= 2 ? Action::Attack2 : Action::Defend);
    case Action::Attack2:
        return self.can_reflect ? Action::Reflect : (self.qi >= 3 ? Action::Attack3 : Action::Defend);
    case Action::Attack3:
        if (self.can_reflect)
        {
            return Action::Reflect;
        }
        else if (self.qi >= 4)
        {
            return Action::Attack4;
        }
        else if (self.qi >= 3)
        {
            return Action::Attack3;
        }
        else
        {
            return Action::Absorb;
        }
    case Action::Attack4:
        if (self.can_reflect)
        {
            return Action::Reflect;
        }
        else if (self.qi >= 5)
        {
            return Action::Attack5;
        }
        else if (self.qi >= 4)
        {
            return Action::Attack4;
        }
        else
        {
            return Action::Absorb;
        }
    case Action::Attack5:
        if (self.qi >= 5)
        {
            return Action::Attack5;
        }
        else
        {
            return Action::Absorb;
        }
    default:
        return Action::Absorb;
        break;
    }
}

constexpr Action action_map[8] = {Action::Absorb, Action::Attack1,
                                  Action::Attack2, Action::Attack3,
                                  Action::Attack4, Action::Attack5,
                                  Action::Defend, Action::Reflect};

class MachineStrat : public IStrategy
{
public:
    Action get_action(const Player &self, const Player &enemy) override
    {
        // record the current state
        to_state(m_cur_state, self, enemy);

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

        // default distribution of possible enemy actions
        std::array<unsigned, 8> enemy_action_dist_default{};
        enemy_action_dist_default[0] = 1; // can always ABSORB
        enemy_action_dist_default[6] = 1; // can always DEFEND
        enemy_action_dist_default[7] = enemy.can_reflect;
        for (unsigned qi = 1; qi <= 5; ++qi)
        {
            if (enemy.qi >= qi)
            {
                enemy_action_dist_default[qi] = 1;
            }
        }

        // If we have history of the current state, use it instead of default
        unsigned *enemy_action_dist = enemy_action_dist_default.data();
        auto it = m_enemy_action_history.find(m_cur_state);
        if (it != m_enemy_action_history.end())
        {
            enemy_action_dist = it->second.data();
        }

        // distribution of my best response, depending on my current state
        unsigned best_response_dist[8] = {};
        for (size_t i = 0; i < 8; ++i)
        {
            // for each possible enemy action with some weight,
            // get best response and increase its weight
            auto best_response = get_best_response(action_map[i], self);
            auto best_i = get_action_index(best_response);
            best_response_dist[best_i] += enemy_action_dist[i];
        }

        // of all the best reponses, randomly chose one
        return *randchoice(action_map, action_map + 8, best_response_dist);
    }

    void on_round_end(Action self_action, Action enemy_action) override
    {
        auto it = m_enemy_action_history.find(m_cur_state);
        if (it == m_enemy_action_history.end())
        {
            std::array<unsigned, 8> m_enemy_action{};
            it = m_enemy_action_history.emplace(m_cur_state, m_enemy_action).first;
        }
        auto idx = get_action_index(enemy_action);
        ++it->second[idx];
    }

    void on_game_over(GameOutcome outcome) override
    {
        // if (++m_game_count % 10000 == 0)
        // {
        //     printf("s.qi\ts.r\te.qi\te.r\t|\ta\t1\t2\t3\t4\t5\td\tr\t\n");
        //     for (const auto &kv : m_enemy_action_history)
        //     {
        //         auto &state = kv.first;
        //         auto &enemy_action_dist = kv.second;
        //         Player self, enemy;
        //         from_state(state, self, enemy);
        //         printf("%d\t%d\t%d\t%d\t|\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
        //                self.qi, self.can_reflect, enemy.qi, enemy.can_reflect,
        //                enemy_action_dist[0],
        //                enemy_action_dist[1],
        //                enemy_action_dist[2],
        //                enemy_action_dist[3],
        //                enemy_action_dist[4],
        //                enemy_action_dist[5],
        //                enemy_action_dist[6],
        //                enemy_action_dist[7]);
        //     }
        //     std::cout << "num states: " << m_enemy_action_history.size() << std::endl;
        // }
    }

private:
    unsigned m_cur_state;
    std::unordered_map<unsigned, std::array<unsigned, 8>> m_enemy_action_history;

    unsigned m_game_count{0};
};

IStrategy *IStrategy_new()
{
    return new MachineStrat();
}
