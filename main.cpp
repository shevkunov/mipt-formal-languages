#include <iostream>
#include <automat.h>

using namespace std;

int main(int argc, char *argv[])
{
    mfl::Automat a("2 1 0 a 0 0 b 1 1");
    cout << a.toString();
    return 0;
}
