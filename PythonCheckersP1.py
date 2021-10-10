import random

EMPTY=''
INCs=[-1,1]
VALID_RANGE=range(8)

#function to return a list of all valid moves for the current player
def getValidMovesList(board,currentPlayerTokens,rowInc):
    validMovesList=[]
    for row in range(8):
        for col in range(8):
            if board[row][col] in currentPlayerTokens:
                if board[row][col] in ['r','b']: #regular checker
                    for colInc in [-1,1]:
                        if (col+colInc)>=0 and (col+colInc)<=7 and (row+rowInc)>=0 and (row+rowInc)<=7 and board[row+rowInc][col+colInc]=="":
                            validMovesList.append(chr(row+65)+str(col)+":"+chr(row+rowInc+65)+str(col+colInc))
                else: #king
                    for rInc in [1,-1]:
                        for cInc in [-1,1]:
                            if (col+cInc)>=0 and (col+cInc)<=7 and (row+rInc)>=0 and (row+rInc)<=7 and board[row+rInc][col+cInc]=="":
                                validMovesList.append(chr(row+65)+str(col)+":"+chr(row+rInc+65)+str(col+cInc))                    
    return validMovesList

#function to return a list of all valid jumps for the current player
def getValidJumpsList(board,currentPlayerTokens,rowInc,opposingPlayerTokens): #Not finished
    lst=[]
    for row in range(8):
        for col in range(8):
            if board[row][col] in currentPlayerTokens:
                if board[row][col] in ['r','b']: #regular checker
                    for colInc in [-1,1]:
                        if (col+2*colInc)>=0 and (col+2*colInc)<=7 and (row+2*rowInc)>=0 and (row+2*rowInc)<=7 and  board[row+rowInc][col+colInc] in opposingPlayerTokens and board[row+2*rowInc][col+2*colInc]=='': 
                            lst.append(chr(row+65)+str(col)+":"+chr(row+(2*rowInc)+65)+str(col+(2*colInc)))
                else: #king
                    for rInc in [1,-1]:
                        for cInc in [-1,1]:
                            if (col+2*cInc)>=0 and (col+2*cInc)<=7 and (row+2*rInc)>=0 and (row+2*rInc)<=7 and board[row+rInc][col+cInc] in opposingPlayerTokens and board[row+2*rInc][col+2*cInc]=='': 
                                lst.append(chr(row+65)+str(col)+":"+chr(row+(2*rInc)+65)+str(col+(2*cInc)))                            
    return lst

def expandJumps(oldJumps,board,rowInc,opposingPlayerTokens):
    newJumps=[]
    for jmp in oldJumps:
        initiatingRow=ord(jmp[0])-65
        initiatingCol=int(jmp[1])
        tokenType=board[initiatingRow][initiatingCol] #whole purpose of looking at first square is to get token type

        endRow=ord(jmp[-2])-65              #becomes starting row for next jump
        endCol=int(jmp[-1])                 #becomes starting columns for next jump
        
        #newJumps.append(jmp)
        expanded=False
        if tokenType in ["r","b"]:          #regular checker
            for colInc in [-1,1]:
                jumpOverRow=endRow+rowInc
                jumpOverCol=endCol+colInc
                toRow=endRow+2*rowInc
                toCol=endCol+2*colInc
                if toRow in VALID_RANGE and toCol in VALID_RANGE and board[jumpOverRow][jumpOverCol] in opposingPlayerTokens and board[toRow][toCol]==EMPTY:
                    newJumps.append(jmp+":"+chr(toRow+65)+str(toCol))
                    expanded=True
        else:#king checker
            for rInc in [1,-1]:
                for cInc in [-1,1]:
                    jumpOverRow=endRow+rInc
                    jumpOverCol=endCol+cInc
                    toRow=endRow+2*rInc
                    toCol=endCol+2*cInc
                    toCoords=chr(toRow+65)+str(toCol)
                    if toRow in VALID_RANGE and toCol in VALID_RANGE and board[jumpOverRow][jumpOverCol] in opposingPlayerTokens and \
                    (board[toRow][toCol]==EMPTY or toCoords==jmp[0:2]) and \
                    ((jmp[-2:]+":"+toCoords) not in jmp) and ((toCoords+':'+jmp[-2:] not in jmp)):
