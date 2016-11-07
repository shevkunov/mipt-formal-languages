#include <iostream>
#include <fstream>
#include <string>
#include "automat.h"
#include "regsub.h"

using namespace std;

int main() {
    string word, reg;
    cin >> reg >> word;
    std::pair <int, int> ans = mfl::regSubDP<mfl::DPVSubWord>(reg, word);
    //only size :
    //cout << ans.first << "\n";

    //substring [L, R):
    cout << ans.second << " " << ans.second + ans.first << "\n";
    return 0;
}
