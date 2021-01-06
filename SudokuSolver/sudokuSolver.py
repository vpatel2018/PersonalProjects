import os
import numpy

'''
Vihan Patel
'''

class Solution:
    
    '''
    This function serves as a helper
    function for the function below.
    '''
    def computePossCombs(self, arr):
    
        array = []
        zerosAreThere = False

        for x in range(0, len(arr)):
            digs = []
            for y in range(0, len(arr)):
                if(y != x):
                    string = str(arr[y])
                    string = string.replace('0','')
                    digs.append(string[len(string) - 1])
                #
            #
            zeros = 0
            number = str(arr[x])
            for y in range(0, len(number)):
                if(number[y] == '0'):
                    zeros += 1
                #
            #
            zeros -= 1
            if(zeros != 0):
                zerosAreThere = True
            #
            items = []
            for d in digs:
                num = int(d) * (10 ** zeros)
                total = num + arr[x]
                items.append(total)
            #
            array.append(items)
        #
        
        arr = array
            
        if(zerosAreThere == True):
            for a in range(0, len(arr)):
                arr[a] = Solution.computePossCombs(self, arr[a])
            #
        #
        
        return arr
                
    #
    
    '''
    This function generates all unique
    permutations from a list of digits.
    If there are k digits, k! unique
    permutations will be generated.
    '''
    def getAllPossCombs(self, digits):

        power = len(digits) - 1
        arr = []

        for x in digits:
            product = (10 ** power) * x
            arr.append(product)
        #

        arr = Solution.computePossCombs(self, arr)
        arr = numpy.array(arr)
        arr = arr.flatten()
        arr = list(arr)
        
        return arr

    #
    
    '''
    This serves as a helper function for the
    function below.
    '''
    def compNumsInSameBox(self, a, b):
    
        lstA = []
        word = str(a)
        
        for x in range(0, 3):
            start = x * 3
            end = start + 3
            lstA.append(word[start:end])
        #
        
        lstB = []
        word = str(b)
        
        for x in range(0, 3):
            start = x * 3
            end = start + 3
            lstB.append(word[start:end])
        #
                
        for x in range(0, len(lstA)):
            string = lstA[x]
            for y in lstB[x]:
                if(string.find(y) != -1):
                    return False
                #
            #
        #
        
        return True
        
    #
    
    '''
    This function is used to solve a sudoku puzzle.
    '''
    def solveSudoku(self, board):
    
        digits = [1, 2, 3, 4, 5, 6, 7, 8, 9]
        arr = Solution.getAllPossCombs(self, digits)
        rowPoss = []
        
        for r in range(0, len(board)):
            elements = board[r]
            items = []
            for x in arr:
                isValid = True
                string = str(x)
                for k in range(0, len(elements)):
                    if(elements[k] != '.'):
                        if(elements[k] != string[k]):
                            isValid = False
                            break
                        #
                    #
                #
                if(isValid == True):
                    items.append(x)
                #
            #
            rowPoss.append(items)
        #
                
        for r in range(0, len(board)):
            items = []
            for x in rowPoss[r]:
                string = str(x)
                isValid = True
                for c in range(0, len(board)):
                    if(c != r):
                        for k in range(0, len(board[c])):
                            if(board[c][k] != '.'):
                                if(board[c][k] == string[k]):
                                    isValid = False
                                    break
                                #
                            #
                        #
                    #
                #
                if(isValid == True):
                    items.append(x)
                #
            #
            rowPoss[r] = items
        #
        
        for r in range(0, len(rowPoss)):
            items = []
            for c in range(0, len(rowPoss[r])):
                string = str(rowPoss[r][c])
                isValid = True
                for x in range(0, len(string)):
                    startRow = int(r / 3) * 3
                    startCol = int(x / 3) * 3
                    ints = []
                    for i in range(startRow, startRow + 3):
                        for j in range(startCol, startCol + 3):
                            if((i == r) & (x == j)):
                                pass
                            elif(board[i][j] != '.'):
                                ints.append(board[i][j])
                            #
                        #
                    #
                    for y in ints:
                        if(y == string[x]):
                            isValid = False
                            break
                        #
                    #
                #
                if(isValid == True):
                    items.append(rowPoss[r][c])
                #
            #
            rowPoss[r] = items
        #
                
        for x in range(0, 3):
            start = x * 3
            end = start + 3
            for y in range(start, end):
                items = []
                for z in range(start, end):
                    if(z != y):
                        for u in rowPoss[z]:
                            items.append(u)
                        #
                    #
                #
                items = set(items)
                items = list(items)
                newList = []
                for z in rowPoss[y]:
                    count = 0
                    for u in items:
                        truth = Solution.compNumsInSameBox(self, z, u)
                        if(truth == True):
                            count += 1
                        #
                    #
                    if(count > 0):
                        newList.append(z)
                    #
                #
                rowPoss[y] = newList
            #
            for y in range(start, end - 1):
                items = []
                for k in range(0, len(rowPoss[y])):
                    for z in range(0, len(rowPoss[y + 1])):
                        num1 = rowPoss[y][k]
                        num2 = rowPoss[y + 1][z]
                        truth = Solution.compNumsInSameBox(self, num1, num2)
                        if(truth == True):
                            items.append(num2)
                        #
                    #
                #
                items = set(items)
                items = list(items)
                rowPoss[y + 1] = items
            #
        #
        
        boxPosses = []
        
        for x in range(0, 3):
            start = x * 3
            items = []
            for a in rowPoss[start]:
                for b in rowPoss[start + 1]:
                    for c in rowPoss[start + 2]:
                        t1 = Solution.compNumsInSameBox(self, a, b)
                        t2 = Solution.compNumsInSameBox(self, a, c)
                        t3 = Solution.compNumsInSameBox(self, b, c)
                        if((t1 == True) & (t2 == True) & (t3 == True)):
                            lst = [a, b, c]
                            items.append(lst)
                        #
                    #
                #
            #
            boxPosses.append(items)
        #
        
        partialSolutions = []
        
        for x in boxPosses[0]:
            for y in boxPosses[1]:
                items = []
                canAdd = True
                for k in x:
                    items.append(str(k))
                #
                for k in y:
                    items.append(str(k))
                #
                for k in range(0, 9):
                    isValid = True
                    ints = []
                    for z in items:
                        ints.append(z[k])
                    #
                    for z in range(0, len(ints)):
                        for f in range(z + 1, len(ints)):
                            if(ints[f] == ints[z]):
                                isValid = False
                                break
                            #
                        #
                    #
                    if(isValid == False):
                        canAdd = False
                        break
                    #
                #
                if(canAdd == True):
                    partialSolutions.append(items)
                #
            #
        #
        
        fullSolutions = []
        
        for x in partialSolutions:
            for y in boxPosses[2]:
                items = []
                canAdd = True
                for k in x:
                    items.append(str(k))
                #
                for k in y:
                    items.append(str(k))
                #
                for k in range(0, 9):
                    isValid = True
                    ints = []
                    for z in items:
                        ints.append(z[k])
                    #
                    for z in range(0, len(ints)):
                        for f in range(z + 1, len(ints)):
                            if(ints[f] == ints[z]):
                                isValid = False
                                break
                            #
                        #
                    #
                    if(isValid == False):
                        canAdd = False
                        break
                    #
                #
                if(canAdd == True):
                    fullSolutions.append(items)
                #
            #
        #
        
        solution = fullSolutions[0]
        
        for x in range(0, len(solution)):
            for y in range(0, len(solution[x])):
                board[x][y] = solution[x][y]
            #
        #
                    
    #
    
#

file = input('Enter a file that contains a sudoku puzzle you want to solve: ')
f = open(file, "r")
f = f.readlines()
for r in range(0, len(f)):
    f[r] = f[r].replace('\n','')
    f[r] = f[r].split()
#

print('')
print('INPUT')

for r in f:
    for c in r:
        print(c + " ", end='')
    #
    print('')
#

print('')

obj = Solution()
obj.solveSudoku(f)

print('SOLUTION')

for r in f:
    for c in r:
        print(c + " ", end='')
    #
    print('')
#

print('')
