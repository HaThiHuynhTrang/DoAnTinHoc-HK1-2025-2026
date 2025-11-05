#ifndef LIBRARY_H
#define LIBRARY_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <tuple>
using namespace std;

struct Record {
    string trans;
    string item;
    string datetime;
    string daypart;
    string daytype;
};

struct Node {
    Record data;
    Node* left;
    Node* right;
    int height;
};
void cleanBakeryData(const string& inputFile, const string& outputFile);

vector<string> splitLine(const string& line, char delimiter = ',');
Record parseRecord(const string& line);
Node* createNode(const Record& rec);
int getHeight(Node* node);
int getBalance(Node* node);
Node* rotateRight(Node* y);
Node* rotateLeft(Node* x);

Node* insertNode(Node* root, const Record& rec);
void inorder(Node* root);
int countNodes(Node* root);
int getTreeHeight(Node* root);

Node* buildAVLFromCSV(const string& filename);
Node* deleteNode(Node* root, const string& item);   
void deleteTree(Node*& root);                      
void searchAndDisplay(Node* root, const string& keyword); 
void displayLevel(Node* root, int level);           
void menu(Node*& root);                            
#endif

