/* huff.hpp
 * Written by:  Keefer Rourke
 * License:     GPLv3
 *
 * COPYRIGHT    Keefer Rourke 2015
 *
 * Description: This header file contains a set of functions required
 *              for the deflation of a plain-text file into a Huffman
 *              binary file
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
//NOTES: OUTPUT FILE IS LARGER THAN INPUT FILE HELP 
 
#ifndef __HUFF_HPP__
#define __HUFF_HPP__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <climits>
#include <bitset>
#include <cmath>
#include <cctype>

using namespace std;
typedef unsigned int uint;

/* structure used to build character-frequency database */
struct cfreq
{
    char c;
    int freq;
};

/* structure used to hold the huffman codes per character */
struct huffcode
{
    char c;
    string code;
};

/* function protoypes */
string getContents(string infilename);
vector <cfreq> getCFreqs(string &contents);
bool   compareByFreq(const cfreq &a, const cfreq &b);
vector <node *> makeForest(vector <cfreq> cfreqs);
node * createHuffTree(vector <node *> forest);
node * findSmallest(vector <node *> forest, int &pos);
void   printForest(vector <node *> forest);
void   genHuffCodes(node * huffTree, string code, vector<huffcode> &codes);
string encodeText(string &contents, vector <huffcode> codes);
void   flatten(node * huffTree, string &flatTree);
void   writeToFile(string outfilename, string flatTree, string encodedText);
vector <uint> splitBinary(string binary);
uint    toDecimal(string binstr);

/* this function will build a huffman tree which can be used to create the
 * output binary file; the name of the ouput binary file can optionally be
 * provided, but will default to out.bin if no output filename is provided
 */
void huffCompress(string infilename, string outfilename = "out.bin")
{
    // dump the file contents into a string
    string infileContents = getContents(infilename);
    // if empty string returned due to error opening file, cut this function short
    if (infileContents == "")
        return;

    // build a sorted vector of characters and their frequencies
    vector <cfreq> cfreqs = getCFreqs(infileContents);
    sort(cfreqs.begin(), cfreqs.end(), compareByFreq);
    /* use character-frequency database to build a vector of single node
     * trees, or a forest */
    vector <node *> forest = makeForest(cfreqs);
    // merge the trees in the forest to make a single Huffman tree
    node * huffTree = createHuffTree(forest);
    
    //print huffTree
    //printTree(huffTree);
    
    // generate the prefix code for each character
    string code = "";
    vector <huffcode> codes;
    genHuffCodes(huffTree, code, codes);
    cout << endl;
    for (int i = 0; i < (int)codes.size(); i++)
        cout << codes[i].c << ": " << codes[i].code << endl;
    
    /* iterate through the file contents and piece together a new string
     * containing the prefix codes for each individual character as it occurs */
    string encodedText = encodeText(infileContents, codes);

    // flatten huffman tree
    string flatTree = "";
    flatten(huffTree, flatTree);
    
    // write to the binary file
    writeToFile(outfilename, flatTree, encodedText);
}

/* this function will read the contents of an input file and dump them into
 * a single string with new line characters */
string getContents(string infilename)
{
    // open the file
    ifstream infile;
    infile.open(infilename.c_str());
    // if file opening fails, print error and return empty string
    if (!infile.is_open())
    {
        cerr << "Error. Could not open file '" << infilename << "'." << endl;
        return "";
    }

    /* grab the file contents line by line, placing newline chars at the end
     * of each line */
    string str;
    string contents;
    while (getline(infile, str))
    {
        contents += str;
        contents.push_back('\n');
    }
    /* push back the eof character which will be used to indicate the end of
     * the data stream when rebuilding and reinflating the file */
    contents.push_back('\0');
    //if file contents are bad, print error and return empty string
    if (infile.bad())
    {
        cerr << "Error while reading file '" << infilename << "'." << endl;
        infile.close();
        return "";
    }
    
    // close the file
    infile.close();
    
    // if file read successfully, return the its contents + an added EOF char
    return contents;
}

