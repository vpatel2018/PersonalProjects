#include <iostream>
#include <list>
#include <iterator>
#include <cmath>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;

/* This file contains functions for my self balancing tree project.
   You need to only use the del and insert functions. */

struct TreeNode{
    int integer;
    int times;
    struct TreeNode * parent;
    struct TreeNode * left;
    struct TreeNode * right;
    int lf;
    int rf;
};

void updateHeight(struct TreeNode ** node) {
    
    if((*(node)) == nullptr) {
        return;
    }
    
    if((*(node))->right == nullptr) {
        (*(node))->rf = 0;
    }else{
        if((*(node))->right->rf <= (*(node))->right->lf) {
            (*(node))->rf = (*(node))->right->lf + 1;
        }else if((*(node))->right->rf >= (*(node))->right->lf) {
            (*(node))->rf = (*(node))->right->rf + 1;
        }
    }
    
    if((*(node))->left == nullptr) {
        (*(node))->lf = 0;
    }else{
        if((*(node))->left->rf <= (*(node))->left->lf) {
            (*(node))->lf = (*(node))->left->lf + 1;
        }else if((*(node))->left->rf >= (*(node))->left->lf) {
            (*(node))->lf = (*(node))->left->rf + 1;
        }
    }
    
}

void doSimpleChange(struct TreeNode * node, struct TreeNode ** pt1, struct TreeNode ** pt2) {
    
    if(node->lf == 2 && node->rf == 0) {
        
        node->right = new TreeNode;
        node->right->parent = node;
        node->right->lf = 0;
        node->right->rf = 0;
        node->lf = 1;
        node->rf = 1;
        
        if(node->left->left != nullptr) {
            node->right->integer = node->integer;
            node->right->times = node->times;
            node->integer = node->left->integer;
            node->times = node->left->times;
            node->left->integer = node->left->left->integer;
            node->left->times = node->left->left->times;
            node->left->left->parent = nullptr;
            delete node->left->left;
            node->left->left = nullptr;
        }else if(node->left->right != nullptr) {
            node->right->integer = node->integer;
            node->right->times = node->times;
            node->integer = node->left->right->integer;
            node->times = node->left->right->times;
            node->left->right->parent = nullptr;
            delete node->left->right;
            node->left->right = nullptr;
        }
        
        node->left->lf = 0;
        node->left->rf = 0;
        
        struct TreeNode * copy = node;
        
        copy = copy->parent;
        
        while(copy != nullptr) {
            updateHeight(&copy);
            copy = copy->parent;
        }
        
        return;
        
    }else if(node->rf == 2 && node->lf == 0) {
        
        node->left = new TreeNode;
        node->left->parent = node;
        node->left->lf = 0;
        node->left->rf = 0;
        
        node->lf = 1;
        node->rf = 1;
        
        if(node->right->right != nullptr) {
            node->left->integer = node->integer;
            node->left->times = node->times;
            node->integer = node->right->integer;
            node->times = node->right->times;
            node->right->integer = node->right->right->integer;
            node->right->times = node->right->right->times;
            node->right->right->parent = nullptr;
            delete node->right->right;
            node->right->right = nullptr;
        }else if(node->right->left != nullptr) {
            node->left->integer = node->integer;
            node->left->times = node->times;
            node->integer = node->right->left->integer;
            node->times = node->right->left->times;
            node->right->left->parent = nullptr;
            delete node->right->left;
            node->right->left = nullptr;
        }
        
        node->right->lf = 0;
        node->right->rf = 0;
        
        struct TreeNode * copy = node;
        
        copy = copy->parent;
        
        while(copy != nullptr) {
            updateHeight(&copy);
            copy = copy->parent;
        }
        
        return;
        
    }else if(node->rf - node->lf == 2) {
        
        int nodeVal = node->integer;
        int nodeTimes = node->times;
        struct TreeNode * start = node->right;
        
        while(start->left != nullptr) {
            start = start->left;
        }
        
        int minVal = start->integer;
        int minTimes = start->times;
        
        if(start->right != nullptr) {
            struct TreeNode * parentNode = start->parent;
            struct TreeNode * rightNode = start->right;
            start->parent = nullptr;
            rightNode->parent = nullptr;
            start->right = nullptr;
            delete start;
            parentNode->left = rightNode;
            rightNode->parent = parentNode;
            parentNode->lf -= 1;
            start = parentNode;
        }else{
            start = start->parent;
            start->left->parent = nullptr;
            delete start->left;
            start->left = nullptr;
            start->lf -= 1;
        }
        
        (*(pt2)) = start;
        node->integer = minVal;
        node->times = minTimes;
        
        while(start != nullptr) {
            updateHeight(&start);
            start = start->parent;
        }
        
        struct TreeNode * copy = node;
        
        while(node != nullptr) {
            if(node->integer < nodeVal) {
                if(node->right != nullptr) {
                    node = node->right;
                }else{
                    node->right = new TreeNode;
                    node->right->integer = nodeVal;
                    node->right->times = nodeTimes;
                    node->right->lf = 0;
                    node->right->rf = 0;
                    node->right->right = nullptr;
                    node->right->left = nullptr;
                    node->right->parent = node;
                    node->rf += 1;
                    (*(pt1)) = node->right;
                    break;
                }
            }else if(node->integer > nodeVal) {
                if(node->left != nullptr) {
                    node = node->left;
                }else{
                    node->left = new TreeNode;
                    node->left->integer = nodeVal;
                    node->left->times = nodeTimes;
                    node->left->lf = 0;
                    node->left->rf = 0;
                    node->left->right = nullptr;
                    node->left->left = nullptr;
                    node->left->parent = node;
                    node->lf += 1;
                    (*(pt1)) = node->left;
                    break;
                }
            }
        }
        
        start = (*(pt1));
        
        while(start != nullptr) {
            updateHeight(&start);
            start = start->parent;
        }
        
        node = copy;
        
    }else if(node->lf - node->rf == 2) {
        
        int nodeVal = node->integer;
        int nodeTimes = node->times;
        struct TreeNode * start = node->left;
        
        while(start->right != nullptr) {
            start = start->right;
        }
        
        int maxVal = start->integer;
        int maxTimes = start->times;
        
        if(start->left != nullptr) {
            struct TreeNode * parentNode = start->parent;
            struct TreeNode * leftNode = start->left;
            start->parent = nullptr;
            leftNode->parent = nullptr;
            start->left = nullptr;
            delete start;
            parentNode->right = leftNode;
            leftNode->parent = parentNode;
            parentNode->rf -= 1;
            start = parentNode;
        }else{
            start = start->parent;
            start->right->parent = nullptr;
            delete start->right;
            start->right = nullptr;
            start->rf -= 1;
        }
        
        (*(pt1)) = start;
        node->integer = maxVal;
        node->times = maxTimes;
        
        while(start != nullptr) {
            updateHeight(&start);
            start = start->parent;
        }
        
        struct TreeNode * copy = node;
        
        while(node != nullptr) {
            if(node->integer < nodeVal) {
                if(node->right != nullptr) {
                    node = node->right;
                }else{
                    node->right = new TreeNode;
                    node->right->integer = nodeVal;
                    node->right->times = nodeTimes;
                    node->right->lf = 0;
                    node->right->rf = 0;
                    node->right->right = nullptr;
                    node->right->left = nullptr;
                    node->right->parent = node;
                    node->rf += 1;
                    (*(pt2)) = node->right;
                    break;
                }
            }else if(node->integer > nodeVal) {
                if(node->left != nullptr) {
                    node = node->left;
                }else{
                    node->left = new TreeNode;
                    node->left->integer = nodeVal;
                    node->left->times = nodeTimes;
                    node->left->lf = 0;
                    node->left->rf = 0;
                    node->left->right = nullptr;
                    node->left->left = nullptr;
                    node->left->parent = node;
                    node->lf += 1;
                    (*(pt2)) = node->left;
                    break;
                }
            }
        }
        
        start = (*(pt2));
        
        while(start != nullptr) {
            updateHeight(&start);
            start = start->parent;
        }
        
        node = copy;
        
    }
    
}


