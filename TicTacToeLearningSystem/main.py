import os
import matplotlib.pyplot as plt

'''
Vihan Patel
'''

#ask the user how many training games he/she would want the player to go through
trainingGames = input('How many training games would you like to play? ')
trainingGames = int(trainingGames)
os.system('rm player*.txt opponentWeights.txt 2> /dev/null')
os.system('rm 1stPlot.png 2ndPlot.png 3rdPlot.png 2> /dev/null')

#make the player go through a certain number of training games as specified by the user
os.system('python3 game.py')

for x in range(0, trainingGames - 1):
    os.system('python3 game.py')
#

#open a file that contains the outcomes that the player had for every training game
file = open('playerResults.txt', 'r')
array = file.readlines()
indices = []

for x in range(0, len(array)):
    array[x] = array[x].replace('\n','')
#

percentOfWins = []
percentOfLosses = []
percentOfTies = []
    
for x in range(0, len(array) + 1):
    lst = array[0: x]
    ties = 0
    for y in lst:
        if(y == 'TIE'):
            ties += 1
        #
    #
    percent = 0
    if(len(lst) > 0):
        percent = ties / (1.0 * len(lst)) * 100
    #
    percentOfTies.append(percent)
#

for x in range(0, len(array) + 1):
    lst = array[0: x]
    losses = 0
    for y in lst:
        if(y == 'LOSS'):
            losses += 1
        #
    #
    percent = 0
    if(len(lst) > 0):
        percent = losses / (1.0 * len(lst)) * 100
    #
    percentOfLosses.append(percent)
#

for x in range(0, len(array) + 1):
    lst = array[0: x]
    wins = 0
    for y in lst:
        if(y == 'WIN'):
            wins += 1
        #
    #
    percent = 0
    if(len(lst) > 0):
        percent = wins / (1.0 * len(lst)) * 100
    #
    percentOfWins.append(percent)
#

for x in range(0, len(percentOfWins)):
    indices.append(x)
#

#1st plot - y-axis = % of games won & x-axis = # of training games
#1st plot will be saved as png file
plt.plot(indices, percentOfWins)
plt.xlabel('# of training games')
plt.ylabel('% of games won')
plt.savefig('1stPlot.png')
plt.clf()

#2nd plot - y-axis = % of games tied & x-axis = # of training games
#2nd plot will be saved as png file
plt.plot(indices, percentOfTies)
plt.xlabel('# of training games')
plt.ylabel('% of games tied')
plt.savefig('2ndPlot.png')
plt.clf()

#3rd plot - y-axis = % of games lost & x-axis = # of training games
#3rd plot will be saved as png file
plt.plot(indices, percentOfLosses)
plt.xlabel('# of training games')
plt.ylabel('% of games lost')
plt.savefig('3rdPlot.png')