/* this function will read a string containing a file's contents and count
 * the frequency of each unique character, returning a vector of structs
 * that contain each unique character and it's corresponding frequency */
vector <cfreq> getCFreqs (string &contents)
{
    vector <cfreq> cfreqs;
    /* iterate over the contents of the file character by character
     * comparing it to existing database */
    for (int i = 0; i < (int)contents.size(); i++)
    {
        bool inDB = false;
        for (int j = 0; j < (int)cfreqs.size(); j++)
        {
            /* if the character in the file is in the database then
             * increase the frequency count of the character */
            if (contents[i] == (cfreqs[j].c))
            {
                cfreqs[j].freq++;
                inDB = true;
                break;
            }
        }
        /* if the character in the file is not in the database then
         * create new cfreq to store the character and its frequency
         * value of 1 in the database */
        if (!inDB)
        {
            cfreq temp;
            temp.c = contents[i];
            temp.freq = 1;
            cfreqs.push_back(temp);
        }
    }
    
    // return the database of character frequencies
    return cfreqs;
}

/* this function serves as a reference for comparing two cfreq structures
 * by std::algorithm::sort, which will sort by greatest frequency to
 * least */
bool compareByFreq(const cfreq &a, const cfreq &b)
{
    return a.freq > b.freq;
}

/* this function will use the data found in a character-frequency database
 * to create a forest of single-node trees in the form of a doubly-linked
 * list */
vector <node *> makeForest(vector <cfreq> cfreqs)
{
    // vector to store a forest of single-node trees
    vector <node *> forest;
    // create and append single-node trees to the forest
    for (int i = 0; i < (int)cfreqs.size(); i++)
    {
        node * newTree = createNode(cfreqs[i].freq, true);
        newTree -> c   = cfreqs[i].c;
        forest.push_back(newTree);
    }
        
    // return the list of new single-node trees
    return forest;
}

/* this function will merge all the trees in a forest two-by-two until there
 * is a single Huffman tree remaining, from which prefix codes for the characters
 * in the file contents can be generated */
node * createHuffTree(vector <node *> forest)
{
    node * huffTree = new node;
    
    while (forest.size() != 0)
    {
        // trees to be merged together
        node * tree1 = new node;
        node * tree2 = new node;
        node * mergedTree = new node;

        // tracking variables
        int smallTreePos = 0;

        // find smallest tree in the forest
        node * smallestTree = findSmallest(forest, smallTreePos);
        
        //printForest(forest);
       
        // assign the smallest tree's values to tree1
        tree1 -> freq   = smallestTree -> freq;
        tree1 -> c      = smallestTree -> c;
        tree1 -> isLeaf = smallestTree -> isLeaf;
        tree1 -> left   = smallestTree -> left;
        tree1 -> right  = smallestTree -> right;
        //cout << "populated tree1:" << tree1 << " -> freq:" << tree1 -> freq << " -> c:" << tree1 -> c << endl;
        //cout << "tree1 comes from:" << smallestTree << endl;

        // delete the smallest tree from forest and start over with tree2
        //cout << "deleting:" << smallestTree << endl;
        forest.erase(forest.begin() + smallTreePos);

        // reset relevant variables
        smallTreePos = 0;
        
        // find second smallest tree in forest
        smallestTree = findSmallest(forest, smallTreePos);

        //printForest(forest);

        // assign this smallest tree to tree2
        tree2 -> freq   = smallestTree -> freq;
        tree2 -> c      = smallestTree -> c;
        tree2 -> isLeaf = smallestTree -> isLeaf;
        tree2 -> left   = smallestTree -> left;
        tree2 -> right  = smallestTree -> right;
        //cout << "populated tree2:" << tree2 << " -> freq:" << tree2 -> freq << " -> c:" << tree2 -> c << endl;
        //cout << "tree2 comes from:" << smallestTree << endl;

        // delete the smallest tree from the forest
        //cout << "deleting:" << smallestTree << endl;
        forest.erase(forest.begin() + smallTreePos);

        //printForest(forest);
                
        // merge tree1 and tree2
        mergedTree = mergeTree(tree1, tree2);
        //cout << "merged tree1 and tree2" << endl;
        //cout << "mergedtree:" << mergedTree << " -> freq:" << mergedTree -> freq << endl;
        // add merged tree back into the forest
        forest.push_back(mergedTree);
        //cout << "added mergedTree back into the forest" << endl;

        //printForest(forest);
         
        // repeat until only one tree left in the forest
        if (forest.size() == 1)
        {
            huffTree = forest[0];
            break;
        }
    }

    // return newly created huffman tree
    return huffTree;
}

