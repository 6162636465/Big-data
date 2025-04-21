#include <iostream>
#include <string>
using namespace std;

int main() {
    string current_word, word;
    int count = 0, value;

    while (cin >> word >> value) {
        if (word == current_word) {
            count += value;
        } else {
            if (!current_word.empty())
                cout << current_word << "\t" << count << endl;
            current_word = word;
            count = value;
        }
    }
    if (!current_word.empty())
        cout << current_word << "\t" << count << endl;

    return 0;
}