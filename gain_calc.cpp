#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <iostream>
#include <cstdlib>
#include <cmath> // for abs()
#include <iomanip> // for setpercision()

using namespace std;

void Common_Emitter_Amp(float &Rsig, float &Rb, float &Rc, float &Re, float &RL, float &Ic, float &Beta) {
    
    float GAIN;
    float V_TH = 0.025; // Thermal voltage constant equal to 25mV
    float gm = (Ic*1e-3)/V_TH;
    float r_pi = (Beta/gm); // For Hybrid-Pi Model Approach
    float re = V_TH/(Ic*1e-3); // For T_Model Approach
    float R; // Total resistance at the collector in the Pi Model
    
    if (Re == 0) {
        // Case 1: (Re is absent)
        // Use Hybrid-Pi Model Approach
        cout << "\nCalculated parameters for the PI Model:\n";
        cout << "gm = " << gm*1e3 << "mA/V\n";
        cout << "r_pi = " << r_pi << " Ohms.\n";
        
        // Calculate the total resistance at the collector in the Pi Model
        if (Rc == 0) {
            R = RL;
        } else if (RL == 0) {
            R = Rc;
        } else { // This also takes care of the case if Rc = RL = 0
            R = (Rc*RL)/(Rc + RL); // Rc||RL
        }
        if (Rsig == 0 && Rb == 0) {
            // Subcase 1: (Rsig & Rb are absent)
            // GAIN = r_pi * -gmR 
            GAIN = r_pi * gm * R; 
        } else if (Rsig == 0) {
            // Subcase 2: (Rsig is absent but Rb is present)
            // GAIN =  Rb||r_pi * -gmR 
            GAIN = ((Rb * r_pi)/(Rb + r_pi)) * gm * R;
        } else if (Rb == 0) {
            // Subcase 3: (Rb is absent but Rsig is present)
            // GAIN = r_pi/(r_pi+Rsig) * -gmR 
            GAIN = (r_pi/(r_pi + Rsig)) * gm * R;
        } else {
            // Subacase 4: (Rsig and Rb are both present)
            // GAIN = R_IN/(R_IN + Rsig) * -gm * R
            float R_IN = (Rb * r_pi)/(Rb + r_pi); // Input resistance at base (Rb||r_pi)
            GAIN = (R_IN/(R_IN + Rsig)) * gm * R;
        }
    }
    
    // Case 2 (Re is present)
    // Use T-Model approach
    
    cout << "\n\nGAIN = " << GAIN << " V/V";
}

void Emitter_Follower(float &Rsig, float &Rb, float &Rc, float &Re, float &RL, float &Ic, float &Beta) {
    int V_TH = 0.025; // Thermal voltage constant equal to 25mV
}

void General_BJT(float &Rsig, float &Rb, float &Rc, float &Re, float &RL, float &Ic, float &Beta) {
    int V_TH = 0.025; // Thermal voltage constant equal to 25mV
}

void PROMPT() {
    int choice;
    cout << "This program will calculate the gain either a BJT or MOSFET Circuit.\n\n";
    cout << "Before using this calculator, make sure that:\n\n";
    cout << "1. You have drawn the AC circuit of the BJT \n";
    cout << "2. You know the value of the collector current (Ic) \n\n";
    cout << "Below is a set of BJT circuits. Select the type of circuit you wish to calculate the gain of\n";
    cout << "1. Common-Emitter Amplifier\n";
    cout << "2. Common-Collector/Emitter-Follower\n";
    cout << "3. General BJT circuit\n\n";
    cout << "Choice: ";
    cin >> choice;
    if (cin.fail()) {
        cout << "Invalid Choice!\n";
        exit(42);
    }

    float Rsig, Rb, Rc, Re, RL, Ic, Beta;
    cout << "\nWhat is the value of Rsig? (Enter 0 if Rsig is absent)\nRsig = ";
    cin >> Rsig;
    if (cin.fail()) {
        cout << "\nInvalid input!";
        exit(42);
    } else {
        cout << "You entered Rsig = " << Rsig << " Ohms.\n";
    }
    
    cout << "\nWhat is the value of Rb? (Enter 0 if Rb is absent)\nNote: Rb may be the value of R1||R2\nRb = ";
    cin >> Rb;
    if (cin.fail()) {
        cout << "\nInvalid input!";
        exit(42);
    } else {
        cout << "You entered Rb = " << Rb << " Ohms.\n";
    }
    
    cout << "\nWhat is the value of Rc? (Enter 0 if Rc is absent)\nRc = ";
    cin >> Rc;
    if (cin.fail()) {
        cout << "\nInvalid input!";
        exit(42);
    } else {
        cout << "You entered Rc = " << Rc << " Ohms.\n";
    }
    
    cout << "\nWhat is the value of Re? (Enter 0 if Re is absent)\nRe = ";
    cin >> Re;
    if (cin.fail()) {
        cout << "\nInvalid input!";
        exit(42);
    } else {
        cout << "You entered Re = " << Re << " Ohms.\n";
    }
    
    cout << "\nWhat is the value of RL? (Enter 0 if RL is absent)\nRL = ";
    cin >> RL;
    if (cin.fail()) {
        cout << "\nInvalid input!";
        exit(42);
    } else {
        cout << "You entered RL = " << RL << " Ohms.\n";
    }
    
    cout << "\nWhat is the value of Ic (in miliamps)?\nIc = ";
    cin >> Ic;
    if (cin.fail()) {
        cout << "\nInvalid input!";
        exit(42);
    } else {
        cout << "You entered Ic = " << Ic*1e-3 << " A.\n";
    }
    
    cout << "\nLastly, what is the value of Beta (if beta is very large, enter 999)?\nBeta = ";
    cin >> Beta;
    if (cin.fail()) {
        cout << "\nInvalid input!";
        exit(42);
    } else {
        cout << "You entered Beta = " << Beta;
    }
    
    if (choice == 1) {
        cout << "\nYou selected: Common-Emitter Amplifier Circuit\n";
        Common_Emitter_Amp(Rsig,Rb,Rc,Re,RL,Ic,Beta);
    }
    
    if (choice == 2) {
        cout << "\nYou selected: Common-Collector/Emitter-Follower Circuit\n";
        Emitter_Follower(Rsig,Rb,Rc,Re,RL,Ic,Beta);
    }
    
    if (choice == 3) {
        cout << "\nYou selected: General BJT Circuit\n";
        General_BJT(Rsig,Rb,Rc,Re,RL,Ic,Beta);
    }
    
}

int main()
{
    PROMPT();
    return 0;
}
