#include <iostream>
#include <set>
//#include <time.h>
#include <ilcplex/ilocplex.h>
#include <ctime>

using namespace std;


const float EPS = 0.0001; // 0.01
const int NMAX = 16;

int Cs[NMAX];
int Ps[NMAX];


void sort_periods(const int n) {
    int x;
    for (int j = n-1; j >= 0; j--) {
        for (int i = 0; i <= j; i++) {
            if (i < j) {
                if (Ps[i] > Ps[i+1]) {
                    x = Ps[i];
                    Ps[i] = Ps[i+1];
                    Ps[i+1] = x;
                }
            }
        }
    }
}


void generate_periods_RM(const int n, const int Pmin_LB, const int Pmin_UB, const float Pmaxmin) {
    srand(std::clock());

    // Generating Pmin
    int Pmin;
    if (Pmin_UB - Pmin_LB > 0) Pmin = rand() % (Pmin_UB - Pmin_LB) + Pmin_LB;
    else Pmin = Pmin_LB;
    
    // and Pmax
    int Pmax = floor((float)Pmin*Pmaxmin);
    
    Ps[0] = Pmin;
    for (int i = 1; i < n-1; i++){
        Ps[i] = Pmin + (int)rand() % (Pmax - Pmin);
    }
    Ps[n-1] = Pmax;
    
    sort_periods(n);
}


void generate_periods_notRM(const int n, const int Pmin_LB, const int Pmin_UB, const float Pmaxmin) {
    srand(std::clock());
    
    unsigned short i_forPmin = rand() % n;
    unsigned short i_forPmax = rand() % n;
    if (i_forPmax == i_forPmin) {
        if (i_forPmin == n-1) i_forPmin -= 1;
        if (i_forPmax == 0) i_forPmax += 1;
    }
    
    // Generating Pmin
    int Pmin;
    if (Pmin_UB - Pmin_LB > 0) Pmin = rand() % (Pmin_UB - Pmin_LB) + Pmin_LB;
    else Pmin = Pmin_LB;
    
    // and Pmax
    int Pmax = floor((float)Pmin*Pmaxmin);
    
    Ps[i_forPmin] = Pmin;
    Ps[i_forPmax] = Pmax;
    
    for (int i = 0; i < n; i++){
        if ((i == i_forPmin) || (i == i_forPmax)) continue;
        else Ps[i] = Pmin + (int)rand() % (Pmax - Pmin);
    }
}


/*bool generate_execs_with_Umin(const int n, const float Utotal, const float Umax, const float Umin) {
    
    if (Utotal < 1) allowedDeviation = 0.4;
    else allowedDeviation = 0.2;

    ILOSTLBEGIN
    IloEnv env;
    IloModel model(env);
    IloIntVarArray Cvars(env);
    IloCplex cplex(model);
    
    bool generated = false;
    
    cplex.setOut(env.getNullStream());
    
    try {
        // task with Umax
        srand(std::clock());
        int iUmax = rand() % (n);
        int iUmin = iUmax;
        while (iUmin == iUmax) {iUmin = rand() % (n-1);}
        
        // create Cs, Ts variables
        for (int i = 0; i < n; i++) {
            IloIntVar C_i(env);
            Cvars.add(C_i);
            model.add(Cvars[i] >= EPS);
            if (i != iUmax) model.add(Cvars[i]/Ps[i] <= Umax);
            model.add(Cvars[i]/Ps[i] >= Umin);
            if (i == iUmax) model.add(Cvars[i]/Ps[i] >= (1-allowedDeviation)*Umax);
            if (i == iUmin) model.add(Cvars[i]/Ps[i] <= (1+allowedDeviation)*Umin);
        }
        
        // total utilization
        IloNumExpr U(env);
        for (int i = 0; i < n; i++) {
            U += Cvars[i]/Ps[i];
        }
        model.add(U <= Utotal);
        model.add(U >= (1-allowedDeviation)*Utotal);
        model.add(IloMinimize(env, (Utotal-U)/Utotal + (Umax - Cvars[iUmax]/Ps[iUmax])/Umax + (Umin + Cvars[iUmin]/Ps[iUmin])/Umin));
        
        if (cplex.solve()) {
            generated = true;
            //cout << "Utotal: " << cplex.getValue(U) << endl;
            //cout << "Deviation: " << 1 - cplex.getValue(U)/Utotal << endl;
            cout << "Umax: " << cplex.getValue(Cvars[iUmax]/Ps[iUmax]) << endl;
            cout << "Umin: " << cplex.getValue(Cvars[iUmin]/Ps[iUmin]) << endl;
            //cout << "Deviation: " << 1 - cplex.getValue(Cvars[iUmax]/Ps[iUmax])/Umax << endl;
            for (int i = 0; i < n; i++) Cs[i] = (int)cplex.getValue(Cvars[i]);
            
        } else {
            //cout << "Failed to generate task set" << endl;
        }
    } catch (IloException e) {
        cout << e << endl;
    }
    
    cplex.end();
    model.end();
    env.end();
    
    return generated;
}*/


