#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "tasks_generator.h"

using namespace std;


void generate_serie(const string fileName_string, const int casesNum, const int n, const float U, const float Umax,
                    const float Umin, const float Pmin_LB, const float Pmin_UB, const float Pratio,
                    const float allowedDeviation) {
    cout << "generate_series" << endl;
    cout << "n: " << n << endl;
    cout << "U: " << U << endl;
    cout << "Umax: " << Umax << endl;
    cout << "Umin: " << Umin << endl;
    cout << "Pmin_LB: " << Pmin_LB << endl;
    cout << "Pmin_UB: " << Pmin_UB << endl;
    cout << "Pratio: " << Pratio << endl;
    cout << "allowedDeviation: " << allowedDeviation << endl;

    const unsigned short MAXN = 16;
    int C[MAXN];
    int P[MAXN];
    
    int curCase = 0;
    // TODO add column names to the generated files
    while (curCase < casesNum) {
        // Generate task set
        generate_taskset(n, U, Umax, Umin, Pmin_LB, Pmin_UB, Pratio, allowedDeviation, 1, C, P);
        
        cout << "generated task set:" << endl;
        for (unsigned short i = 0; i < n; i++) {
            cout << "( " << C[i] << ", " << P[i] << " )" << endl;
        }
        cout << endl << endl;

        // store task set parameters in file
        const char* fileName = fileName_string.c_str();
        ofstream fileResults;
        fileResults.open(fileName, ios::app);
        
        fileResults << n << "\t" << U << "\t" << Umax << "\t" << Pratio;
        for (unsigned short taskItr = 0; taskItr < n; taskItr++) {
            fileResults << "\t" << C[taskItr] << "\t" << P[taskItr];
        }
        fileResults << endl;
        fileResults.close();
        
        curCase++;
    }
}


string generate_file_name(const int expType, const int n, const float U, const float Umax, const float Pratio) {
    ostringstream ss_n;
    ss_n << ((unsigned short)n);
    string n_str(ss_n.str());
    
    ostringstream ss_U;
    ss_U << ((float)U);
    string U_str(ss_U.str());
    
    ostringstream ss_Umax;
    ss_Umax << ((float)Umax);
    string Umax_str(ss_Umax.str());
    
    ostringstream ss_Pratio;
    ss_Pratio << ((float)Pratio);
    string Pratio_str(ss_Pratio.str());
    
    string fileName;

    if (expType == 0) {
        fileName = string("task_sets_varying_n[U_") + U_str + "_Umax_" + Umax_str + "_Pratio_" + Pratio_str + "].tsv";
    } else if (expType == 1) {
        fileName = string("task_sets_varying_U[n_") + n_str + "_Umax_" + Umax_str + "_Pratio_" + Pratio_str + "].tsv";
    } else if (expType == 2) {
        fileName = string("task_sets_varying_Pratio[n_") + n_str + "_U_" + U_str + "_Umax_" + Umax_str + "].tsv";
    } else if (expType == 3) {
        fileName = string("task_sets_varying_Umax[n_") + n_str + "_U_" + U_str + "_Pratio_" + Pratio_str + "].tsv";
    }
    
    return fileName;
}


int main() {
    int expType;
    int n = 0;
    float U = 0;
    float Umax = 0;
    float Umin = 0;
    int Pmin_LB;
    int Pmin_UB;
    float Pratio = 1;
    float allowedDeviation;
    int casesNum;

    cerr << "Experiment type [0 - varying n, 1 - varying U, 2 - varying Pmax/Pmin, 3 - varying Umax]?\n> ";
    cin >> expType;
    
    if (expType != 0) {
        cerr << "n?\n> ";
        cin >> n;
    }
    
    if (expType != 1) {
        cerr << "U?\n> ";
        cin >> U;
    }
    
    if (expType != 3) {
        cerr << "Umax?\n> ";
        cin >> Umax;
    }
    
    if (expType != 2) {
        cerr << "Pmax/Pmin?\n> ";
        cin >> Pratio;
    }
    
    cerr << "Pmin_LB?\n> ";
    cin >> Pmin_LB;
    cerr << "Pmin_UB?\n> ";
    cin >> Pmin_UB;
    cerr << "Allowed deviation?\n> ";
    cin >> allowedDeviation;
    cerr << "Number of cases to generate?\n> ";
    cin >> casesNum;

    string fileName_string = generate_file_name(expType, n, U, Umax, Pratio);

    if (expType == 0) {
        for (n = 7; n <= 11; ++n) {  // TODO: move Pratio to user input
            cout << "n: " << n << endl;
            generate_serie(fileName_string, casesNum, n, U, Umax,
                           Umin, Pmin_LB, Pmin_UB, Pratio, allowedDeviation);
        }
    } else if (expType == 1) {
        for (U = 1.6; U < 4.05; U = U + 0.1) {  // TODO: move Pratio to user input
            cout << "U: " << U << endl;
            generate_serie(fileName_string, casesNum, n, U, Umax,
                           Umin, Pmin_LB, Pmin_UB, Pratio, allowedDeviation);
        }
    } else if (expType == 2) {        
        for (Pratio = 6; Pratio <= 6; Pratio = Pratio + 1) {  // TODO: move Pratio to user input
            cout << "Pratio: " << Pratio << endl;
            generate_serie(fileName_string, casesNum, n, U, Umax,
                           Umin, Pmin_LB, Pmin_UB, Pratio, allowedDeviation);
        }
    } else if (expType == 3) {
        for (Umax = 0.4; Umax < 1.01; Umax = Umax + 0.05) {  // TODO: move Pratio to user input
            cout << "Umax: " << Umax << endl;
            generate_serie(fileName_string, casesNum, n, U, Umax,
                           Umin, Pmin_LB, Pmin_UB, Pratio, allowedDeviation);
        }
    }

    return 0;
}
