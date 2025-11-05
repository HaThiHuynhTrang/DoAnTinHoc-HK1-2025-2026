#include "library.h"

int main() {
   /* string input = "Bakery.txt";
    string output = "Cleaned_Bakery.csv";

    cleanBakeryData(input, output);*/

    string cleanedFile = "Cleaned_Bakery.csv";
    Node* root = buildAVLFromCSV(cleanedFile);

    menu(root);

    deleteTree(root);
    return 0;
}