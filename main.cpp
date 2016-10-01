#include <iostream>
#include <string>
#include <automat.h>

using namespace std;

std::string s = "8 2 0 a 0 0 b 0 0 b 1 1 a 4 1 a 2 2 a 2 2 b 2 2 a 3 3 4";

int main()
{
    cout << mfl::Automat(s).toString();
    cout << mfl::Automat(s).determinize().toString();
    cout << mfl::Automat(s).determinize().completion().toString();
    cout << mfl::Automat(s).minimize().toString();
    return 0;
}
