/* puff.hpp
 * Written by:  Keefer Rourke
 * License:     GPLv3
 *
 * COPYRIGHT    Keefer Rourke 2015
 *
 * Description: This header file contains a set of functions required
 *              to inflate a Huffman binary file to its original plain-text
 *              format
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

#ifndef __PUFF_HPP__
#define __PUFF_HPP__

#include <iostream>
#include <fstream>
#include <string>

using namespace std;
typedef unsigned int uint;

/* function prototypes */
string getBinContents(string infilename);
string readHeader(string &contents);
node * rebuildHuffTree(string huffInfo);
vector <huffcode> regenCodes(node * huffTree);
string decode(string &contents, vector <huffcode> codes);
void   writeTxtFile(string &decodedText);

/* this function will read a binary file and rebuild a Huffman tree from the file
 * header, it will then use that tree to decode the encoded text stream and output
 * it to a new plain-text file */
void huffExtract(string infilename, string outfilename = "out.txt")
{
    // get the binary file contents as a string of 1s and 0s
    string binContents = getBinContents(infilename);
    // read in header
    string huffInfo = readHeader(binContents);
    // build huffman tree
    node * huffTree = rebuildHuffTree(huffInfo);
    // regenerate prefix codes
    vector <huffcode> codes = regenCodes(huffTree);
    // read in and decode text
    string decodedText = decode(binContents, codes);
    // write decoded text to file
    writeTxtFile(decodedText);
}

/* this function will get the contents of the binary file and save to a 
 * string of 1s and 0s */
string getBinContents(string infilename)
{
    string binContents = "";
    return binContents;
}

/* this function will extract the header from the file */
string readHeader(string &contents)
{
    string header = "";
    return header;
}

/* this function will use the extracted header to rebuild a Huffman tree */
node * rebuildHuffTree(string huffInfo)
{
    node * huffTree = createNode(-1, false);
    return huffTree;
}

/* this function will use the rebuild Huffman tree to generate the prefix codes
 * for each character that should be in the plain-text file */
vector <huffcode> regenCodes(node * huffTree)
{
    vector <huffcode> codes;
    return codes;
}

/* this function will decode the encoded text stream from the file and return
 * the plain-text */
string decode(string &contents, vector <huffcode> codes)
{
    string decodedText = "";
    return decodedText;
}

/* this function will open a file (over-write any existing contents) and write
 * the decoded text to it */
void writeTxtFile(string &decodedText)
{
}

#endif
