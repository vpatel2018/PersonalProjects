CSC 426 Project 3: Decision Tree Implementation, Experiments, and Analysis
Authors: Vihan Patel, Billal Zughbi, Adeena Ahmed, Yuriy Deyneka


FILES
1. descisionTree.py - the input is a csv file that has training data, and the output is a pdf file containing a decision tree 
2. outputTree.py - used by decisionTree.py to create an output file for task 1 of the project 
3. tables.py - used by decisionTree.py to analyze a table from a csv file 
4. Task2.pdf - contains decision tree for D2 
5. Task3DecTree.pdf - contains decision tree for D3 
6. AnswersToQuestionsForTask3.pdf - contains write up for D3 
7. Task4Data.csv - contains dataset needed for D4 
8. Task4DecTree.pdf - contains decision tree of dataset needed for D4 
9. D5.pdf - contains the writeup for D5 


Steps for running D1
1. Figure out the absolute path for a folder called project3. 
2. Type 'cd ?', where ? represents the absolute path for a folder called project3. Press the return key.
3. Type ‘module add python’ and press the return key.
4. Type ‘python3 decisionTree.py @ &’, where @ is a relative or absolute path to a csv file that has training data, and & = (some directory) + ‘/’ + (preferred name for output file). Suppose /home/hpc/vihan/b.csv is the path for some csv file. Suppose you want the output file to be named GREEN and store the output file in /home/hpc/yellow. Then, you would type ‘python3 decisionTree.py /home/hpc/vihan/b.csv /home/hpc/yellow/GREEN’. After pressing the return key, you will see GREEN.pdf in /home/hpc/yellow. If you want to see GREEN.pdf in your current working directory, type ‘python3 decisionTree.py /home/hpc/vihan/b.csv GREEN’.
5. If you have not done so, press the return key.