#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "tasks_generator.h"

using namespace std;


void generate_serie(const string fileName_string, const int casesNum, const int n, const float U, const float Umax,
                    const float Umin, const int Pmin_LB, const int Pmin_UB, const float Pratio,
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


string generate_file_name(const int n, const float U, const float Umax, const float Umin, const int Pmin_LB,
                          const int Pmin_UB, const float Pratio, const float allowedDeviation, const int casesNum) {
    ostringstream ss_n;
    ss_n << ((unsigned short)n);
    string n_str(ss_n.str());
    
    ostringstream ss_U;
    ss_U << ((float)U);
    string U_str(ss_U.str());
    
    ostringstream ss_Umax;
    ss_Umax << ((float)Umax);
    string Umax_str(ss_Umax.str());

    ostringstream ss_Umin;
    ss_Umin << ((float)Umin);
    string Umin_str(ss_Umin.str());

    ostringstream ss_Pmin_LB;
    ss_Pmin_LB << ((float)Pmin_LB);
    string Pmin_LB_str(ss_Pmin_LB.str());

    ostringstream ss_Pmin_UB;
    ss_Pmin_UB << ((float)Pmin_UB);
    string Pmin_UB_str(ss_Pmin_UB.str());
    
    ostringstream ss_Pratio;
    ss_Pratio << ((float)Pratio);
    string Pratio_str(ss_Pratio.str());

    ostringstream ss_allowedDeviation;
    ss_allowedDeviation << ((float)allowedDeviation);
    string allowedDeviation_str(ss_allowedDeviation.str());

    ostringstream ss_casesNum;
    ss_casesNum << ((float)casesNum);
    string casesNum_str(ss_casesNum.str());

    string fileName = (string("task_sets_[n_") + n_str + "_U_" + U_str + "_Umax_" + Umax_str + "_Umin_" + Umin_str
                       + "_Pmin_LB_" + Pmin_LB_str + "_Pmin_UB_" + Pmin_UB_str + "_Pratio_" + Pratio_str
                       + "_allowedDeviation_" + allowedDeviation_str + "_casesNum_" + casesNum_str + "].tsv");
    
    return fileName;
}


int main() {
    int n;
    float U;
    float Umax;
    float Umin = 0;
    float Pratio;
    int Pmin_LB;
    int Pmin_UB;
    float allowedDeviation;
    int casesNum;
    
    cout << "n?\n> ";
    cin >> n;
    cout << "U?\n> ";
    cin >> U;
    cout << "Umax?\n> ";
    cin >> Umax;
    cout << "Pmax/Pmin?\n> ";
    cin >> Pratio;
    cout << "Pmin_LB?\n> ";
    cin >> Pmin_LB;
    cout << "Pmin_UB?\n> ";
    cin >> Pmin_UB;
    cout << "Allowed deviation?\n> ";
    cin >> allowedDeviation;
    cout << "Number of cases to generate?\n> ";
    cin >> casesNum;
    
    string fileName_string = generate_file_name(n, U, Umax, Umin, Pmin_LB, Pmin_UB, Pratio, allowedDeviation,
                                                casesNum);
    generate_serie(fileName_string, casesNum, n, U, Umax, Umin, Pmin_LB, Pmin_UB, Pratio, allowedDeviation);

    return 0;
}