/* this function will return the smallest tree in a forest and its position in vector */
node * findSmallest (vector <node *> forest, int &pos)
{
    node * smallest;
    int compareFreq = INT_MAX;

    for (int i = 0; i < (int)forest.size(); i++)
    {
        if ((forest[i] -> freq) < compareFreq)
        {
            compareFreq = forest[i] -> freq;
            smallest = forest[i];
            pos = i;
        }
    }

    //cout << "smallest tree in forest is: " << smallest << " -> freq:" << smallest -> freq << " -> c:" << smallest -> c << endl;
    return smallest;
}

/* this is a function mainly for debugging purposes that will
 * print a vector of nodes (or a forest) in a relatively readable format */
void printForest(vector <node *> forest)
{
    // find size of forest
    cout << (int)forest.size() << endl;

    // print forest
    for (int i = 0; i < forest.size(); i++)
    {
        cout << "addr = " << forest[i];
        if (!(forest[i] -> isLeaf))
        {
            cout << " left = "  << forest[i] -> left;
            cout << " right = " << forest[i] -> right;
        }
        else
            cout << " char = "  << forest[i] -> c;
        cout << " freq = " << forest[i] -> freq;
        cout << "   <-->   ";
        
    }
    cout << "\n" << endl;
}

/* this function will recurse through the tree to generate variable
 * length prefix codes for each character in the tree */
void genHuffCodes(node * huffTree, string code, vector<huffcode> &codes)
{
    // append a 0 to the code if we go left in the tree
    if (huffTree -> left)
    {
        code.push_back('0');
        genHuffCodes(huffTree -> left, code, codes);
        code.erase(code.begin()+code.length()-1);
    }
    // append a 1 to the code if we go left in the tree
    if (huffTree -> right)
    {
        code.push_back('1');
        genHuffCodes(huffTree -> right, code, codes);
        code.erase(code.begin()+code.length()-1);
    }
    /* if a leaf node is encountered, assign the code and the encountered
     * character to a huffcode structure, and add it to the vector */
    if (huffTree -> isLeaf)
    {
        huffcode ccode;
        ccode.c = huffTree -> c;
        ccode.code = code;
        codes.push_back(ccode);
    }
}

/* this function will iterate through the plain-text contents and translate it using
 * the huffman codes that were generated previously */
string encodeText(string &contents, vector <huffcode> codes)
{
    string encodedText = "";
    for (int i = 0; i < (int)contents.length(); i++)
    {
        for (int j = 0; j < (int)codes.size(); j++)
        {
            /* if current character matches a character in the database
             * add the code associated with the character to the end of the 
             * encoded text string */
            if (contents[i] == codes[j].c)
            {
                encodedText += codes[j].code;
                break;
            }
        }
    }

    return encodedText;
}

/* this function will take a huffman tree and create a "flattened" string
 * representation of it which can be used to create the header of the binary file */
void flatten(node * huffTree, string &flatTree)
{
    if (!(huffTree -> isLeaf))
    {
        flatTree += '0';
        flatten(huffTree -> left, flatTree);
        flatten(huffTree -> right, flatTree);
    }
    if (huffTree -> isLeaf)
    {
        flatTree += '1';
        int charValue = huffTree -> c;
        // convert character to binary
        string binary = bitset<8>(charValue).to_string();
        flatTree += binary;
    }
}

