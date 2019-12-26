
#include "game.h"
#include "strategy.h"

#include <ctime>
#include <iostream>
#include <random>

/**
 * @return 1 if Player 1 wins, -1 if Player 2 wins, 0 if draw
 */
int game_loop(Strategy_t p1_strat,
              Strategy_t p2_strat,
              std::vector<Action> &p1_actions,
              std::vector<Action> &p2_actions)
{
    Player p1, p2;
    Action a1, a2;

    while (!is_game_over(p1, p2))
    {
        a1 = p1_strat(p1, p2, p1_actions, p2_actions);
        if (!is_action_valid(a1, p1))
        {
            return -1;
        }
        p1_actions.push_back(a1);

        a2 = p2_strat(p2, p1, p2_actions, p1_actions);
        if (!is_action_valid(a2, p2))
        {
            return 1;
        }
        p2_actions.push_back(a2);

        process_round_actions(p1, a1, a2, p2);
    }

    return (p2.qi < 0) - (p1.qi < 0);
}

Strategy_t LoadStrategy(const char *lpath)
{
    auto hLib = LoadLibrary(TEXT(lpath));
    if (hLib == NULL)
    {
        std::cerr << "failed to open library " << lpath << std::endl;
        exit(-1);
    }

    auto hFunc = (Strategy_t)GetProcAddress(hLib, "GetAction");
    if (hFunc == NULL)
    {
        std::cerr << "failed to load symbol 'GetAction' from " << lpath << std::endl;
        exit(-1);
    }

    return hFunc;
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
        int outcome = game_loop(p1_strat, p2_strat, p1_actions, p2_actions);
        if (outcome > 0)
        {
            ++p1_wins;
            p1_actions.push_back(Action::Win);
            p2_actions.push_back(Action::Lose);
        }
        else if (outcome < 0)
        {
            ++p2_wins;
            p1_actions.push_back(Action::Lose);
            p2_actions.push_back(Action::Win);
        }
        else
        {
            ++draws;
            p1_actions.push_back(Action::Draw);
            p2_actions.push_back(Action::Draw);
        }
    }

    std::cout << "Out of " << num_games << " game(s): " << std::endl;
    printf("Player 1 won %d game(s) (%.2f%%)\n", p1_wins, (double)p1_wins / num_games * 100);
    printf("Player 2 won %d game(s) (%.2f%%)\n", p2_wins, (double)p2_wins / num_games * 100);
    printf("Draw %d game(s) (%.2f%%)\n", draws, (double)draws / num_games * 100);

    return 0;
}