bool doComplexChange(struct TreeNode * node, struct TreeNode ** pt1, struct TreeNode ** pt2) {
    
    if(node->lf == 2 && node->rf == 0) {
        
        if(node->left->right != nullptr && node->left->left != nullptr) {
            
            struct TreeNode * s = new TreeNode;
            s->rf = 0;
            s->lf = 0;
            s->integer = node->integer;
            s->times = node->times;
            s->right = nullptr;
            s->left = nullptr;
            s->parent = node;
            node->right = s;
            node->rf += 1;
            
            int integer = node->left->right->integer;
            int times = node->left->right->times;
            
            node->left->right->parent = nullptr;
            delete node->left->right;
            node->left->right = nullptr;
            node->left->rf -= 1;
            
            node->integer = integer;
            node->times = times;
            
            return true;
            
        }else{
            
            return false;
            
        }
        
    }else if(node->lf == 0 && node->rf == 2) {
        
        if(node->right->right != nullptr && node->right->left != nullptr) {
            
            struct TreeNode * s = new TreeNode;
            s->rf = 0;
            s->lf = 0;
            s->integer = node->integer;
            s->times = node->times;
            s->right = nullptr;
            s->left = nullptr;
            s->parent = node;
            node->left = s;
            node->lf += 1;
            
            int integer = node->right->left->integer;
            int times = node->right->left->times;
            
            node->right->left->parent = nullptr;
            delete node->right->left;
            node->right->left = nullptr;
            node->right->lf -= 1;
            
            node->integer = integer;
            node->times = times;
            
            return true;
            
        }else{
            
            return false;
            
        }
        
    }else if(node->lf - node->rf == 2) {
        
        struct TreeNode * maxNode = node->left;
        
        while(maxNode->right != nullptr) {
            maxNode = maxNode->right;
        }
        
        maxNode = maxNode->parent;
        int conditions = 0;
        
        if(maxNode->right != nullptr) {
            if(maxNode->right->left == nullptr && maxNode->right->right == nullptr) {
                conditions++;
            }
        }
        
        if(maxNode->left != nullptr) {
            if(maxNode->left->left != nullptr) {
                if(maxNode->left->left->left == nullptr && maxNode->left->left->right == nullptr) {
                    conditions++;
                }
            }
            if(maxNode->left->right != nullptr) {
                if(maxNode->left->right->left == nullptr && maxNode->left->right->right == nullptr) {
                    conditions++;
                }
            }
        }
        
        if(conditions == 3) {
            
            int integer = node->integer;
            int times = node->times;
            
            node->integer = maxNode->right->integer;
            node->times = maxNode->right->times;
            
            maxNode->right->parent = nullptr;
            delete maxNode->right;
            maxNode->right = nullptr;
            maxNode->rf -= 1;
            
            struct TreeNode * k1;
            struct TreeNode * k2;
            
            doComplexChange(maxNode, &k1, &k2);
            
            (*(pt1)) = maxNode;
            
            struct TreeNode * u = node;
            
            while(u != nullptr) {
                if(u->integer < integer) {
                    if(u->right != nullptr) {
                        u = u->right;
                    }else{
                        u->right = new TreeNode;
                        u->right->integer = integer;
                        u->right->times = times;
                        u->right->lf = 0;
                        u->right->rf = 0;
                        u->right->right = nullptr;
                        u->right->left = nullptr;
                        u->right->parent = u;
                        break;
                    }
                }else if(u->integer > integer) {
                    if(u->left != nullptr) {
                        u = u->left;
                    }else{
                        u->left = new TreeNode;
                        u->left->integer = integer;
                        u->left->times = times;
                        u->left->lf = 0;
                        u->left->rf = 0;
                        u->left->right = nullptr;
                        u->left->left = nullptr;
                        u->left->parent = u;
                        break;
                    }
                }
            }
            
            struct TreeNode * y = u;
            
            while(y != nullptr) {
                updateHeight(&y);
                y = y->parent;
            }
            
            (*(pt2)) = u;
            
            return true;
            
        }else{
            
            return false;
            
        }
        
    }else if(node->rf - node->lf == 2) {
        
        struct TreeNode * minNode = node->right;
        
        while(minNode->left != nullptr) {
            minNode = minNode->left;
        }
        
        minNode = minNode->parent;
        int conditions = 0;
        
        if(minNode->left != nullptr) {
            if(minNode->left->left == nullptr && minNode->left->right == nullptr) {
                conditions++;
            }
        }
        
        if(minNode->right != nullptr) {
            if(minNode->right->left != nullptr) {
                if(minNode->right->left->left == nullptr && minNode->right->left->right == nullptr) {
                    conditions++;
                }
            }
            if(minNode->right->right != nullptr) {
                if(minNode->right->right->left == nullptr && minNode->right->right->right == nullptr) {
                    conditions++;
                }
            }
        }
        
        if(conditions == 3) {
            
            int integer = node->integer;
            int times = node->times;
            
            node->integer = minNode->left->integer;
            node->times = minNode->left->times;
            
            minNode->left->parent = nullptr;
            delete minNode->left;
            minNode->left = nullptr;
            minNode->lf -= 1;
            
            struct TreeNode * k1;
            struct TreeNode * k2;
            
            doComplexChange(minNode, &k1, &k2);
            
            (*(pt1)) = minNode;
            
            struct TreeNode * u = node;
            
            while(u != nullptr) {
                if(u->integer < integer) {
                    if(u->right != nullptr) {
                        u = u->right;
                    }else{
                        u->right = new TreeNode;
                        u->right->integer = integer;
                        u->right->times = times;
                        u->right->lf = 0;
                        u->right->rf = 0;
                        u->right->right = nullptr;
                        u->right->left = nullptr;
                        u->right->parent = u;
                        break;
                    }
                }else if(u->integer > integer) {
                    if(u->left != nullptr) {
                        u = u->left;
                    }else{
                        u->left = new TreeNode;
                        u->left->integer = integer;
                        u->left->times = times;
                        u->left->lf = 0;
                        u->left->rf = 0;
                        u->left->right = nullptr;
                        u->left->left = nullptr;
                        u->left->parent = u;
                        break;
                    }
                }
            }
            
            struct TreeNode * y = u;
            
            while(y != nullptr) {
                updateHeight(&y);
                y = y->parent;
            }
            
            (*(pt2)) = u;
            
            return true;
            
        }else{
            
            return false;
            
        }
        
    }
    
    return false;
    
}

