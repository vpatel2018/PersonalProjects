import PlayerStatus
import os.path
import sys

'''
Vihan Patel
'''

#this function assigns weights to player before he/she plays tic tac toe with opponent
def assignInitWeightsToPlayer():
        
    value = os.path.exists('playerWeights.txt')
    
    if(value == False):
        arr = []
        for x in range(0, 8):
            arr.append(0)
        #
        return arr
    #
    
    file = open('playerWeights.txt', 'r')
    array = file.readlines()
    
    for x in range(0, len(array)):
        array[x] = array[x].replace('\n','')
        array[x] = float(array[x])
    #
    
    return array
    
#

#a file is used to store information about the player's weights after the player loses, wins or scores a tie in tic tac toe against the opponent
def writePlayerWeightsToFile(playerWeights):

    for x in range(0, len(playerWeights)):
        playerWeights[x] = str(playerWeights[x]) + '\n'
    #
    
    file = open('playerWeights.txt', 'w')
    file.writelines(playerWeights)

#

#this function is used to assign a piece for the player to use when playing tic tac toe
#the player can choose to use an O or X
def getPlayerPiece():
    
    value = os.path.exists('playerPiece.txt')
    
    if(value == False):
        string = 'Do you want be O or X? '
        inputStr = input(string)
        b1 = inputStr == 'x'
        b2 = inputStr == 'X'
        b3 = inputStr == 'o'
        b4 = inputStr == 'O'
        result = b1 | b2 | b3 | b4
        if(result == True):
            file = open('playerPiece.txt', 'w')
            if(inputStr == 'o'):
                inputStr = 'O'
            elif(inputStr == 'x'):
                inputStr = 'X'
            #
            word = inputStr + '\n'
            file.writelines(word)
            return inputStr
        else:
            return ''
        #
    #
    
    file = open('playerPiece.txt', 'r')
    array = file.readlines()
    piece = array[0].replace('\n','')
    
    return piece
    
#

#a file will be used to track the performance of a player in every tic tac toe game he/she plays
def writeResult(status):
    
    value = os.path.exists('playerResults.txt')
    
    if(value == False):
        file = open('playerResults.txt', 'w')
        status = status + '\n'
        array = [status]
        file.writelines(array)
        return
    #
    
    file = open('playerResults.txt', 'r')
    array = file.readlines()
    status = status + '\n'
    array.append(status)
    
    file = open('playerResults.txt', 'w')
    file.writelines(array)
    
#

#this function is used to extract an array representing board features
def getBoardFeatures(board):

    #this represents 1st row of board
    row1 = [board[0], board[1], board[2]]
    
    #this represents 2nd row of board
    row2 = [board[3], board[4], board[5]]
    
    #this represents 3rd row of board
    row3 = [board[6], board[7], board[8]]
    
    #this represents 1st column of board
    col1 = [board[0], board[3], board[6]]
    
    #this represents 2nd column of board
    col2 = [board[1], board[4], board[7]]
    
    #this represents 3rd column of board
    col3 = [board[2], board[5], board[8]]
    
    #diag1 and diag2 represent the diagonals of tic tac toe board
    diag1 = [board[0], board[4], board[8]]
    diag2 = [board[2], board[4], board[6]]
    
    #places is a list of all lists above
    places = [row1, row2, row3, col1, col2, col3, diag1, diag2]
    
    #this will be used to store board features
    features = []
    
    '''
    Board Features
    1. x_1 to x_8 represent either a 0 or 1
    2. 1 represents True
    3. 0 represents False
    
    x_1 = (row1 has both empty spots and O's ONLY) || (row1 has both empty spots and X's ONLY) || (row1 has empty spots ONLY)
    x_2 = (row2 has both empty spots and O's ONLY) || (row2 has both empty spots and X's ONLY) || (row2 has empty spots ONLY)
    x_3 = (row3 has both empty spots and O's ONLY) || (row3 has both empty spots and X's ONLY) || (row3 has empty spots ONLY)
    x_4 = (col1 has both empty spots and O's ONLY) || (col1 has both empty spots and X's ONLY) || (col1 has empty spots ONLY)
    x_5 = (col2 has both empty spots and O's ONLY) || (col2 has both empty spots and X's ONLY) || (col2 has empty spots ONLY)
    x_6 = (col3 has both empty spots and O's ONLY) || (col3 has both empty spots and X's ONLY) || (col3 has empty spots ONLY)
    x_7 = (diag1 has both empty spots and O's ONLY) || (diag1 has both empty spots and X's ONLY) || (diag1 has empty spots ONLY)
    x_8 = (diag2 has both empty spots and O's ONLY) || (diag2 has both empty spots and X's ONLY) || (diag2 has empty spots ONLY)
    '''
    
    for x in range(0, len(places)):
        count = 0
        for y in places[x]:
            if(y == ''):
                count += 1
            #
        #
        if(count == 0):
            places[x] = []
        #
    #
    
    for x in range(0, len(places)):
        if(len(places[x]) > 0):
            numOfX = 0
            numOfO = 0
            for k in range(0, len(places[x])):
                if(places[x][k] == 'O'):
                    numOfO += 1
                elif(places[x][k] == 'X'):
                    numOfX += 1
                #
            #
            if((numOfX == 0) | (numOfO == 0)):
                features.append(1)
            else:
                features.append(0)
            #
        else:
            features.append(0)
        #
    #
        
    return features

