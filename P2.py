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
                move=validMovesList[random.randrange(0,len(validMovesList))]
    else:
        if not auto:
            print("Valid Jumps",newJumps)
            index=int(input("Player " + currentPlayer + " enter your jump INDEX => "))
            while index<0 or index>=len(newJumps):
                index=int(input("Player " + currentPlayer + " enter your jump INDEX => "))
            move = newJumps[index]
        else:
            move=newJumps[random.randrange(0,len(newJumps))]
    return move
