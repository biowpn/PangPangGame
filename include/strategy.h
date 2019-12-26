
#pragma once

#include "game.h"

#include <windows.h>

class IStrategy
{
public:
    virtual Action get_action(const Player &self, const Player &enemy) = 0;

    virtual void on_round_end(Action self_action, Action enemy_action) = 0;

    virtual void on_game_over(GameOutcome outcome) = 0;
};

extern "C"
{
    __declspec(dllexport) IStrategy *__cdecl IStrategy_new();
}
