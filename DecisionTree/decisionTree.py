import os
import tables
import sys

'''
Vihan Patel
Yuriy Deyneka
Adeena Ahmed
Billal Zughbi
'''

#the class, dtNode, will help in creating a node for a decision tree
class dtNode:
    def __init__(self):
        #represents a name of a column in a csv file that has training data
        #represents an attribute
        self.attr = ''
        
        #contains possible values for self.attr
        self.values = []
        
        #contains a list of child nodes that may or may not be a leaf node
        self.nextNodes = []
        
        #for a node that is not a leaf node, self.result is an empty string
        #for a leaf node, self.result is a value predicted by a decision tree
        self.result = ''
    #
#

#this is used to build a decision tree from a root node
def createDecisionTree(node):

    '''
    input: a dtNode object
    '''
    
    if((node.attr == '') & (len(node.values) == 0) & (len(node.nextNodes) == 0)):
        return
    #
    
    for x in range(0, len(node.nextNodes)):
        node.nextNodes[x] = getNodeFromTable(node.nextNodes[x])
        createDecisionTree(node.nextNodes[x])
    #
    
#

#this function uses a table to create a decision tree node
def getNodeFromTable(table):

    '''
    input: a two dimensional list where each element represents a row of a table
    output: a dtNode object
    '''
    
    node = dtNode()
    subTables = tables.getSubTables(table)
    
    if(len(subTables) == 0):
        node.result = tables.mostFrequentValue(table)
    else:
        node.attr = tables.getBestAttr(table)
        copyOfSubTables = []
        values = []

        for x in subTables:
            val = x[0]
            x.pop(0)
            copyOfSubTables.append(x)
            values.append(val)
        #
        
        node.values = values
        node.nextNodes = copyOfSubTables
        
    #
    
    return node
    
#

#creates decision tree using training data stored in a csv file
def getDecisionTree(inputDataFileLocation):

    '''
    input: a string representing location of csv file with training data
    output: a dtNode object that represents the root of a decision tree
    '''
    
    table = tables.getTableFromCSVFile(inputDataFileLocation)
    root = getNodeFromTable(table)
    createDecisionTree(root)
    
    return root
    
#

#creates an output file that is a pdf file
def createOutputFile(root, outputFileLocation):

    '''
    input: root represents the root of a decision tree with dtNode objects,
    outputFileLocation tells where the output should be stored
    '''
        
    outNode = outputTree.getOutNodeFromDtNode(root)
    outputTree.getTreeForOutputFile(outNode)
    array = []
    outputTree.getNodesFromTree(outNode, array)
    for x in range(0, len(array)):
        array[x].number = x + 1
    #
    obj = Graph()
    for x in array:
        obj.node(str(x.number), x.content)
    #
    for x in array:
        num = x.number
        listOfNums = []
        for y in x.childNodes:
            listOfNums.append(y.number)
        #
        for y in listOfNums:
            obj.edge(str(num), str(y))
        #
    #
    
    obj.render(outputFileLocation)
    os.remove(outputFileLocation)

#

if __name__ == "__main__":

    #location of csv file with training data
    inputDataFileLocation = sys.argv[1]
    
    #location of file with output
    outputFileLocation = sys.argv[2]
    
    #represents root node of decision tree for a csv file
    root = getDecisionTree(inputDataFileLocation)
    
    #check if graphviz library is installed or not
    os.system('python3 -c "import graphviz" 2> out.txt')
    
    file = open("out.txt", "r")
    lines = file.readlines()
    os.system('rm out.txt')
    
    #install graphviz library if it is not installed yet
    if(len(lines) > 0):
        os.system('curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py > /dev/null 2>&1')
        os.system('python get-pip.py > /dev/null 2>&1')
        os.system('pip install graphviz > /dev/null 2>&1')
        os.system('rm get-pip.py')
    #
    
    #import Graph class from graphviz library
    #the Graph class is useful since a tree is an acyclic graph
    from graphviz import Graph
    
    #import functions from outputTree.py
    import outputTree
    
    #creates an output file required for task 1 of the project
    #output file is a pdf file
    outputTreeRoot = outputTree.getOutNodeFromDtNode(root)
    createOutputFile(root, outputFileLocation)

#
