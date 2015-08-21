/* hufftree.hpp
 * Written by:  Keefer Rourke
 * License:     GPLv3
 *
 * COPYRIGHT    Keefer Rourke 2015
 *
 * Description: This header file contains a set of functions required
 *              for building and operating with a binary Huffman tree
 *
 * Disclaimer:  This program is free software: you can redistribute it
 *              and/or modify it under the terms of the GNU General
 *              Public License as published by the Free Software
 *              Foundation, either version 3 of the License, or (at
 *              your option) any later version.
 *
 *              This program is distributed in the hope that it will
 *              be useful, but WITHOUT ANY WARRANTY; without even the
 *              implied warranty of MERCHANTABILITY or FITNESS FOR A
 *              PARTICULAR PURPOSE.  See the GNU General Public License
 *              for more details.
 *
 *              You should have received a copy of the GNU General
 *              Public License along with this program.  If not, see
 *              <http://www.gnu.org/licenses/>.
 */


#ifndef __HUFF_TREE_HPP__
#define __HUFF_TREE_HPP__

#include <iostream>

using namespace std;

/* node structure for the huffman tree with parent pointers */
struct node
{
    int  freq;      // frequency of occurance for each character
    char c;         // the character itself
    bool isLeaf;    // is the node a leaf?
    node * left;    // left child for non-leaf nodes
    node * right;   // right child for non-leaf nodes
};

/* prototypes for tree functions */
node * createNode(int freq, bool isLeaf);
void   printNode(node * Node);
void   printTree(node * tree);
void   destroy(node * tree);
node * mergeTree(node * tree1, node * tree2);

/* function to create a new node */
node * createNode(int freq, bool isLeaf)
{
    node * newNode      = new node;
    newNode -> freq     = freq;
    // non-leaf nodes will have a non-character value assigned
    newNode -> c = -1; /* maybe do: newNode -> c = '\0' instead */
    newNode -> isLeaf   = isLeaf;
    newNode -> left     = NULL;
    newNode -> right    = NULL;
    //cout << "created new node @ ";
    //printNode(newNode);
    return newNode;
}

/* function to print an individual node */
void printNode(node * Node)
{
    cout << "addr = " << Node;
    if (!(Node -> isLeaf))
    {
        cout << " left = "  << Node -> left;
        cout << " right = " << Node -> right;
    }
    else
        cout << " char = "  << Node -> c;
    cout << " freq = " << Node -> freq;
}

/* function to recursively print an entire tree */
void printTree(node * tree)
{
    if (tree != NULL)
    {
        printTree(tree -> left);
        printNode(tree);
        cout << endl;
        printTree(tree -> right);
    }
}

/* function to recursively destroy an entire tree */
void destroy(node * tree)
{
    if (tree != NULL)
    {
        if (tree -> left != NULL)
            destroy(tree -> left);
        if (tree -> right != NULL)
            destroy(tree -> right);
        delete tree;
    }
}

/* function to merge two trees together */
node * mergeTree(node * tree1, node * tree2)
{
    int freq1 = tree1 -> freq;
    int freq2 = tree2 -> freq;
    // find the frequency of the new tree's root node
    int sum = freq1 + freq2;    
    // create new root node for the merged tree, the root node is not a leaf
    node * mergedTree = createNode(sum, false);
    
    // determine how the subtrees are assigned as children to the root node
    if (freq1 > freq2)
    {
        mergedTree -> right = tree1;
        mergedTree -> left = tree2;
    }
    else
    {
        mergedTree -> right = tree2;
        mergedTree -> left = tree1;
    }
    
    return mergedTree;
}

#endif
