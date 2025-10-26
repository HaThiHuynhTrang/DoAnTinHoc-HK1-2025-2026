#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
using namespace std;
struct Record {
    string transNo, item, datetime, daypart, daytype;
};
struct Node {
    Record data;
    Node* left;
    Node* right;
};
string trim(const string& str);
string normalizeItem(string s);
bool compare(const Record& a, const Record& b);
Node* insertBST(Node* root, Record rec);
void inorderWrite(Node* root, ofstream& fout);