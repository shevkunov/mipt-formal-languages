#include <iostream>
#include <string>
#include <automat.h>

using namespace std;

int main()
{
    mfl::Automat a("10 2 0 a 1 1 b 1 1 a 2 2 a 3 2 a 4 3 a 2 3 b 1 3 a 5 4 b 2 5 b 3 1 3");
    cout << a.toString();
    cout << a.determinize().toString();
    return 0;
}
