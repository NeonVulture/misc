#include <iostream>

using namespace std;

void Common_Emitter_Amp_GAIN(float &Rsig, float &Rb, float &Rc, float &Re, float &RL, float &Ic, float &Beta) {
    cout << "\n\nTest1 = " << Rsig;
    cout << "\n\nTest2 = " << Rb;
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

    float Rsig, Rb, Rc, Re, RL, Ic, Beta;
    cout << "\nWhat is the value of Rsig? (Enter 0 if Rsig is absent)\nRsig = ";
    cin >> Rsig;
    if (cin.fail()) {
        cout << "\nInvalid input!";
    } else {
        cout << "You entered Rsig = " << Rsig << " Ohms.\n";
    }
    
    cout << "\nWhat is the value of Rb? (Enter 0 if Rb is absent)\nNote: Rb may be the value of R1||R2\nRb = ";
    cin >> Rb;
    if (cin.fail()) {
        cout << "\nInvalid input!";
    } else {
        cout << "You entered Rb = " << Rb << " Ohms.\n";
    }
    
    cout << "\nWhat is the value of Rc? (Enter 0 if Rc is absent)\nRc = ";
    cin >> Rc;
    if (cin.fail()) {
        cout << "\nInvalid input!";
    } else {
        cout << "You entered Rc = " << Rc << " Ohms.\n";
    }
    
    cout << "\nWhat is the value of Re? (Enter 0 if Re is absent)\nRe = ";
    cin >> Re;
    if (cin.fail()) {
        cout << "\nInvalid input!";
    } else {
        cout << "You entered Re = " << Re << " Ohms.\n";
    }
    
    cout << "\nWhat is the value of RL? (Enter 0 if RL is absent)\nRL = ";
    cin >> RL;
    if (cin.fail()) {
        cout << "\nInvalid input!";
    } else {
        cout << "You entered RL = " << RL << " Ohms.\n";
    }
    
    cout << "\nWhat is the value of Ic (in miliamps)? (Enter 0 if Ic is absent)\nIc = ";
    cin >> Ic;
    if (cin.fail()) {
        cout << "\nInvalid input!";
    } else {
        cout << "You entered Ic = " << Ic*1e3 << " mA.\n";
    }
    
    cout << "\nLastly, what is the value of Beta (if beta is very large, enter 999)?\n Beta = ;
    cin >> Beta;
    if (cin.fail()) {
        cout << "\nInvalid input!";
    } else {
        cout << "You entered Beta = " << Beta;
    }
    
    if (choice == 1) {
        cout << "\nYou selected : Common-Emitter Amplifier Circuit\n";
        Common_Emitter_Amp_GAIN(Rsig,Rb,Rc,Re,RL,Ic,Beta);
    }
    
}

int main()
{
    PROMPT();
    return 0;
}