#                    ((jmp[-2:]+":"+toCoords) not in jmp) and ((toCoords+':'+jmp[-2:] not in jmp)):
                        newJumps.append(jmp+":"+toCoords)
                        expanded=True
        if not expanded:
            newJumps.append(jmp)
    return newJumps

def heuristicsMove(validMovesList,board,opposingPlayerTokens):
    pointslst=[]
    for move in validMovesList:
        count=0
        
        if opposingPlayerTokens==["b","B"]:
        
            if "A"==move[0]:      #Back row checkers do not move unless forced to by a jump or
                count=count-10     # no other move is available
            if "0"==move[4] or "7"==move[4]:    # Checkers are forced to move to side columns unless 
                count+=3                            # the current color checker has a jump available.
                
            if board[ord(move[0])-65][int(move[1])] not in ['R','B'] and "H" == move[3]:      # Checker is forced to move to a king position unless 
                count+=10           # the current color checker has a jump available.
                
                
            if "0" == move[1] or "7" == move[1]:  # if checker in side spot then checker is forced to stay there unless   
                count = count-5                   # jump available.
                
            if "B" == move[0] or "C" == move[0]:       # prioritizes moving checkers as a unit to encourage trades. 
                count+=2
            if "D" == move[0] or "E" == move[0]:
                count+=1
               
            if "1" == move[4] or "6" == move[4]:        # prioritizes moving checkers to the outside columns to encourage natural 
                count+=3                                # blocks because of the pioritization of the moves that put checkers on 
            if "2" == move[4] or "5" == move[4]:        # the outer most columns. 
                count+=2  


            if board[ord(move[0])-65][int(move[1])] in ['R','B'] and ("G"==move[3] or "F" ==move[3]):    # prioritizes moving kings back toward
                count+=4                                                                                 # its home row. 

            if board[ord(move[0])-65][int(move[1])] in ['R','B'] and ("E"==move[3] or "D" ==move[3]):   
                count+=5

            if (ord(move[3])-65+1)<=7 and (ord(move[3])-65+1)>=0 and (int(move[4])-1)>=0 and (int(move[4])-1)<=7 and (int(move[4])+1)<=7 and (int(move[4])+1)>=0: #stops checkers from walking into unnecessary jumps.
                if board[ord(move[3])-65+1][int(move[4])+1] in opposingPlayerTokens or board[ord(move[3])-65+1][int(move[4])-1] in opposingPlayerTokens:
                    count= count-7

                    

            if (ord(move[3])-65+1)<=7 and (ord(move[3])-65+1)>=0 and (int(move[4])-1)>=0 and (int(move[4])-1)<=7 and (int(move[4])+1)<=7 and (int(move[4])+1)>=0: #encourages checkers to block jumps.
                if board[ord(move[3])-65+1][int(move[4])+1] not in opposingPlayerTokens or board[ord(move[3])-65+1][int(move[4])-1] not in opposingPlayerTokens:
                    count+=3

            if "A"==move[0] and "0"==move[1]:  #encourages the corner checker in the backrow to move before other back row checkers to protect back row. 
                count+=3

            if (ord(move[0])-65+1)<=7 and (ord(move[0])-65+1)>=0 and (int(move[1])-1)>=0 and (int(move[1])-1)<=7 and (int(move[1])+1)<=7 and (int(move[1])+1)>=0: #prioritizes moving checkers that are already in danger of being jumped.
                if board[ord(move[0])-65+1][int(move[1])+1] in opposingPlayerTokens or board[ord(move[0])-65+1][int(move[1])-1] in opposingPlayerTokens:
                    count+=2
                    
        else:
            if "H" == move[0]:      #Back row checkers do not move unless forced to by a jump or
                count=count-10      # no other move is available
                
            if "0" == move[4] or "7" == move[4]:    # Checkers are forced to move to side columns unless 
                count+=3                            # the current color checker has a jump available.
                
            if board[ord(move[0])-65][int(move[1])] not in ['R','B'] and "A" == move[3]:      # Checker is forced to move to a king position unless 
                count+=10           # the current color checker has a jump available.
                
                
            if "0" == move[1] or "7" == move[1]:  # if checker in side spot then checker is forced to stay there unless   
                count = count-5                   # jump available.
                
            if "G" == move[0] or "F" == move[0]:       # prioritizes moving checkers as a unit to encourage trades. 
                count+=2
            if "D" == move[0] or "E" == move[0]:
                count+=1
               
            if "1" == move[4] or "6" == move[4]:        # prioritizes moving checkers to the outside columns to encourage natural 
                count+=3                                # blocks because of the pioritization of the moves that put checkers on 
            if "2" == move[4] or "5" == move[4]:        # the outer most columns. 
                count+=2  


            if board[ord(move[0])-65][int(move[1])] in ['R','B'] and ("B"==move[3] or "C" ==move[3]):    # prioritizes moving kings back toward
                count+=4                                                                                 # its home row. 

            if board[ord(move[0])-65][int(move[1])] in ['R','B'] and ("E"==move[3] or "D" ==move[3]):   
                count+=5

            if (ord(move[3])-65-1)<=7 and (ord(move[3])-65-1)>=0 and (int(move[4])+1)>=0 and (int(move[4])+1)<=7 and (int(move[4])-1)<=7 and (int(move[4])-1)>=0: #stops checkers from walking into unnecessary jumps.
                if board[ord(move[3])-65-1][int(move[4])-1] in opposingPlayerTokens or board[ord(move[3])-65-1][int(move[4])+1] in opposingPlayerTokens:
                    count= count-7

                    

            if (ord(move[3])-65-1)<=7 and (ord(move[3])-65-1)>=0 and (int(move[4])+1)>=0 and (int(move[4])+1)<=7 and (int(move[4])-1)<=7 and (int(move[4])-1)>=0: #encourages checkers to block jumps.
                if board[ord(move[3])-65-1][int(move[4])-1] not in opposingPlayerTokens or board[ord(move[3])-65-1][int(move[4])+1] not in opposingPlayerTokens:
                    count= count+4

            if "H"==move[0] and "7"==move[1]:  #encourages the corner checker in the backrow to move before other back row checkers to protect back row. 
                count+=3

            if (ord(move[0])-65-1)<=7 and (ord(move[0])-65-1)>=0 and (int(move[1])+1)>=0 and (int(move[1])+1)<=7 and (int(move[1])-1)<=7 and (int(move[1])-1)>=0: #prioritizes moving checkers that are already in danger of being jumped.
                if board[ord(move[0])-65-1][int(move[1])-1] in opposingPlayerTokens or board[ord(move[0])-65-1][int(move[1])+1] in opposingPlayerTokens:
                    count+=3
        
        pointslst.append(count)
    
    return validMovesList[pointslst.index(max(pointslst))]
    
    


