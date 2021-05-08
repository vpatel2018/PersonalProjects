import csv
import math
from statistics import mode

'''
Vihan Patel
Yuriy Deyneka
Adeena Ahmed
Billal Zughbi
'''

#this function returns all the rows of a csv file
def getTableFromCSVFile(csvFileName):

    '''
    input: a string representing the absolute or relative path to csv file
    output: a two dimensional list where each element represents a row of a csv file
    '''
    
    object = open(csvFileName)
    array = []
    
    for x in object:
        x = x.replace('\n','')
        x = x.split(',')
        array.append(x)
    #
    
    return array
    
#

#this function returns all the columns that are present in a table
def getColsFromTable(table):

    '''
    input: a two dimensional list where each element represents a row of a table
    output: a list in which the 1st element is a list of column names and the 2nd element is a list of columns present in the table
    '''
    
    colNames = table[0]
    cols = []
    
    for h in range(0, len(colNames)):
        items = []
        for x in range(1, len(table)):
            items.append(table[x][h])
        #
        cols.append(items)
    #
    
    return [colNames, cols]
    
#

#chooses attribute from a table that will result in the highest information gain
def getBestAttr(table):

    '''
    input: a two dimensional list where each element represents a row of a table
    output: a string representing the best attribute
    '''
    
    info = getColsFromTable(table)
    cols = info[1]
    results = cols[len(cols) - 1]
    maxInfoGain = 0
    attribute = ''
    
    for x in range(0, len(cols) - 1):
        attrCol = cols[x]
        infoGain = calcInfoGain(attrCol, results)
        if(infoGain > maxInfoGain):
            maxInfoGain = infoGain
            attribute = info[0][x]
        #
    #
    
    return attribute
    
#

#returns the most frequent value for the very last column of a table
def mostFrequentValue(table):

    '''
    input: a two dimensional list where each element represents a row of a table
    output: a string representing the most frequent value for the very last column of a table
    '''

    array = []
    
    for x in range(1, len(table)):
        result = table[x][len(table[x]) - 1]
        array.append(result)
    #
    
    return mode(array)
    
#


'''

    input for this function -> a table that is represented by a two dimensional list
    --> each element of the two dimensional list represents a row of a table

    Analyzing the output of getSubTables function ->

    The output of the getSubTables function will be a 3D list. Each element of the output is a 2D list.
    Suppose you do not get an empty list as output. Then ...
    
    for each 2D list of the output:
        - the 1st element of the 2D list will be a possible value for the best attribute of the input for the getSubTables function
        - the 2nd to last element of the 2D list represents rows of a subtable that would be created using
        the best attribute of the input for the getSubTables function and the first element of the 2D list
    #

'''

def getSubTables(table):

    #if the first element of the input is not a list, delete it
    if(isinstance(table[0], list) == False):
        table.pop(0)
    #

    '''
    check if input has only one column
    if it has only one column, return an empty array
    '''
    
    singleColTable = True
    
    for x in table:
        if(len(x) > 1):
            singleColTable = False
            break
        #
    #
    
    if(singleColTable == True):
        return []
    #
    
    '''
    check if input has either all positive examples or all negative examples
    if input has either all positive examples or all negative examples, then
    return an empty array
    '''
    
    info = getColsFromTable(table)
    cols = info[1]
    results = cols[len(cols) - 1]
    keys = list(set(results))
    
    if(len(keys) == 1):
        return []
    #
    
    '''
    for the input, find the attribute that leads to the highest information gain
    '''
    
    attr = getBestAttr(table)
    
    #represents (column # associated with best attribute) - 1
    #Let's say column 3 of the input is the column for the best attribute. Then index = 2.
    index = -1
    
    for x in range(0, len(table[0])):
        if(table[0][x] == attr):
            index = x
            break
        #
    #
    
    #represents the possible values for the best attribute
    values = []
    
    for x in range(1, len(table)):
        values.append(table[x][index])
    #
    
    values = list(set(values))
    
    '''
    
    for each possible value of the best attribute:
        - create a subtable using the best attribute and the possible value
        - remove column no. (index + 1) of the subtable
        - append subtable to an array of subtables
    #
    
    ----------------------------------------------------------------------------------------------------------------------------
    
    Example:
    
    Sample table with attributes = {A, B, C}
    
        A | B | C | D
        -------------
        0 | 0 | 0 | 0
        0 | 0 | 0 | 1
        0 | 0 | 1 | 0
        0 | 0 | 1 | 1
        0 | 1 | 0 | 0
        
    ------------------------------------
        
    Suppose the best attribute is C and the value we are looking at is 0. Then we create the subtable below.
    
        A | B | C | D
        -------------
        0 | 0 | 0 | 0
        0 | 0 | 0 | 1
        0 | 1 | 0 | 0
    
    ------------------------------------
    
    For the subtable above, we need to get rid of the 3rd column since it is the column for the best attribute C.
    This will make building a decision tree easier. The final version of the subtable is shown below.
    
        A | B | D
        ---------
        0 | 0 | 0
        0 | 0 | 1
        0 | 1 | 0
    
    -----------------------------------------------------------------------------------------------------------------------------
            
    '''
    
    subTables = []
    
    for v in values:
        items = []
        words = []
        items.append(v)
        for y in range(0, len(table[0])):
            if(y != index):
                words.append(table[0][y])
            #
        #
        items.append(words)
        for x in range(1, len(table)):
            if(table[x][index] == v):
                words = []
                for y in range(0, len(table[x])):
                    if(y != index):
                        words.append(table[x][y])
                    #
                #
                items.append(words)
            #
        #
        subTables.append(items)
    #
    
    return subTables
    
#

#calculates information gain using an attribute column and results column
#results column is the very last column of a table
def calcInfoGain(attrCol, resultsCol):

    '''
    input: attrCol is a 1 dimensional list representing an attribute column, resultsCol is a 1 dimensional list representing a results column
    output: a float
    '''
    
    #calculate entropy of results column
    
    keys1 = list(set(resultsCol))
    numbers = []
    
    for x in keys1:
        amt = resultsCol.count(x)
        numbers.append(amt)
    #
    
    total = sum(numbers)
    entOfResultsCol = 0
    
    for x in numbers:
        if(x != 0):
            decimal = x / (total * 1.0)
            product = -1 * decimal * math.log2(decimal)
            entOfResultsCol = entOfResultsCol + product
        #
    #
    
    ######################################################
    
    #calculate entropy of attribute column
    
    keys2 = list(set(attrCol))
    entOfAttrCol = 0
    
    for k in keys2:
        array = []
        for x in range(0, len(attrCol)):
            if(attrCol[x] == k):
                array.append(resultsCol[x])
            #
        #
        uniqueKeys = list(set(array))
        counts = []
        for u in uniqueKeys:
            amount = array.count(u)
            counts.append(amount)
        #
        ent = 0
        for x in counts:
            if(x != 0):
                decimal = x / (sum(counts) * 1.0)
                product = -1 * decimal * math.log2(decimal)
                ent = ent + product
            #
        #
        numToAdd = ent * sum(counts)
        entOfAttrCol = entOfAttrCol + numToAdd
    #
    
    entOfAttrCol = entOfAttrCol / (1.0 * len(resultsCol))
    
    ######################################################
    
    #return (entropy of results column) - (entropy of attribute column)
    
    return entOfResultsCol - entOfAttrCol
        
#
