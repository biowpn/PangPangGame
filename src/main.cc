
#include "game.h"
#include "strategy.h"

#include <ctime>
#include <iostream>
#include <random>

/**
 * @return 1 if Player 1 wins, -1 if Player 2 wins, 0 if draw
 */
int game_loop(IStrategy *p1_strat,
              IStrategy *p2_strat)
{
    Player p1, p2;
    Action a1, a2;
    bool a1_valid, a2_valid;

    while (!is_game_over(p1, p2))
    {
        a1 = p1_strat->get_action(p1, p2);
        a2 = p2_strat->get_action(p2, p1);

        a1_valid = is_action_valid(a1, p1);
        a2_valid = is_action_valid(a2, p2);

        if (a1_valid && a2_valid)
        {
            p1_strat->on_round_end(a1, a2);
            p2_strat->on_round_end(a2, a1);
            process_round_actions(p1, a1, a2, p2);
        }
        else if (a1_valid)
        {
            return 1;
        }
        else if (a2_valid)
        {
            return -1;
        }
        else
        {
            return 0;
        }
    }

    return (p2.qi < 0) - (p1.qi < 0);
}

IStrategy *LoadStrategy(const char *lpath)
{
    auto hLib = LoadLibrary(TEXT(lpath));
    if (hLib == NULL)
    {
        std::cerr << "failed to open library " << lpath << std::endl;
        exit(-1);
    }

    typedef IStrategy *(*IStrategy_new_t)();

    auto hFunc = (IStrategy_new_t)GetProcAddress(hLib, "IStrategy_new");
    if (hFunc == NULL)
    {
        std::cerr << "failed to load symbol 'IStrategy_new' from " << lpath << std::endl;
        exit(-1);
    }

    return hFunc();
}

int main(int argc, char **argv)
{
    srand(std::time(0));

    if (argc < 3)
    {
        std::cout << "Usage: strategy1_library strategy2_library [,num_games = 100]" << std::endl;
        return -1;
    }

    unsigned num_games = 100;
    if (argc > 3)
    {
        num_games = std::atoi(argv[3]);
    }

    auto p1_strat = LoadStrategy(argv[1]);
    auto p2_strat = LoadStrategy(argv[2]);

    std::vector<Action> p1_actions, p2_actions;
    unsigned p1_wins = 0, p2_wins = 0, draws = 0;

    for (unsigned i = 0; i < num_games; ++i)
    {
        int outcome = game_loop(p1_strat, p2_strat);
        if (outcome > 0)
        {
            ++p1_wins;
            p1_strat->on_game_over(GameOutcome::Win);
            p2_strat->on_game_over(GameOutcome::Lose);
        }
        else if (outcome < 0)
        {
            ++p2_wins;
            p1_strat->on_game_over(GameOutcome::Lose);
            p2_strat->on_game_over(GameOutcome::Win);
        }
        else
        {
            ++draws;
            p1_strat->on_game_over(GameOutcome::Draw);
            p2_strat->on_game_over(GameOutcome::Draw);
        }
    }

    std::cout << "Out of " << num_games << " game(s): " << std::endl;
    printf("Player 1 won %d game(s) (%.2f%%)\n", p1_wins, (double)p1_wins / num_games * 100);
    printf("Player 2 won %d game(s) (%.2f%%)\n", p2_wins, (double)p2_wins / num_games * 100);
    printf("Draw %d game(s) (%.2f%%)\n", draws, (double)draws / num_games * 100);

    return 0;
}
