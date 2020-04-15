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
        cout << "Notation:\nAvo = Terminal Voltage Gain\nGv = Overall Voltage Gain\n";
        cout << "\nCalculated parameters for the PI Model:\n";
        cout << "gm = " << gm*1e3 << "mA/V\n";
        cout << "r_pi = " << r_pi << " Ohms.\n";
        
        // Calculate the total resistance at the collector in the Pi Model
        if (Rc == 0) {
            R = RL;
            cout << "R = " << R << " Ohms\n";
        } else if (RL == 0) {
            R = Rc;
            cout << "R = " << R << " Ohms\n";
        } else { // This also takes care of the case if Rc = RL = 0
            R = (Rc*RL)/(Rc + RL); // Rc||RL
            cout << "R = " << R << " Ohms\n";
        }
        
        float Avo = gm * R;
        
        cout << "Avo = " << Avo << " V/V\n";
        
        if (Rsig == 0 && Rb == 0) {
            // Subcase 1: (Rsig & Rb are absent)
            // GAIN = r_pi * -gmR
            cout << "Case 1\n";
            GAIN = r_pi * Avo; 
        } else if (Rsig == 0 && Rb !=0) {
            // Subcase 2: (Rsig is absent but Rb is present)
            // GAIN =  Rb||r_pi * -gmR 
            GAIN = ((Rb * r_pi)/(Rb + r_pi)) * Avo;
            cout << "Case 2\n";
        } else if (Rb == 0 && Rsig != 0) {
            // Subcase 3: (Rb is absent but Rsig is present)
            // GAIN = r_pi/(r_pi+Rsig) * -gmR 
            GAIN = (r_pi/(r_pi + Rsig)) * Avo;
            cout << "Case 3\n";
        } else {
            // Subacase 4: (Rsig and Rb are both present)
            // GAIN = R_IN/(R_IN + Rsig) * -gm * R
            float R_IN = (Rb * r_pi)/(Rb + r_pi); // Input resistance at base (Rb||r_pi)
            GAIN = (R_IN/(R_IN + Rsig)) * Avo;
            cout << "Case 4\n";
        }
    } else {
        // Case 2 (Re is present)
        // Use T-Model approach
        
        //Calculate the Reflected Resistance to Base (Re+re)(beta+1) depending if beta is very large or not
        float RRB;
        if (Beta == 999) {
            RRB = 1;
        } else {
            RRB = (Re + re) * (Beta + 1); // Reflected Resistance to Base (RRB)
        }
        
        float Zc; // Total resistance of collector
        
        if (Rc == 0 && RL == 0) {
            // Do Not Calculate
            cout << "\nLooks like the circuit is an emitter follower. Retry the program.";
            exit(42);
        } else if (Rc == 0) {
            Zc = RL;
            cout << "Zc = " << Zc << " Ohms\n";
        } else if (RL == 0) {
            Zc = Rc;
            cout << "Zc = " << Zc << " Ohms\n";
        } else {
            Zc = (Rc * RL)/(Rc + RL); // Rc||RL
            cout << "Zc = Rc||RL = " << Zc << " Ohms\n";
        }
        
        float Ze = Re + re; // Total resistance of emitter (Re may be 0)
        cout << "Ze = " << Ze << " Ohms\n";
        
        float Zb; // The total resistance at the base including Rb (if present) but excluding Rsig (if present)
        // NOTE: Calculation for Zb does not depend on Rsig but does depend on Beta
        
        if (Beta == 999 && Rb != 0) { // Beta is very large AND Rb is present        
            // Consider only Rb
            Zb = Rb;
            cout << "Zb = " << Zb << " Ohms\n";
        } else if (Beta == 999 && Rb == 0) { // Beta is VERY LARGE AND Rb is absent  
            Zb = 1; // FLAG
        } else if (Beta != 999 && Rb == 0) { // Beta is SOME VALUE AND Rb is absent   
            // Zb = RRB = (Beta + 1)(Re + re)
            Zb = RRB;
            cout << "Zb = " << Zb << " Ohms\n";
        } else { // Beta is some fixed value like 100 AND Rb is present
            // Zb = Rb||RRB
            Zb = (Rb * RRB)/(Rb + RRB);
            cout << "Zb = " << Zb << " Ohms\n";
        }
        
        if (Rsig == 0) { 
            // Subcase 1: Rsig is absent and Rb may or may not be absent
            cout << "\nCase 1: Rsig is absent\n";
            cout << "Avo = " << Zc/Ze << " V/V";
            GAIN = Zb * (Zc/Ze); // Zb = (re+re)(Beta + 1) or Zb = 1 (i.e beta is VERY LARGE) or Zb = Rb or Zb = Rb||RRB 
        } else if (Rb == 0) {
            // Subcase 2: If Rb is absent but Rsig is present and Zb is either (2) or (3)
            cout << "\nCase 2: Rb is absent but Rsig is present\n";
            if (Zb == 1) { // beta is large
                cout << "Avo = " << Zc/Ze << " V/V";
                GAIN = (Zc/Ze); // Omit Rsig
            } else { // beta is some value
                cout << "Avo = " << Zc/(Ze + (Rsig/(Beta + 1))) << " V/V";
                GAIN = (Zb * Zc)/((Zb + Rsig)*Ze);  
            }
        } else {
            // Subcase 4: If both Rsig and Rb are both present
            cout << "\nCase 4\n";
            cout << "Avo = " << Zc/Ze << " V/V";
            GAIN = (Zb/(Zb + Rsig)) * (Zc/Ze); // Zb = RRB||Rb or Zb = Rb
        }
    }
    
    cout << "\n\nGAIN = " << GAIN << " V/V";
}