#

#used to evaluate a linear function
def calcPredictedVal(weights, board):

    features = getBoardFeatures(board)
    sum = 0
    
    for x in range(0, len(weights)):
        product = weights[x] * features[x]
        sum += product
    #
    
    return sum

#

#this function is used to update weights for player after an opponent makes a moves
#LMS update algorithm is used
def updateWeights(actualValue, predictedValue, boardFeatures, playerWeights):

    for k in range(0, len(playerWeights)):
        product = learningRate * (actualValue - predictedValue) * boardFeatures[k]
        playerWeights[k] = playerWeights[k] + product
    #
        
    return playerWeights
    
#

#given a board and weights for player, this function will help an opponent decide where to put his/her piece on tic tac toe board
def opponentMove(opponentWeights, playerWeights, board, learningRate, opponentPiece):
    
    #this represents 1st row of board
    row1 = [board[0], board[1], board[2]]
    
    #this represents 2nd row of board
    row2 = [board[3], board[4], board[5]]
    
    #this represents 3rd row of board
    row3 = [board[6], board[7], board[8]]
    
    #this represents 1st column of board
    col1 = [board[0], board[3], board[6]]
    
    #this represents 2nd column of board
    col2 = [board[1], board[4], board[7]]
    
    #this represents 3rd column of board
    col3 = [board[2], board[5], board[8]]
    
    #diag1 and diag2 represent the diagonals of tic tac toe board
    diag1 = [board[0], board[4], board[8]]
    diag2 = [board[2], board[4], board[6]]
    
    #places is a list of all lists above
    places = [row1, row2, row3, col1, col2, col3, diag1, diag2]
    
    #represents the index the opponent needs to use when referencing the 'places' list
    #if value is a decimal, then we need to refer to places[int(value)] and places[int(value) + 1]
    value = calcPredictedVal(opponentWeights, board)
    number = int(value)
    
    if((number < 0) | (number > 7)):
        return [playerWeights, board]
    #
    
    '''
    Suppose for an index i, place[i] = col2. This means the opponent
    has to put a piece on the first empty spot in the 2nd column of the
    tic tac toe board.
    '''
    
    boardFeatures = getBoardFeatures(board)
    index = -1
    
    if(boardFeatures[number] == 1):
        area = places[number]
        for x in range(0, len(area)):
            if(area[x] == ''):
                index = x
                break
            #
        #
        if(index == -1):
            return [playerWeights, board]
        #
    elif((number + 1) <= 7):
        number = number + 1
        if(boardFeatures[number] == 1):
            area = places[number]
            for x in range(0, len(area)):
                if(area[x] == ''):
                    index = x
                    break
                #
            #
            if(index == -1):
                return [playerWeights, board]
            #
        #
    #
    
    #the three lines of code below are used to update weights for player
    predictedValue = calcPredictedVal(playerWeights, board)
    boardFeatures = getBoardFeatures(board)
    playerWeights = updateWeights(number, predictedValue, boardFeatures, playerWeights)
    
    #the if-else statements below are used to decide which place in the tic tac toe board the opponent needs to put his piece
    if((number >= 0) & (number <= 2)):
       boardIndex = (number * 3) + index
       board[boardIndex] = opponentPiece
    elif((number >= 3) & (number <= 5)):
       boardIndex = (number - 3) + (3 * index)
       board[boardIndex] = opponentPiece
    elif(number == 6):
       boardIndex = index * 4
       board[boardIndex] = opponentPiece
    elif(number == 7):
       boardIndex = 2 * (index + 1)
       board[boardIndex] = opponentPiece
    #
    
    return [playerWeights, board]
    
#

