#include <iostream>

using namespace std;

void GAIN(float &RBB) {
    cout << "\n\nTest " << RBB;
}

void PROMPT() {
    int choice;
    cout << "This program will calculate the gain either a BJT or MOSFET Circuit.\n";
    cout << "Before using this calculator, make sure that:\n";
    cout << "1. You have drawn the AC circuit of the BJT \n\n";
    cout << "Below is a set of BJT circuits. Select the type of circuit you wish to calculate the gain of\n";
    cout << "1. Common-Emitter Amplifier\n";
    cout << "2. Common-Collector/Emitter-Follower\n";
    cout << "3. General BJT circuit\n\n";
    cout << "Choice: ";
    cin >> choice;
    if (cin.fail()) {
        cout << "Invalid Choice!\n";
    }
    cout << "\nYou selected: " << choice << "\n";
    float RBB;
    cout << "\nWhat is the value of RBB? ";
    cin >> RBB;
    if (cin.fail()) {
        cout << "\nInvalid input!";
    }
    GAIN(RBB);
}

int main()
{
    BJT_PROMPT();
    return 0;
}
