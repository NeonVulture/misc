/******************************************************************************

                          BJT DC-Circuit Analysis Program
               	     I'm 95% sure this program works correctly

*******************************************************************************/
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

int main()
{
    float VBB, RB, VCC, RC, VEE, RE; // Known values to be entered by user
    int beta;

    // constant parameters for BJT
    float Vbe = 0.7; // Active & Saturation Mode
    float Vce = 0.2; // Saturation Mode Only
    float Vcb = -0.5; // Saturation Mode Only

    cout << "Welcome to BJT Circuit Solver!\n\n";
    sleep(2);
    cout << "This progam will calculate all node voltages & currents.\n\n";
    sleep(2);
    cout << "Before entering the values make sure and keep in mind that:\n";
    cout << "1. You already know the values of RB, RC, RE, VBB, VCC, VEE.\n";
    cout << "2. RB must be a single resistor. If not, modify the circuit (i.e. use Thevenin's Theorem).\n";
    cout << "3. The BJT Must be a NPN transistor (values will be off for pnp transistor)\n";
    cout << "4. Vbe is assummed to be equal to 0.7V. If problems state Vbe is something else, note that calculations may be a bit off\n\n";
    sleep(2);
    cout << "Let's Begin:\n\n";
    cout << "What is the value of VBB? ";
    cin >> VBB;
    if (cin.fail()) {
        cout << "Invalid input\n";
        exit(42);
    }
    cout << "You entered " << VBB << "V\n\n";
    cout << "What is the value of RB? (If there is no resistor, enter 1) ";
    cin >> RB;
    if (cin.fail()) {
        cout << "Invalid input\n";
        exit(42);
    }
    cout << "You entered " << RB << " Ohms\n\n";
    cout << "What is the value of VCC? ";
    cin >> VCC;
    if (cin.fail()) {
        cout << "Invalid input\n";
        exit(42);
    }
    cout << "You entered " << VCC << "V\n\n";
    cout << "What is the value of RC? (If there is no resistor, enter 1) ";
    cin >> RC;
    if (cin.fail()) {
        cout << "Invalid input\n";
        exit(42);
    }
    cout << "You entered " << RC << " Ohms\n\n";
    cout << "What is the value of VEE? ";
    cin >> VEE;
    if (cin.fail()) {
        cout << "Invalid input\n";
        exit(42);
    }
    cout << "You entered " << VEE << "V\n\n";
    cout << "What is the value of RE? (If there is no resistor, enter 1) ";
    cin >> RE;
    if (cin.fail()) {
        cout << "Invalid input\n";
        exit(42);
    }
    cout << "You entered " << RE << " Ohms\n\n";
    cout << "Lastly, what is the value of beta? (If beta is very large, enter 999). ";
    cin >> beta;
    if (cin.fail()) {
        cout << "Invalid input\n";
        exit(42);
    }
    cout << "You entered beta = " << beta << "\n\n";

    float Ib, Ic, Ie, Vb, Vc, Ve;

    // Calculate the currents
    if (beta == 999) {
        Ib = 0; // Neglect any base current
        Ie = (VBB - Vbe)/RE; // See EX5
        Ic = Ie; // for beta >> 1
    } else {
        Ib = (VBB - VEE - Vbe)/(RB + ((beta + 1) * RE));
        Ie = (beta + 1) * Ib;
        Ic = beta * Ib;
    }

    // Calculate Vce and check if the BJT is active or saturated
    float Vce_check = VCC - (RC*Ic) - (RE*Ie) - VEE;

    if (Vce_check < Vce) {
      cout << "\nThe BJT is operating in the SATURATION REGION\n";
      cout << "That is because Vce = " << Vce_check << "V and is < " << Vce;

      // Calculate the currents Ib, Ic, Ie
      // a1x + b1y + c1 = 0 : where x = Ib & y = Ic
      // a2x + b2y + c2 = 0 :
      float a1, b1, c1;
      float a2, b2, c2;

      a1 = RB + RE;
      b1 = RE;
      c1 = VBB - Vbe - VEE;
      a2 = RE;
      b2 = RC + RE;
      c2 = VCC - Vce - VEE;

      // Find Ib and Ic via cross-multiplication
      float x, y;
      x = (b1*c2 - b2*c1) / (a1*b2 - a2*b1);
      y = (c1*a2 - c2*a1) / (a1*b2 - a2*b1);

      Ib = abs(x);
      Ic = abs(y);
      Ie = Ib + Ic;
      cout << "\n\nIb = " << Ib;
      cout << "\n\nIc = " << Ic;
      cout << "\n\nIe = " << Ie;

      // Find the voltages Vb, Vc, Ve
      Ve = (RE * Ie) - VEE;
      Vc = Vce + Ve;
      Vb = Vbe + Ve;

      // Show the final results
      cout << "\n\nSummary: \n ";
      cout << "\tIb = " << setprecision(3) << Ib * 1e3 <<"mA\n";
      cout << "\tVb = " << Vb <<"V\n";
      cout << "\tIc = " << Ic * 1e3 <<"mA\n";
      cout << "\tVc = " << Vc <<"V\n";
      cout << "\tIe = " << Ie * 1e3 <<"mA\n";
      cout << "\tVe = " << Ve <<"V\n";

    } else {
      cout << "\nThe BJT is operating in the ACTIVE REGION";
      Vb = VBB - (RB*Ib);
      Vc = VCC - (RC*Ic);
      Ve = (RE*Ie) + VEE; // Double Check

      // Show the final results
      cout << "\n\nSummary: \n ";
      cout << "\tIb = " << setprecision(3) << Ib * 1e3 <<"mA\n";
      cout << "\tVb = " << Vb <<"V\n";
      cout << "\tIc = " << Ic * 1e3 <<"mA\n";
      cout << "\tVc = " << Vc <<"V\n";
      cout << "\tIe = " << Ie * 1e3 <<"mA\n";
      cout << "\tVe = " << Ve <<"V\n";

    }
    return 0;
}
