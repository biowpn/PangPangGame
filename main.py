from pangpanggameAI import *
print("Welcome to Pang Pang game V1.0！This program is writen by Yuan(ymengad@ust.hk)\n\n")
print("[Introduction]")
print("This is a Strategy Game by two players. Each player makes one move simultaneously in every round.\n")
print("There are four kinds of moves: ABSORB, ATTACK, DEFEND and REFLECT.")
print("ABSORB: Gain one unit of Chi.")
print("ATTACK: Consume Chi to attack. Level N attack consumes N unit(s) of Chi, where N is an integer determined by you. N must be between 1 and the unit(s) of Chi you have.")
print("DEFEND: Defuse level 1 and level 2 attack. Level 3 or above attack can overrun it!")
print("REFLECT: Reflect opponent's attack to himself and defeat him! Except that level 5 or above attack can overrun it. Also, one can reflect only once in a game.\n")
print("If you attack and your opponent absorbs, you win. Vice versa.")
print("If your level of ATTACK is greater than that of your opponent, or you overrun his defense or reflection, you win. Vice versa.")
print("If your level of ATTACK is the same as that of your opponent, you both will be fine.\n\n\n")
input("[Enter to start]")


def UserStrategy(p1_chi,p1_r,p2_chi,p2_r,roundCount):
    print("\nThis is Round",roundCount)
    print("Your info:",p1_chi,p1_r)
    print("Opponent info:",p2_chi,p2_r)

    while True:
        print("\nWhat's your move?")
        usercmd = input("[Enter 'a' to ABSORB, 'k' to ATTACK, 'd' to DEFEND or 'r' to REBOUND]\n")

        if usercmd == "a":
            decision = 0
            break
        elif usercmd == "k" and p1_chi > 0:
            while True:
                try:
                    level = int(input("What's the level of attack?\n"))
                    if 0 < level <= p1_chi: break
                except: pass
            decision = level
            break
        elif usercmd == "d":
            decision = -1
            break
        elif usercmd == "r" and p1_r:
            decision = -2
            break

    f = open(history,'a')
    f.write(str(p2_chi)+"\t"+str(p2_r)+"\t"+str(p1_chi)+"\t"+str(p1_r)+"\t"+str(decision)+"\n")
    f.close()

    return decision


def ShowStrategy(sbj, m):

    if m==0:        print(sbj+" chose to absorb")
    elif m>0:       print(sbj+" chose to attack at level ",m)
    elif m==-1:     print(sbj+" chose to defend")
    else:           print(sbj+" chose to rebound")




def GamePlay():

    roundCount = 0
    p1_chi,p1_r,p2_chi,p2_r = 0,True,0,True

    while True:
        roundCount += 1

        p1_move = UserStrategy(p1_chi,p1_r,p2_chi,p2_r,roundCount)
        p2_move = AI_03(p2_chi,p2_r,p1_chi,p1_r)
       
        ShowStrategy("\n\nYou",p1_move)
        ShowStrategy("I",p2_move)

        roundRes,State = ResultOfMoves(p1_move,p2_move,[p1_chi,p1_r, p2_chi,p2_r])
        p1_chi,p1_r,p2_chi,p2_r = State[0],State[1],State[2],State[3]
   
        if roundRes:
            return roundRes

history = "history.txt"
p1_wt = 0
p2_wt = 0

f = open(history,'w')
f.close()

while True:
    gameRes = GamePlay()
    if gameRes == 1:
        p1_wt += 1
        print("\nYou won!","\tp1_wt =",p1_wt,"\tp2_wt =",p2_wt)        
    elif gameRes == -1:
        p2_wt += 1
        print("\nYou lose.","\tp1_wt =",p1_wt,"\tp2_wt =",p2_wt)

    print("="*16+"\n\tRestart\n"+"="*16)



    
    

