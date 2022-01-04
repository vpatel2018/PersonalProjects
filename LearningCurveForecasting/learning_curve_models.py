from sklearn.metrics import mean_absolute_error
import scipy.optimize
import numpy as np
import warnings
import random
import math
import sys

# AUTHOR: VIHAN PATEL
# THIS FILE PROVIDES FUNCTIONS NEEDED TO FORECAST PERFORMANCE OF SYSTEMS INCLUDING TEXT CLASSIFICATION SYSTEMS (ex. Support Vector Machines)
# YOU ONLY NEED TO USE THE LAST TWO FUNCTIONS WRITTEN IN THIS FILE

def lc_model1(x, a, b, c):
    return 1 - (a * b**(c * x))
#

def lc_model2(x, a, b):
    return 1 - (a * b**(x))
#

def lc_model3(x, a, b):
    return 1 - (a**(b * x))
#

def lc_model4(x, a):
    return 1 - (a**(x))
#

def lc_model5(x, m, a, b, c):
    return m - (a * b**(c * x))
#

def lc_model6(x, m, a, b):
    return m - (a * b**(x))
#

def lc_model7(x, m, a, b):
    return m - (a**(b * x))
#

def lc_model8(x, m, a):
    return m - (a**(x))
#

# data point is noisy if > 50 % of points ahead of it are below it
# noisy data points must be ignored to make forecasting easier
def data_is_noise(pairs, index):
    
    numPtsBelow = 0
    numPtsAboveSame = 0
    
    for x in range(index + 1, len(pairs)):
        if(pairs[x][1] < pairs[index][1]):
           numPtsBelow += 1
        else:
           numPtsAboveSame += 1   
        #
    #
    
    return numPtsBelow > numPtsAboveSame
    
#

def get_important_points(xVals, yVals):
    
    pairs = [[xVals[x], yVals[x]] for x in range(0, len(xVals))]
    lastIndex = len(pairs) - 1
    newXVals = []
    newYVals = []
    
    for p in range(0, lastIndex):
        isNoisy = data_is_noise(pairs, p)
        if(isNoisy == False):
           newXVals.append(pairs[p][0])
           newYVals.append(pairs[p][1])
        #
    #
    
    newXVals.append(pairs[lastIndex][0])
    newYVals.append(pairs[lastIndex][1])
    
    meaningfulPairs = [[0,-1]]
    
    for x in range(0, len(newYVals)):
        if(newYVals[x] > meaningfulPairs[len(meaningfulPairs) - 1][1]):
           array = [newXVals[x], newYVals[x]]
           meaningfulPairs.append(array)
        #
    #
    
    meaningfulPairs.pop(0)
    
    xs = []
    ys = []
    
    for x in range(0, len(meaningfulPairs)):
        valOfX = meaningfulPairs[x][0]
        valOfY = meaningfulPairs[x][1]
        xs.append(valOfX)
        ys.append(valOfY)
    #
        
    return [xs, ys]

#

def get_working_equations(xVals, yVals, initValue, guessForM):

    if(initValue != -1):
       xVals.insert(0, 0)
       yVals.insert(0, initValue)
    #

    warnings.filterwarnings("ignore") #
    
    goodPairs = get_important_points(xVals, yVals) #
    xValues = goodPairs[0] #
    yValues = goodPairs[1] #

    model = [] #
    functions = [lc_model1, lc_model2, lc_model3, lc_model4, lc_model5, lc_model6, lc_model7, lc_model8] #
    names = ['lc_model1', 'lc_model2', 'lc_model3', 'lc_model4', 'lc_model5', 'lc_model6', 'lc_model7', 'lc_model8'] #
    guesses = [[1, 0.00001, 1], [1, 0.00001], [0.00001, 1], [0.00001], [guessForM, 1, 0.00001, 1], [guessForM, 1, 0.00001], [guessForM, 0.00001, 1], [guessForM, 0.00001]] #
    working_equations = [] #

    for x in range(0, len(functions)): #
        try: #
          model = scipy.optimize.curve_fit(functions[x], xValues, yValues, p0=guesses[x])[0] #
          model = list(model) #
        except: #
          model = [] #
        #
        if(len(model) > 0): #
           if((x >= 4) & (model[0] > 1)): #
              model[0] = 1 #
           #
           working_equations.append([model, names[x]]) #
        #
    #
    
    if(initValue != -1):
       xVals.pop(0)
       yVals.pop(0)
    #
        
    return working_equations #
    