void Emitter_Follower(float &Rsig, float &Rb, float &Rc, float &Re, float &RL, float &Ic, float &Beta) {
     // Only T-Model is appropriate 
    float V_TH = 0.025; // Thermal voltage constant equal to 25mV
    float re = V_TH/(Ic*1e-3); // For T_Model Approach
    float GAIN;
    
    cout << "\nCalculated parameters for T-Model:\n";
    cout << "re = 25mV/Ic = " << re << " Ohms.\n";
    
    float RE; // Total resistance in the emitter
    if (Re == 0 && RL == 0) {
        // Do Not Compute
        cout << "\nLooks like the circuit is a Common-Emitter Amp or General BJT";    
        exit(30);
    } else if (Re == 0) {
        RE = RL;
        cout << "RE' = RL = " << RE << " Ohms.\n";
    } else if (RL == 0) {
        RE = Re;
        cout << "RE' = Re = " << RE << " Ohms.\n";
    } else {
        RE = (Re * RL)/(Re + RL); // Rc||RL
        cout << "RE' = Re||RL = " << RE << " Ohms.\n";
    }
    
    float RRB = (RE + re) * (Beta + 1); // Reflected Resistance to Base (ONLY IF Beta is some value)
    
    float Av = RE/(RE+re); // Voltage Gain Av
    
    cout << "Av = " << Av << "V/V.\n";
    
    float Zb; // The total resistance at the base including Rb (if present) but excluding Rsig (if present)
    // NOTE: Calculation for Zb does not depend on Rsig but does depend on Beta
        
    if (Rb == 0) { // Beta is SOME VALUE but Rb is absent   
        Zb = RRB;
        cout << "Zb = (RE'+re)(Beta+1) = " << Zb << " Ohms.\n";
    } else { // Beta is some fixed value like 100 AND Rb is present
        //(4)
        Zb = (Rb * RRB)/ (Rb + RRB); // Rb||RRB
        cout << "Zb = Rb||(RE'+re)(Beta+1) = " << Zb << " Ohms.\n";
    }
    
    if (Beta == 999) {
        // Subcase 1: Beta is VERY LARGE (i.e infinite)
        // Ib = 0, Ic = Ie
        cout << "\nCase 1\n";
        GAIN = Av;
    } else if (Rsig == 0 && Rb == 0) {
        // Subcase 2a: Rsig and Rb are both absent ( Zb = RRB = (RE'+re)(Beta + 1) )
        // Subcase 2b: Rsig is absent but Rb is present (Zb = Rb||RRB)
        cout << "\nCase 2\n";
        GAIN = Zb * Av; 
    } else  {
        // Subcase 4a: If Rb is absent but Rsig is present
        // Subcase 4b: If Rb and Rsig are both present
        cout << "\nCase 3\n";
        GAIN = (Zb/(Zb + Rsig)) * Av; // Zb = RRB = (RE+re)(Beta + 1) or Zb = Rb||RRB
    } 
    
    cout << "GAIN = Gv = " << GAIN << " V/V"; 
}

void General_BJT(float &Rsig, float &Rb, float &Rc, float &Re, float &RL, float &Ic, float &Beta) {
    int V_TH = 0.025; // Thermal voltage constant equal to 25mV
    // GAIN = 
}

void BJT_PROMPT() {
    int choice;
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
    cout << "\nYou entered Beta = " << Beta ; 
    
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

void MOSFET_PROMPT() {
    cout << "\n\nComing Soon!";
}

int main()
{
    int choice;
    cout << "This program will calculate the gain of either a BJT or MOSFET circuit.\n\n";
    cout << "Select the circuit you wish to anaylize.\n\n";
    cout << "1. BJT\n";
    cout << "2. MOSFET\n\n";
    cout << "Choice: ";
    cin >> choice;
    
    if (choice == 1) {
        BJT_PROMPT();
    } else if (choice == 2) {
        MOSFET_PROMPT();
    } else {
        cout << "\nInvalid Choice!";
        exit(42);
    }
    
    return 0;
}