void rebalance(struct TreeNode * node) {
    
    while(abs(node->lf - node->rf) == 2) {
        
        struct TreeNode * point1 = nullptr;
        struct TreeNode * point2 = nullptr;
        
        bool q = doComplexChange(node, &point1, &point2);
        
        if(q == false) {
            doSimpleChange(node, &point1, &point2);
        }
        
        if(point1 != nullptr) {
            while(point1 != node) {
                updateHeight(&point1);
                rebalance(point1);
                point1 = point1->parent;
            }
        }
        
        if(point2 != nullptr) {
            while(point2 != node) {
                updateHeight(&point2);
                rebalance(point2);
                point2 = point2->parent;
            }
        }
        
    }
    
}

//insert a value in a tree
//to call this function, use insert(&(pointer to root of tree), value to insert)
void insert(struct TreeNode ** root, int value) {
    
    if((*(root)) == nullptr) {
        (*(root)) = new TreeNode;
        (*(root))->integer = value;
        (*(root))->times = 1;
        (*(root))->lf = 0;
        (*(root))->rf = 0;
        (*(root))->right = nullptr;
        (*(root))->left = nullptr;
        (*(root))->parent = nullptr;
        return;
    }
    
    struct TreeNode * node = (*(root));
    
    while(node != nullptr) {
        
        if(node->integer < value) {
            if(node->right != nullptr) {
                node = node->right;
            }else{
                node->right = new TreeNode;
                node->right->integer = value;
                node->right->times = 1;
                node->right->lf = 0;
                node->right->rf = 0;
                node->right->right = nullptr;
                node->right->left = nullptr;
                node->right->parent = node;
                node = node->right;
                break;
            }
        }else if(node->integer > value) {
            if(node->left != nullptr) {
                node = node->left;
            }else{
                node->left = new TreeNode;
                node->left->integer = value;
                node->left->times = 1;
                node->left->lf = 0;
                node->left->rf = 0;
                node->left->right = nullptr;
                node->left->left = nullptr;
                node->left->parent = node;
                node = node->left;
                break;
            }
        }else if(node->integer == value) {
            node->times += 1;
            return;
        }
        
    }
    
    struct TreeNode * insNode = node;
    
    while(insNode != nullptr) {
        updateHeight(&insNode);
        rebalance(insNode);
        insNode = insNode->parent;
    }
    
    return;
    
}

