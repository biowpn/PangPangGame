
#include "game.h"

#include <stdio.h>

bool is_action_valid(Action a, const Player &p)
{
    switch (a)
    {
    case Action::Absorb:
    case Action::Defend:
        return true;
    case Action::Reflect:
        return p.can_reflect;
    case Action::Attack1:
    case Action::Attack2:
    case Action::Attack3:
    case Action::Attack4:
    case Action::Attack5:
        return p.qi > (int)a - (int)Action::Attack1;
    default:
        return false;
    }
}

bool is_action_valid(Action a, const Player &p, char *expl)
{
    switch (a)
    {
    case Action::Absorb:
    case Action::Defend:
        return true;
    case Action::Reflect:
    {
        if (p.can_reflect)
        {
            return true;
        }
        else
        {
            sprintf(expl, "already reflected");
        }
    }
    case Action::Attack1:
    case Action::Attack2:
    case Action::Attack3:
    case Action::Attack4:
    case Action::Attack5:
    {
        int required = (int)a - (int)Action::Attack1 + 1;
        if (p.qi >= required)
        {
            return true;
        }
        else
        {
            sprintf(expl, "not enough qi (required %d, have %d)", required, p.qi);
            return false;
        }
    }
    default:
    {
        sprintf(expl, "unknown action '%c'", a);
        return false;
    }
    }
}

bool is_action_attack(Action a)
{
    return a >= Action::Attack1 && a <= Action::Attack5;
}

void process_action(Player &p, Action a)
{
    if (a == Action::Absorb)
    {
        p.qi += 1;
    }
    else if (is_action_attack(a))
    {
        p.qi -= ((int)a - (int)Action::Attack1 + 1);
    }
    else if (a == Action::Reflect)
    {
        p.can_reflect = false;
    }
}

void process_KO(Action a_self, Action a_enemy, Player &enemy)
{
    if (is_action_attack(a_self))
    {
        if (a_enemy == Action::Absorb)
        {
            enemy.qi = -1;
        }
        else if (a_enemy == Action::Defend && a_self >= Action::Attack3)
        {
            enemy.qi = -1;
        }
        else if (a_enemy == Action::Reflect && a_self >= Action::Attack5)
        {
            enemy.qi = -1;
        }
        else if (is_action_attack(a_enemy) && a_self > a_enemy)
        {
            enemy.qi = -1;
        }
    }
    else if (a_self == Action::Reflect)
    {
        if (is_action_attack(a_enemy) && a_enemy < Action::Attack5)
        {
            enemy.qi = -1;
        }
    }
}

void process_round_actions(Player &p1, Action a1, Action a2, Player &p2)
{
    process_action(p1, a1);
    process_action(p2, a2);
    process_KO(a1, a2, p2);
    process_KO(a2, a1, p1);
}

bool is_game_over(const Player &p1, const Player &p2)
{
    return p1.qi < 0 || p2.qi < 0;
}
