'''
Vihan Patel
'''

#this function determines whether a player lost, won or scored a tie against an opponent
#it analyzes a tic tac toe board
def playerStanding(playerPiece, lst):

    playerStatus = getStatus(playerPiece, lst)
    
    if(playerPiece == 'O'):
       opponentPiece = 'X' 
    elif(playerPiece == 'X'):
       opponentPiece = 'O'
    #
    
    opponentStatus = getStatus(opponentPiece, lst)
    
    if(playerStatus == 'TIE'):
        return 'TIE'
    elif(playerStatus == 'WON'):
        return 'WON'
    elif(opponentStatus == 'WON'):
        return 'LOST'
    #
    
    return ''

#

#this function is a helper function for the function above
def getStatus(playersPiece, lst):
    
    status = ''

    diag1 = [lst[0], lst[4], lst[8]]
    diag2 = [lst[2], lst[4], lst[6]]
    row1 = [lst[0], lst[1], lst[2]]
    row2 = [lst[3], lst[4], lst[5]]
    row3 = [lst[6], lst[7], lst[8]]
    col1 = [lst[0], lst[3], lst[6]]
    col2 = [lst[1], lst[4], lst[7]]
    col3 = [lst[2], lst[5], lst[8]]
        
    #################################
    
    amount = 0

    for x in diag1:
        if(x == playersPiece):
            amount += 1
        #
    #
    
    if(amount == 3):
        status = 'WON'
        return status
    #
        
    #################################
    
    amount = 0

    for x in diag2:
        if(x == playersPiece):
            amount += 1
        #
    #
    
    if(amount == 3):
        status = 'WON'
        return status
    #
        
    #################################
    
    amount = 0

    for x in row1:
        if(x == playersPiece):
            amount += 1
        #
    #
    
    if(amount == 3):
        status = 'WON'
        return status
    #
        
    #################################
    
    amount = 0

    for x in row2:
        if(x == playersPiece):
            amount += 1
        #
    #
    
    if(amount == 3):
        status = 'WON'
        return status
    #
       
    #################################
    
    amount = 0

    for x in row3:
        if(x == playersPiece):
            amount += 1
        #
    #
    
    if(amount == 3):
        status = 'WON'
        return status
    #
        
    #################################
    
    amount = 0

    for x in col1:
        if(x == playersPiece):
            amount += 1
        #
    #
    
    if(amount == 3):
        status = 'WON'
        return status
    #
    
    #################################
    
    amount = 0

    for x in col2:
        if(x == playersPiece):
            amount += 1
        #
    #
    
    if(amount == 3):
        status = 'WON'
        return status
    #
    
    #################################
    
    amount = 0

    for x in col3:
        if(x == playersPiece):
            amount += 1
        #
    #
    
    if(amount == 3):
        status = 'WON'
        return status
    #
    
    #################################
    
    amount = 0
    
    for x in lst:
         if(x != ''):
             amount += 1
         #
    #
    
    if(amount == 9):
        status = 'TIE'
    #

    return status
    
    #################################
    
#
