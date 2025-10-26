#include"library.h"
using namespace std;
void main() {
    ifstream fin("Bakery.txt");
    ofstream fout("Bakery_cleaned.csv");

    if (!fin.is_open()) {
        cout << "Can't open file bakery.txt \n";
        return;
    }

    fout << "TransactionNo,Items,DateTime,Daypart,DayType\n";

    string line;
    getline(fin, line); 

    Node* root = nullptr;
    int count = 0;

    while (getline(fin, line)) {
        stringstream ss(line);
        string transNo, item, datetime, daypart, daytype;

        getline(ss, transNo, '\t');
        getline(ss, item, '\t');
        getline(ss, datetime, '\t');
        getline(ss, daypart, '\t');
        getline(ss, daytype, '\t');


        transNo = trim(transNo);
        item = normalizeItem(trim(item));
        datetime = trim(datetime);
        daypart = trim(daypart);
        daytype = trim(daytype);

        if (transNo.empty() || item.empty() || datetime.empty()) continue;

        Record rec = { transNo, item, datetime, daypart, daytype };
        root = insertBST(root, rec);
        count++;
    }

    inorderWrite(root, fout);
    fin.close();
    fout.close();

    cout << "data have been cleaned \n";
    cout << "Total number of records retrieved: " << count << endl;
    cout << "data have been save in Bakery_cleaned.csv\n";
}