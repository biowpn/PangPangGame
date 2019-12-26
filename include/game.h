
#pragma once

enum class GameOutcome: char
{
    Win = 'W',
    Lose = 'L',
    Draw = 'D',
};

enum class Action : char
{
    Absorb = 'a',
    Defend = 'd',
    Reflect = 'r',
    Attack1 = '1',
    Attack2 = '2',
    Attack3 = '3',
    Attack4 = '4',
    Attack5 = '5',
};

struct Player
{
    char qi = 0;
    bool can_reflect = true;
};

/**
 * check whether an action is valid.
 * 
 * @return true if valid, and false otherwise
 */
bool is_action_valid(Action a, const Player &p);

/**
 * check whether an action is valid, and if it is not explain the reason why.
 * 
 * @return true if valid, and false otherwise
 */
bool is_action_valid(Action a, const Player &p, char *expl);

/**
 * process actions of one round.
 * both actions are assumed to be valid.
 */
void process_round_actions(Player &p1, Action a1, Action a2, Player &p2);

/**
 * check whether the game is over.
 */
bool is_game_over(const Player &p1, const Player &p2);