#given a board and weights for player, this function will help a player decide where to put his/her piece on tic tac toe board
def playerMove(playerWeights, board, playerPiece):

    #this represents 1st row of board
    row1 = [board[0], board[1], board[2]]
    
    #this represents 2nd row of board
    row2 = [board[3], board[4], board[5]]
    
    #this represents 3rd row of board
    row3 = [board[6], board[7], board[8]]
    
    #this represents 1st column of board
    col1 = [board[0], board[3], board[6]]
    
    #this represents 2nd column of board
    col2 = [board[1], board[4], board[7]]
    
    #this represents 3rd column of board
    col3 = [board[2], board[5], board[8]]
    
    #diag1 and diag2 represent the diagonals of tic tac toe board
    diag1 = [board[0], board[4], board[8]]
    diag2 = [board[2], board[4], board[6]]
    
    #places is a list of all lists above
    places = [row1, row2, row3, col1, col2, col3, diag1, diag2]

    #represents the index the player needs to use when referencing the 'places' list
    value = calcPredictedVal(playerWeights, board)
    number = int(value)
    
    if((number < 0) | (number > 7)):
        return board
    #
    
    '''
    Suppose for an index i, place[i] = row2. This means the player
    has to put a piece on the first empty spot in the 2nd row of the
    tic tac toe board.
    '''
    
    boardFeatures = getBoardFeatures(board)
    index = -1
    
    if(boardFeatures[number] == 1):
        area = places[number]
        for x in range(0, len(area)):
            if(area[x] == ''):
                index = x
                break
            #
        #
        if(index == -1):
            return [playerWeights, board]
        #
    elif((number + 1) <= 7):
        number = number + 1
        if(boardFeatures[number] == 1):
            area = places[number]
            for x in range(0, len(area)):
                if(area[x] == ''):
                    index = x
                    break
                #
            #
            if(index == -1):
                return [playerWeights, board]
            #
        #
    #
    
    #the if-else statements below are used to decide which place in the tic tac toe board the player needs to put his piece
    if((number >= 0) & (number <= 2)):
       boardIndex = (number * 3) + index
       board[boardIndex] = playerPiece
    elif((number >= 3) & (number <= 5)):
       boardIndex = (number - 3) + (3 * index)
       board[boardIndex] = playerPiece
    elif(number == 6):
       boardIndex = index * 4
       board[boardIndex] = playerPiece
    elif(number == 7):
       boardIndex = 2 * (index + 1)
       board[boardIndex] = playerPiece
    #
    
    return board
    
#

if __name__ == "__main__":

    #this represents a learning rate
    learningRate = 0.1
    
    #represents the piece that a player is using when playing tic tac toe
    #the piece that a player uses can be O or X
    playerPiece = getPlayerPiece()
    
    if(playerPiece == ''):
        sys.exit(0)
    #
        
    #represents the piece that an opponent uses when playing tic tac toe
    opponentPiece = ''
    
    if(playerPiece == 'O'):
      opponentPiece = 'X'
    elif(playerPiece == 'X'):
      opponentPiece = 'O'
    #
    
    #represents fixed weights for a fixed evaluation function created by hand
    opponentWeights = [-0.03302126, -0.01739626, 0.01385374, 0.07635374, 0.20135374, 0.45135374, 0.95135374, 1.95135374]
    
    #represents weights for the learned function V^
    playerWeights = assignInitWeightsToPlayer()
    
    #this list represents a tic tac toe board
    board = ['','','','','','','','','']
         
    #if (no. of moves made by player + no. of moves made by opponent) equals 20000, then the tic tac toe game is halted
    moves = 20000
    
    for x in range(0, moves):
    
        #when opponent makes a move, the player's weights and tic tac toe board will be updated
        a = opponentMove(opponentWeights, playerWeights, board, learningRate, opponentPiece)
        playerWeights = a[0]
        board = a[1]
        
        #check if opponent has won, lost or scored a tie in a tic tac toe game
        status = PlayerStatus.playerStanding(opponentPiece, board)
        
        #if opponent has won or scored a tie, end the tic tac toe game
        if(status == 'TIE'):
          writePlayerWeightsToFile(playerWeights)
          writeResult(status)
          sys.exit(0)
        elif(status == 'WON'):
          writePlayerWeightsToFile(playerWeights)
          status = 'LOSS'
          writeResult(status)
          sys.exit(0)
        #
            
        #update tic tac toe board when player makes a move
        board = playerMove(playerWeights, board, playerPiece)
        
        #check if player has won, lost or scored a tie in a tic tac toe game
        status = PlayerStatus.playerStanding(playerPiece, board)

        #if player has won or scored a tie, end the tic tac toe game
        if(status == 'TIE'):
          writePlayerWeightsToFile(playerWeights)
          writeResult(status)
          sys.exit(0)
        elif(status == 'WON'):
          writePlayerWeightsToFile(playerWeights)
          status = 'WIN'
          writeResult(status)
          sys.exit(0)
        #
        
    #
    
    #If a player does not win, lose or score a tie in a tic tac toe game, then we can say a player's performance in tic tac toe game is unknown
    status = 'UNKNOWN'
    writeResult(status)
    
#
