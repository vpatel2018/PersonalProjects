import decisionTree
import os

'''
Vihan Patel
Yuriy Deyneka
Adeena Ahmed
Billal Zughbi
'''

'''
This file contains functions that will help us
create the output file needed for task 1 of the
project.
'''

#the class, outNode, is used to create a decision tree node that will be shown in an output file
class outNode:
    def __init__(self):
        #represents the label of a decision tree node in an output file
        self.content = ''
        
        #represents unique identifier of a decision tree node in an output file
        self.number = 0
        
        #contains list of child nodes of a decision tree node in an output file
        self.childNodes = []
    #
#

#converts a dtNode object to outNode object
def getOutNodeFromDtNode(node):

    '''
    input: dtNode object
    output: outNode object
    '''
    
    if(len(node.result) > 0):
        theNode = outNode()
        theNode.content = node.result
        return theNode
    else:
        theNode = outNode()
        theNode.content = node.attr
        array = []
        for x in range(0, len(node.values)):
            tempNode = outNode()
            tempNode.content = node.values[x]
            tempNode.childNodes = [node.nextNodes[x]]
            array.append(tempNode)
        #
        theNode.childNodes = array
        return theNode
    #
    
#

#builds a decision tree for output file
def getTreeForOutputFile(node):

    '''
    input: outNode object representing the root of a decision tree that has outNode objects only
    '''
        
    if(len(node.childNodes) == 0):
       return
    else:
       for x in range(0, len(node.childNodes)):
           node.childNodes[x].childNodes[0] = getOutNodeFromDtNode(node.childNodes[x].childNodes[0])
           getTreeForOutputFile(node.childNodes[x].childNodes[0])
       #
    #
    
#

#gets a list of all the decision tree nodes that will be shown in an output file
def getNodesFromTree(node, array):

    '''
    input: node is outNode object, array is 1 dimensional list
    '''
    
    array.append(node)
    
    for x in node.childNodes:
        getNodesFromTree(x, array)
    #
    
#
