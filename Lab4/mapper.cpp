#include <iostream>
#include <sstream>
using namespace std;

int main() {
    string line, word;
    while (getline(cin, line)) {
        stringstream ss(line);
        while (ss >> word) {
            cout << word << "\t1" << endl;
        }
    }
    return 0;
}