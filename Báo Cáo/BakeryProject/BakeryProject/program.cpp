#include "library.h"
vector<string> splitLine(const string& line, char delimiter) {
    vector<string> tokens;
    string token;
    stringstream ss(line);
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void cleanBakeryData(const string& inputFile, const string& outputFile) {
    ifstream fin(inputFile);
    ofstream fout(outputFile);

    if (!fin.is_open() || !fout.is_open()) {
        cerr << "Cannot open file!" << endl;
        return;
    }

    string line;
    bool header = true;
    int total = 0, valid = 0, skipped = 0;

    fout << "TransactionNo,Item,DateTime,Daypart,DayType\n";

    while (getline(fin, line)) {
        total++;
        if (header) {
            header = false;
            continue;
        }

        if (line.empty()) {
            skipped++;
            continue;
        }

        auto tokens = splitLine(line, '\t');
        if (tokens.size() < 5) {
            skipped++;
            continue;
        }

        Record rec{ tokens[0], tokens[1], tokens[2], tokens[3], tokens[4] };
        for (string* field : { &rec.trans, &rec.item, &rec.datetime, &rec.daypart, &rec.daytype }) {
            if (!field->empty() && field->front() == ' ') field->erase(0, field->find_first_not_of(' '));
            if (!field->empty() && field->back() == ' ') field->erase(field->find_last_not_of(' ') + 1);
        }
        fout << rec.trans << ","
            << rec.item << ","
            << rec.datetime << ","
            << rec.daypart << ","
            << rec.daytype << "\n";

        valid++;
    }

    cout << "Finished reading all data (" << total << " lines)\n";
    cout << "Valid: " << valid << " lines | Skipped: " << skipped << " incomplete lines\n";
    cout << "Output file: " << outputFile << endl;

    fin.close();
    fout.close();
}



Record parseRecord(const string& line) {
    Record rec;
    auto parts = splitLine(line, ',');
    if (parts.size() >= 5) {
        rec.trans = parts[0];
        rec.item = parts[1];
        rec.datetime = parts[2];
        rec.daypart = parts[3];
        rec.daytype = parts[4];
    }
    return rec;
}

Node* createNode(const Record& rec) {
    Node* node = new Node{ rec, nullptr, nullptr, 1 };
    return node;
}

int getHeight(Node* node) {
    return node ? node->height : 0;
}

int getBalance(Node* node) {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

Node* rotateRight(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = 1 + max(getHeight(y->left), getHeight(y->right));
    x->height = 1 + max(getHeight(x->left), getHeight(x->right));
    return x;
}

Node* rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = 1 + max(getHeight(x->left), getHeight(x->right));
    y->height = 1 + max(getHeight(y->left), getHeight(y->right));
    return y;
}


bool lessThan(const Record& a, const Record& b) {
    if (a.trans < b.trans) return true;
    if (a.trans > b.trans) return false;
    return a.item < b.item;
}

bool greaterThan(const Record& a, const Record& b) {
    if (a.trans > b.trans) return true;
    if (a.trans < b.trans) return false;
    return a.item > b.item;
}


Node* insertNode(Node* root, const Record& rec) {
    if (!root)
        return createNode(rec);

    if (lessThan(rec, root->data))
        root->left = insertNode(root->left, rec);
    else if (greaterThan(rec, root->data))
        root->right = insertNode(root->right, rec);
    else
        return root;

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));

    int balance = getBalance(root);

    if (balance > 1 && lessThan(rec, root->left->data))
        return rotateRight(root);

    if (balance < -1 && greaterThan(rec, root->right->data))
        return rotateLeft(root);

    if (balance > 1 && greaterThan(rec, root->left->data)) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    if (balance < -1 && lessThan(rec, root->right->data)) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}


void inorder(Node* root) {
    if (!root) return;
    inorder(root->left);
    cout 
        << root->data.item <<  endl;
    inorder(root->right);
}


int countNodes(Node* root) {
    if (!root) return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
}


int getTreeHeight(Node* root) {
    return getHeight(root);
}


Node* buildAVLFromCSV(const string& filename) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        cerr << "Cannot open file!" << filename << endl;
        return nullptr;
    }

    string line;
    Node* root = nullptr;

    getline(fin, line);

    while (getline(fin, line)) {
        Record rec = parseRecord(line);
        root = insertNode(root, rec);
    }

    fin.close();
    return root;
}

Node* minValueNode(Node* node) {
    Node* current = node;
    while (current && current->left)
        current = current->left;
    return current;
}

Node* deleteNode(Node* root, const string& item) {
    if (!root) return root;
    if (item < root->data.item)
        root->left = deleteNode(root->left, item);
    else if (item > root->data.item)
        root->right = deleteNode(root->right, item);
    else {
        if (!root->left || !root->right) {
            Node* temp = root->left ? root->left : root->right;
            if (!temp) {
                temp = root;
                root = nullptr;
            }
            else
                *root = *temp;
            delete temp;
        }
        else {
            Node* temp = minValueNode(root->right);
            root->data = temp->data;
            root->right = deleteNode(root->right, temp->data.item);
        }
    }
    if (!root) return root;

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rotateRight(root);
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    if (balance < -1 && getBalance(root->right) <= 0)
        return rotateLeft(root);
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

void deleteTree(Node*& root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
    root = nullptr;
}

void searchAndDisplay(Node* root, const string& keyword) {
    if (!root) return;
    if (root->data.item.find(keyword) != string::npos)
        cout << root->data.item << " (" << root->data.trans << ")" << endl;
    searchAndDisplay(root->left, keyword);
    searchAndDisplay(root->right, keyword);
}

void displayLevel(Node* root, int level) {
    if (!root) return;
    if (level == 1)
        cout << root->data.item << " (" << root->data.trans << ")" << endl;
    else if (level > 1) {
        displayLevel(root->left, level - 1);
        displayLevel(root->right, level - 1);
    }
}
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void menu(Node*& root) {
    int choice;
    do {
        cout << "\n===== AVL Tree Menu =====\n";
        cout << "1. Display inorder traversal\n";
        cout << "2. Search by keyword\n";
        cout << "3. Delete a node\n";
        cout << "4. Delete entire tree\n";
        cout << "5. Display nodes by level\n";
        cout << "6. Display tree height\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            clearScreen();
            cout << "=== Inorder Traversal ===\n";
            inorder(root);
            break;
        case 2: {
            clearScreen();
            string keyword;
            cout << "ALL KEYWORD ARE: TRANS | ITEM | DATETIME | DAYPART | DAYTYPE\n";
            cout << "Enter keyword: ";
            cin >> keyword;
            searchAndDisplay(root, keyword);
            break;
        }
        case 3: {
            clearScreen();
            string item;
            cout << "Enter item to delete: ";
            cin >> item;
            root = deleteNode(root, item);
            cout << "Node deleted.\n";
            break;
        }
        case 4:
            clearScreen();
            deleteTree(root);
            cout << "Tree deleted.\n";
            break;
        case 5: {
            clearScreen();
            int level;
            cout << "Enter level to display: ";
            cin >> level;
            displayLevel(root, level);
            break;
        }
        case 6:
            clearScreen();
            cout << "Tree height: " << getTreeHeight(root) << endl;
            break;
        case 0:
            clearScreen();
            cout << "Exiting...\n";
            break;
        default:
            clearScreen();
            cout << "Invalid choice. Try again.\n";
        if (choice != 0) {
                cout << "\n\nPress Enter to return to the menu...";
                cin.ignore();
                cin.get();
        }
        }
    } while (choice != 0);
    clearScreen();
}