//delete a value from tree
//to call this function, use del(&(pointer to root of tree), value to delete)
void del(struct TreeNode ** root, int value) {
    
    struct TreeNode * node = (*(root));
    struct TreeNode * parent = nullptr;
    
    while(node != nullptr) {
        if(node->integer < value) {
            node = node->right;
        }else if(node->integer > value) {
            node = node->left;
        }else if(node->integer == value) {
            break;
        }
    }
    
    if(node == nullptr) {
        return;
    }
    
    if(node->right == nullptr && node->left == nullptr) {
        
        if(node->parent == nullptr) {
            delete node;
            *(root) = nullptr;
            return;
        }
        
        parent = node->parent;
        
        if(node->parent->right == node) {
            node->parent = nullptr;
            delete node;
            struct TreeNode ** p = &node;
            *(p) = nullptr;
            parent->right = nullptr;
            parent->rf -= 1;
        }else if(node->parent->left == node) {
            node->parent = nullptr;
            delete node;
            struct TreeNode ** p = &node;
            *(p) = nullptr;
            parent->left = nullptr;
            parent->lf -= 1;
        }
        
        struct TreeNode * n = parent;
        
        while(n != nullptr) {
            updateHeight(&n);
            rebalance(n);
            n = n->parent;
        }
        
    }else{
        
        string side = "";
        
        srand(time(0));
        
        if(node->rf > node->lf) {
          side = "right";
        }else if(node->rf < node->lf) {
          side = "left";
        }else if(node->rf == node->lf) {
          int c = rand() % 2;
          if(c == 0) {
            side = "left";
          }else if(c == 1) {
            side = "right";
          }
        }
        
        if(side == "left") {
          
            struct TreeNode * maxNode = node->left;
            
            while(maxNode->right != nullptr) {
                maxNode = maxNode->right;
            }
            
            node->integer = maxNode->integer;
            node->times = maxNode->times;
            
            if(maxNode->right == nullptr && maxNode->left == nullptr) {
                if(maxNode->parent->left == maxNode) {
                    maxNode = maxNode->parent;
                    maxNode->left->parent = nullptr;
                    delete maxNode->left;
                    maxNode->left = nullptr;
                    maxNode->lf -= 1;
                }else if(maxNode->parent->right == maxNode) {
                    maxNode = maxNode->parent;
                    maxNode->right->parent = nullptr;
                    delete maxNode->right;
                    maxNode->right = nullptr;
                    maxNode->rf -= 1;
                }
                parent = maxNode;
            }else if(maxNode->left != nullptr && maxNode->right == nullptr) {
                maxNode->integer = maxNode->left->integer;
                maxNode->times = maxNode->left->times;
                maxNode->left->parent = nullptr;
                delete maxNode->left;
                maxNode->left = nullptr;
                maxNode->lf -= 1;
                parent = maxNode;
            }
            
        }else if(side == "right") {
          
            struct TreeNode * minNode = node->right;
            
            while(minNode->left != nullptr) {
                minNode = minNode->left;
            }
            
            node->integer = minNode->integer;
            node->times = minNode->times;
            
            if(minNode->right == nullptr && minNode->left == nullptr) {
                if(minNode->parent->left == minNode) {
                    minNode = minNode->parent;
                    minNode->left->parent = nullptr;
                    delete minNode->left;
                    minNode->left = nullptr;
                    minNode->lf -= 1;
                }else if(minNode->parent->right == minNode) {
                    minNode = minNode->parent;
                    minNode->right->parent = nullptr;
                    delete minNode->right;
                    minNode->right = nullptr;
                    minNode->rf -= 1;
                }
                parent = minNode;
            }else if(minNode->right != nullptr && minNode->left == nullptr) {
                minNode->integer = minNode->right->integer;
                minNode->times = minNode->right->times;
                minNode->right->parent = nullptr;
                delete minNode->right;
                minNode->right = nullptr;
                minNode->rf -= 1;
                parent = minNode;
            }
           
        }
        
        struct TreeNode * n = parent;
        
        while(n != nullptr) {
          updateHeight(&n);
          rebalance(n);
          n = n->parent;
        }
        
    }
    
}
