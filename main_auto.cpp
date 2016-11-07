#include <iostream>
#include <fstream>
#include <string>
#include "automat.h"
#include "regsub.h"

using namespace std;

/**
 * Format (we can use any whitespace symbols as a delimeter):
 * EdgesCount FinalVertexCount
 * Edge1Start Edge1Char Edge1End
 * Edge2Start Edge2Char Edge2End
 * ...
 * Edge(EdgesCount - 1)Start Edge(EdgesCount - 1)Char Edge(EdgesCount - 1)End
 * FinalVertex1 FinalVertex2 ... FinalVertex(FinalVertexCount)
 */

///Example:
///std::string s = "8 2 0 a 0 0 b 0 0 b 1 1 a 4 1 a 2 2 a 2 2 b 2 2 a 3 3 4";


int main()
{
    //ifstream cin("input.txt");
    //ofstream cout("output.txt");

    std::string s;

    while (cin) {
        std::string line;
        cin >> line;
        s += line + " ";
    }

    cout << mfl::Automat(s).toString();
    //cout << mfl::Automat(s).determinize().toString();
    //cout << mfl::Automat(s).determinize().completion().toString();
    cout << mfl::Automat(s).minimize().toString();
    //cout << mfl::Automat(s).determinize().completion().toString();
    // + reverse vertex to find completion for a language
    return 0;
}
