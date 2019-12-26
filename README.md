# PangPangGame

A two-player round-based game. Each player makes a move simultaneously in every round (like Rock-Paper-Scissor).

## Rules


There are four types of moves: *ABSORB*, *ATTACK*, *DEFEND*, and *REFLECT*.

- *ABSORB*: gain one unit of Qi (mysterious energy).

- *ATTACK*: consume Qi to attack. Level *N* attack consumes *N* unit(s) of Qi, where *N* is an integer in [1, 5]. *N* must also be no more than the unit(s) of Qi you have.

- *DEFEND*: Defuse level 1 and level 2 attacks. Has no effect on level 3+ attacks.

- *REFLECT*: Reflect the opponent's attack to himself. Has no effect on level 5 attack. A player can only reflect once in a game.

Further:

- If you *ATTACK* and your opponent *ABSORB*, you win.

- If both of you *ATTACK*, and your attacking level is greater than that of your opponent's, you win.

- If both of you *ATTACK* on the same level, the game continues.

- If your opponent *ATTACK* on level 4 or less, and you *REFLECT*, you win.

- If you *ATTACK* on level 5 and your opponent doesn't do the same, you win.

- If both of you *ATTACK* on level 5, the game is a draw.

## Build

This project uses CMake for building system.

For now, support Windows only.

## Usage

Each strategy is a DLL, which implements an `IStrategy` interface. The most important function of which is

```
virtual Action get_action(const Player &self, const Player &enemy) = 0;
```

Some example strategies are given in [strategies/](strategies/).

The executable `main.exe` takes at least two parameters, which are the paths to two strategy DLLs. The third parameter is the number of games to play, default 100.

```
./main.exe ./libHumanStrat.dll ./libMachineStrat1.dll 100
```

The `HumanStrat` simply asks the human player what to do each round via command line. It also displays some game info. To indicate round action, the user should enter

- `a` for *ABSORB*

- *n* for *ATTACK* on level *n*, where *n* = `1`, `2`, `3`, `4`, `5`

- `d` for *DEFEND*

- `r` for *REFLECT*

Two machine strategies (I chose not to use the term "AI" because it's not intelligent at all) are implemented: `MachineStrat0` and `MachineStrat1`. 

`MachineStrat1` is a little bit cleverer so one would expect it beats `MachineStrat0`. In fact, if you do

```
./main.exe ./libMachineStrat1.dll ./libMachineStrat0.dll 10000
```

A possible output would be

```
Out of 10000 game(s): 
Player 1 won 5216 game(s) (52.16%)
Player 2 won 4784 game(s) (47.84%)
Draw 0 game(s) (0.00%)
```

But it's far from enough to win against human players.