def heuristicsJumps(newJumps,board,opposingPlayerTokens):
    pointsJumps=[]
    for move in newJumps:
        countJump=0
        if opposingPlayerTokens==["b","B"]:
            if board[ord(move[0])-65][int(move[1])] in ['R','B']:   # prioritizes kings to take jumps over regular checkers.
                countJump+=5
        else:
            if board[ord(move[0])-65][int(move[1])] in ['R','B']:   # prioritizes kings to take jumps over regular checkers.
                countJump+=5
        pointsJumps.append(countJump)
        
    return newJumps[pointsJumps.index(max(pointsJumps))]


def getValidPlayerMove(board,currentPlayerTokens,rowInc,opposingPlayerTokens,currentPlayer,auto):
    validMovesList=getValidMovesList(board,currentPlayerTokens,rowInc)
    oldJumps=getValidJumpsList(board,currentPlayerTokens,rowInc,opposingPlayerTokens)
    newJumps=expandJumps(oldJumps,board,rowInc,opposingPlayerTokens)
    while newJumps != oldJumps:
        oldJumps=newJumps[:]
        newJumps=expandJumps(oldJumps,board,rowInc,opposingPlayerTokens)
    if newJumps==[]:
        if not auto:
            print("Valid Moves",validMovesList)
            index=int(input("Player " + currentPlayer + " enter your move INDEX => "))
            while index<0 or index>=len(validMovesList):
                index=int(input("Player " + currentPlayer + " enter your move INDEX => "))
            move = validMovesList[index]
        else:
            if len(validMovesList)>0:
                move=heuristicsMove(validMovesList,board,opposingPlayerTokens)
    else:
        if not auto:
            print("Valid Jumps",newJumps)
            index=int(input("Player " + currentPlayer + " enter your jump INDEX => "))
            while index<0 or index>=len(newJumps):
                index=int(input("Player " + currentPlayer + " enter your jump INDEX => "))
            move = newJumps[index]
        else:
            move=heuristicsJumps(newJumps,board,opposingPlayerTokens)
    return move