bool generate_execs(const int n, const float Utotal, const float Umax, const float Umin, const float allowedDeviation) {
    ILOSTLBEGIN
    IloEnv env;
    IloModel model(env);
    IloIntVarArray Cvars(env);
    IloCplex cplex(model);
    
    bool generated = false;
    
    cplex.setOut(env.getNullStream());
    
    try {
        srand(std::clock());
        
        int iUmax = rand() % (n);
        if (iUmax == n) return false; // to avoid the problem with rand(): never takes max int. value by some reason
        
        // create Cs, Ts variables
        for (int i = 0; i < n; i++) {
            IloIntVar C_i(env);
            Cvars.add(C_i);
            model.add(Cvars[i] >= EPS); //cout << "C[" << i << "] >= " << EPS << endl;
            if (i != iUmax) {model.add(Cvars[i]/Ps[i] <= Umax);} //cout << "u[" << i << "] <= " << Umax << endl;}
            if (i == iUmax) {
                //cout << "adding constraint for Umax" << endl;
                model.add(Cvars[i]/Ps[i] >= (1-allowedDeviation/2)*Umax); //cout << "C[i]/P[i] >= " << Umax << endl;
                model.add(Cvars[i]/Ps[i] <= (1+allowedDeviation/2)*Umax); //cout << "C[i]/P[i] <= " << (1+allowedDeviation)*Umax << endl;
            }
        }
        
        // total utilization
        IloNumExpr U(env);
        for (int i = 0; i < n; i++) {
            U += Cvars[i]/Ps[i];
        }
        model.add(U <= Utotal);
        model.add(U >= (1-allowedDeviation)*Utotal);
        model.add(IloMinimize(env, (Utotal-U)/Utotal + (Umax - Cvars[iUmax]/Ps[iUmax])/Umax));
        
        if (cplex.solve()) {
            generated = true;
            //cout << "Utotal: " << cplex.getValue(U) << endl;
            //cout << "Deviation: " << 1 - cplex.getValue(U)/Utotal << endl;
            //cout << "Umax: " << cplex.getValue(Cvars[iUmax]/Ps[iUmax]) << endl;
            //cout << "Deviation: " << 1 - cplex.getValue(Cvars[iUmax]/Ps[iUmax])/Umax << endl;
            for (int i = 0; i < n; i++) Cs[i] = (int)cplex.getValue(Cvars[i]);
        }
    } catch (IloException e) {
        cout << e << endl;
    }
    
    cplex.end();
    model.end();
    env.end();
    
    return generated;
}


void generate_taskset(const int n, const float Utotal, const float Umax, const float Umin, const int Pmin_LB,
                      const int Pmin_UB, const float Pmaxmin, const float allowedDeviation, const bool isRM,
                      int (&Cs1)[NMAX], int (&Ps1)[NMAX]) {
    bool generated = false;
    
    while (!generated) {
        if (isRM) generate_periods_RM(n, Pmin_LB, Pmin_UB, Pmaxmin);
        else generate_periods_notRM(n, Pmin_LB, Pmin_UB, Pmaxmin);
        
        generated = generate_execs(n, Utotal, Umax, Umin, allowedDeviation);
    }
    
    // print resulted taskset
    for (int i = 0; i < n; i++) {
        Cs1[i] = Cs[i];
        Ps1[i] = Ps[i];
    }
}