#

def get_result(functionNumber, value, parameters):

    expr = 0

    if(functionNumber == 1):
    
      '''
        def lc_model1(x, a, b, c):
            return 1 - (a * b**(c * x))
        #
      '''
      
      a = parameters[0]
      b = parameters[1]
      c = parameters[2]
      expr = 1 - (a * b**(c * value))
      
    elif(functionNumber == 2):
    
      '''
      def lc_model2(x, a, b):
          return 1 - (a * b**(x))
      #
      '''
      
      a = parameters[0]
      b = parameters[1]
      expr = 1 - (a * b**(value))
      
    elif(functionNumber == 3):
    
      '''
      def lc_model3(x, a, b):
          return 1 - (a**(b * x))
      #
      '''
      
      a = parameters[0]
      b = parameters[1]
      expr = 1 - (a**(b * value))
    
    elif(functionNumber == 4):
    
      '''
      def lc_model4(x, a):
         return 1 - (a**(x))
      #
      '''
      
      a = parameters[0]
      expr = 1 - (a**(value))

    elif(functionNumber == 5):
    
      '''
      def lc_model5(x, m, a, b, c):
        return m - (a * b**(c * x))
      #
      '''
      
      m = parameters[0]
      a = parameters[1]
      b = parameters[2]
      c = parameters[3]
      expr = m - (a * b**(c * value))
    
    elif(functionNumber == 6):
    
      '''
      def lc_model6(x, m, a, b):
          return m - (a * b**(x))
      #
      '''
      
      m = parameters[0]
      a = parameters[1]
      b = parameters[2]
      expr = m - (a * b**(value))
    
    elif(functionNumber == 7):
    
      '''
      def lc_model7(x, m, a, b):
          return m - (a**(b * x))
      #
      '''
      
      m = parameters[0]
      a = parameters[1]
      b = parameters[2]
      expr = m - (a**(b * value))
    
    elif(functionNumber == 8):
    
      '''
      def lc_model8(x, m, a):
        return m - (a**(x))
      #
      '''
    
      m = parameters[0]
      a = parameters[1]
      expr = m - (a**(value))
    
    #
    
    return expr


#

# xValues = list of values in interval (0,1)
# yValues = list of values in interval [0,1]
def get_best_equation(xValues, yValues):

    working_equations = []
    errors = []
    
    ##################################################################
    
    # YOU CAN MODIFY iterationsForInitValue AND iterationsForGuessOfM IF NEEDED
    
    iterationsForInitValue = 20
    iterationsForGuessOfM = 10
    
    for x in range(-1, iterationsForInitValue):
        initValue = x / (1.0 * iterationsForInitValue)
        if(initValue < 0):
           initValue = -1   
        #
        for h in range(0, iterationsForGuessOfM):
            guessForM = h / (1.0 * iterationsForGuessOfM)
            setOfEq = get_working_equations(xValues, yValues, initValue, guessForM)
            for y in setOfEq:
                working_equations.append(y)
            #
        #
    #
    
    ##################################################################
        
    for eq in working_equations:
        equationNumber = int(eq[1][len(eq[1]) - 1])
        parameters = eq[0]
        actualYs = []
        predictedYs = []
        for x in range(0, len(xValues)):
            predictedY = get_result(equationNumber, xValues[x], parameters)
            actualY = yValues[x]
            predictedYs.append(predictedY)
            actualYs.append(actualY)
        #
        error = mean_absolute_error(predictedYs, actualYs)
        errors.append(error)
    #
    
    index = -1
    minError = math.inf
    
    for x in range(0, len(errors)):
        if(errors[x] < minError):
           index = x
           minError = errors[x]
        #
    #
    
    best_equation = working_equations[index]
        
    return best_equation
        
#

# inputVal = some number in interval (0,1)
# best_equation = output of get_best_equation(xValues, yValues)
def evaluate(inputVal, best_equation):

    functionNumber = int(best_equation[1][len(best_equation[1]) - 1])
    parameters = best_equation[0]
    result = get_result(functionNumber, inputVal, parameters)
    
    return result

#
