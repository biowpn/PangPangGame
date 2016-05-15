import random

def AI_01(s_chi,s_r,o_chi,o_r,max_depth = 2):

    #print('\n'+"="*32)
    def Evaluate(x,y, state, depth):

        #print('\n'+'\t'*depth,"|",x,",",y,sep='',end=' ')
        Res, state = ResultOfMoves(x,y, state)
        s_chi,s_r,o_chi,o_r = state[0],state[1],state[2],state[3]

        if Res == 1:
            #print("->",10,end='')
            return 10
        elif Res == -1:
            #print("->",-10,end='')
            return -10    
        if depth >= max_depth-1:
            score = (s_chi-o_chi)*2 + (s_r-o_r)*4
            #print("->",score,end='')
            return score

        Scores = list()
        for x in range(-(1+s_r)*bool(o_chi),s_chi+1):
            for y in range(-(1+o_r)*bool(s_chi),o_chi+1):
                Scores.append(Evaluate(x,y, state, depth+1))

        #print('\n'+'\t'*depth+" =>",sum(Scores)/len(Scores),end='')
        return sum(Scores)/len(Scores)



    CurrentState = (s_chi,s_r,o_chi,o_r)

    Strategies = dict()
    Choices = []
    Scores = []
    for x in range(-(1+s_r)*bool(o_chi),s_chi+1):
        Strategies[x] = list()
        for y in range(-(1+o_r)*bool(s_chi),o_chi+1):
            Strategies[x].append(Evaluate(x,y,CurrentState,0))
        score = sum(Strategies[x])/len(Strategies[x])
        if score > 9.99:
            return x
        Strategies[x] = score
        Choices.append(x)
        Scores.append(score)

##    print('\n'+"="*32,"\n\n::Analysis::")
##    for (i,v) in Strategies.items():
##        print("Action:",i,"\t->","\tValue:",round(v,3))
##    print("::End Analysis::\n")


    return Choices[Scores.index(max(Scores))]

    if len(Strategies) == 1:
        return 0

    Scores = [s-min(Scores) for s in Scores]
    t = random.random()
    for i in range(len(Scores)):
        p1 = sum(Scores[0:i])/sum(Scores)
        p2 = sum(Scores[0:i+1])/sum(Scores)
        if p1 <= t < p2:
            return Choices[i]

            

        
def AI_02(s_chi,s_r,o_chi,o_r):
        
    if (s_chi ==2 and o_chi==o_r==0) or ((s_chi ==4 or s_chi==0) and o_chi==0):
        return 0
    elif (s_chi >= 3 and o_r == 0) or (s_chi >= 5):
        return s_chi 
    else:
        return random.randrange((-1-s_r)*bool(o_chi),s_chi+1)



def AI_03(s_chi,s_r,o_chi,o_r):
        
    if (s_chi ==2 and o_chi==o_r==0) or ((s_chi ==4 or s_chi==0) and o_chi==0):
        return 0
    elif (s_chi >= 3 and o_r == 0) or (s_chi >= 5):
        return s_chi                                          
    else:
        if s_chi >= 3:                        
            if o_chi >= 3:
                t2=random.uniform(1,100)
                if t2<10:
                    return 0
                elif t2<70:
                    return s_chi
                else:
                    if s_r ==1:
                        return -2
                    else:
                        return s_chi
            else:
                if s_r == 1:
                    t2=random.uniform(1,100)
                    if t2 < 10:
                        return 0
                    elif t2 < 55:
                        return s_chi
                    elif t2 < 85:
                        return -1
                    else:
                        return -2
                else:
                    t2=random.uniform(1,100)
                    if t2 < 20:
                        return 0
                    elif t2 < 75:
                        return s_chi
                    else:
                        return -1
        else:
            if o_chi == 0:
                if s_chi > 0:
                    t2=random.uniform(1,100)
                    if t2 < 35:
                        return 1
                    else:
                        return 0
                else:
                    return 0
            else:
                if s_r == 1:
                    if o_chi >= 3:
                        t2=random.uniform(1,100)
                        if t2 < 60:
                            return -2
                        else:
                            if s_chi > 0:
                                return s_chi
                            else:
                                return 0
                    else:
                        if s_chi > 0:
                            return random.randrange((-1-s_r)*bool(o_chi),min(o_chi+1,s_chi)+1)
                        else:
                            return random.sample([-2,-1,0],1)[0]
                else:
                    if s_chi > 0:
                        return random.sample([0,-1,min(o_chi+1,s_chi)],1)[0]
                    else:
                        return random.randint(-1,0)
 




def AI_04(s_chi,s_r,o_chi,o_r,historyfile):
        
    f = open(historyfile,'r')
    o_H = dict()
    for line in f:
        record = line.strip().split("\t")
        for i in range(5):
            if record[i] == "True":     record[i] = True
            elif record[i] == "False":  record[i] = False
            else:                       record[i] = int(record[i])
        record = tuple(record)
        if record[:4] not in o_H:       o_H[record[:4]] = [record[-1]]
        else:                           o_H[record[:4]].append(record[-1])


    CurrentState = (s_chi,s_r,o_chi,o_r)
    if len(o_H.get(CurrentState,[])) > 2:
        HistoryMoves = tuple(o_H[CurrentState])
        MovesPosb = []
        for move in set(HistoryMoves):
            MovesPosb.append((move,HistoryMoves.count(move)/len(HistoryMoves)))
        #print(MovesPosb)
    else:   return AI_01(s_chi,s_r,o_chi,o_r)


    CounterMeasure = {-2:(5,0),
                      -1:(3,0),
                      0:(1,0),
                      1:(-2,2,-1),
                      2:(-2,3,-1),
                      3:(-2,4,3,2,1,0),
                      4:(-2,5,4,3,2,1,0),
                      5:(5,4,3,2,1,0)}

    Strategies = []
    for move,posb in MovesPosb:
        subcm = list(CounterMeasure[move])
        for cm in subcm:
            if cm in range(-1-s_r,s_chi+1):
                Strategies.append((cm,posb))
                break

    #print(Strategies)

    t = random.random()
    s = 0
    for MP in Strategies:
        s += MP[1]
        if t < s:
            return MP[0]



def ResultOfMoves(x,y, State):
    State = list(State)
    if x > 0:
        if y == 0:          return 1, State
        elif y > 0:
            if x > y:       return 1, State
            elif x < y:     return -1, State
        elif y == -1:
            if x > 2:       return 1, State
        elif y == -2:
            if x > 4:       return 1, State
            else:           return -1, State
    elif y > 0:
        if x == 0:          return -1, State
        elif x == -1:
            if y > 2:       return -1, State
        elif x == -2:
            if y > 4:       return -1, State
            else:           return 1, State

    if x == 0:      State[0] += 1
    elif x > 0:     State[0] -= x
    elif x == -2:   State[1] = False
    if y == 0:      State[2] += 1
    elif y > 0:     State[2] -= y
    elif y == -2:   State[3] = False

    return 0, tuple(State)





