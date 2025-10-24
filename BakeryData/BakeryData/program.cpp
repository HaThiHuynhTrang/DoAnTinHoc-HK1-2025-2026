#include"library.h"
using namespace std;
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, last - first + 1);
}

string normalizeItem(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    if (!s.empty()) s[0] = toupper(s[0]);
    return s;
}

bool compare(const Record& a, const Record& b) {
    return a.transNo < b.transNo;
}


Node* insertBST(Node* root, Record rec) {
    if (root == nullptr) {
        Node* newNode = new Node{ rec, nullptr, nullptr };
        return newNode;
    }
    if (rec.transNo < root->data.transNo)
        root->left = insertBST(root->left, rec);
    else if (rec.transNo > root->data.transNo)
        root->right = insertBST(root->right, rec);
    else {
        if (rec.item < root->data.item)
            root->left = insertBST(root->left, rec);
        else if (rec.item > root->data.item)
            root->right = insertBST(root->right, rec);
    }
    return root;
}

void inorderWrite(Node* root, ofstream& fout) {
    if (root == nullptr) return;
    inorderWrite(root->left, fout);
    fout << root->data.transNo << ","
        << root->data.item << ","
        << root->data.datetime << ","
        << root->data.daypart << ","
        << root->data.daytype << "\n";
    inorderWrite(root->right, fout);
}