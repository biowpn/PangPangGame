
#pragma once

#include "game.h"

#include <windows.h>
#include <vector>

typedef Action (*Strategy_t)(const Player &,
                             const Player &,
                             const std::vector<Action> &,
                             const std::vector<Action> &);

extern "C"
{
    __declspec(dllexport) Action __cdecl GetAction(const Player &self,
                                                   const Player &enemy,
                                                   const std::vector<Action> &self_actions,
                                                   const std::vector<Action> &enemy_actions);
}