/* this function will build the header of the file and write it and the encoded text
 * to the binary file */
void writeToFile(string outfilename, string flatTree, string encodedText)
{
    /* create output file, if it already exists, and then overwrite its contents:
     * this is done because its more portable than checking if the specified file already exists
     * and writing to a new file with a similar name to protect the existing file's contents 
     * (checking if a file exists is operating system independent, or requires additional libraries) */
    ofstream outfile;
    outfile.open(outfilename.c_str(), ios::binary | ios::trunc ); 
    if(outfile.is_open())
        cout << "Successfully opened " << outfilename << endl;
    if(!outfile.is_open())
        cout << "Failed to open " << outfilename << endl;
    
    // calculate size of header
    int sizeofTree = 0; // size of tree in bits
    for (int i = 0; i < (int)flatTree.length(); i++)
        sizeofTree++;
    // write integer to the file that contains size of header (tree)
    outfile.write((char *)&sizeofTree, sizeof(sizeofTree));
    cout << "bitsize of header: " << sizeofTree << endl;
    //cout << "wrote 4 byte integer bitsize to file" << endl;
    
    // write the tree to file
    vector <uint> treeInts = splitBinary(flatTree);
    for (int i = 0; i < (int)treeInts.size(); i++)
    {
        outfile.write((char *)&treeInts[i], sizeof(treeInts[i]));
        //cout << "wrote " << hex << treeInts[i] << " to file" << endl;
    }
    cout << "wrote header to file" << endl;

    // determine size of the encoded text
    int sizeofText = 0;
    for (int i = 0; i < (int)encodedText.length(); i++)
        sizeofText++;
    // write integer to the file that contains size of encoded text
    outfile.write((char *)&sizeofText, sizeof(sizeofText));
    cout << "\nbitsize of encoded text: " << sizeofText << endl;
    //cout << "wrote 4 byte integer bitsize to file" << endl;

    // write encoded text to the file
    vector <uint> textInts = splitBinary(encodedText);
    for (int i = 0; i < (int)textInts.size(); i++)
    {
        outfile.write((char *)&textInts[i], sizeof(textInts[i]));
        //cout << "wrote " << hex << textInts[i] << " to file" << endl;
    }
    cout << "wrote encoded text stream to file" << endl;

    //close the file
    outfile.close();
}

/* this function will take a binary string and break it up into several
 * 4 byte integers */
vector <uint> splitBinary(string binary)
{
    vector <uint> integers;

    // count bits the binary will take up
    int bits;
    for (int i = 0; i < (int)binary.length(); i++)
        bits++;
    // if bits not a multiple of 4 bytes (8x4 = 32bits) add zeros
    while (bits % 32 != 0)
    {
        binary += '0';
        bits++;
    }
    // print readable for debugging 
    //cout << "Made multiple of four bytes:" << endl;
    /*for (int i = 0; i < (int)binary.length(); i++)
    {
        cout << binary[i];
        if ((i+1) % 8 == 0)
            cout << " ";
    }
    cout << endl;*/
    
    // grab 4 bytes of the string at a time and convert to integer
    while (binary != "")
    {
        string fourbytes;
        for (int i = 0; i < 32; i++)
        {
            fourbytes += binary[0];
            binary.erase(binary.begin());
        }
        //cout << "four bytes grabbed: " << fourbytes << endl;
        //cout << "binary remaining: " << binary << endl;

        uint integer = toDecimal(fourbytes);
        //cout << "converted four bytes to: " << integer << endl;
        integers.push_back(integer);
    }
    cout << endl;

    return integers;
}

/* this function will take a binary string and convert it to an unsigned decimal integer */
uint toDecimal(string binstr)
{
    uint dec = 0;
    reverse(binstr.begin(), binstr.end());
    for (int i = 0; i < (int)binstr.size(); i++) {
		dec += (int(binstr[i]) - '0') * pow(2, i);
	}

    return dec;
}

#endif
