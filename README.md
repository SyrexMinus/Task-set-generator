# Task-set-generator
Generator of real-time task sets.

## Prerequisites
1. Install [IBM ILOG CPLEX Optimization Studio](https://www.ibm.com/products/ilog-cplex-optimization-studio). It contain
CPLEX and Concert software that is used in the project.

2. Edit makefile: specify path to CPLEX and Concert binaries by replacing CPLEXDIR and CONCERTDIR values with 
corresponding paths.
   
3. Compile the tool in terminal:
```
make
```

If `make` produce errors like below, then follow the instructions of clang warnings in console.
```
generate_tasks.cpp:1:10: fatal error: 'iostream' file not found
```

## Usage

0. Make sure that prerequisites are satisfied
   
1. Run the tool in terminal:
```
./generate_tasks
```
   
2. Follow instructions in terminal
    1. First, choose the varying parameter for task system. Notation: 
       + `n` - tasks number,
       + `P` in `Pmax/Pmin` - a minimum time separation between consecutive job releases by a sporadic task,
       + `U` - task set utilization (sum of tasks' utilization rates `U_t`, where `U_t` = 
         (execution_time of a task `C_t` / `P` )), 
       + `Umax` - maximum utilization rate among tasks.
        
    2. Then choose default values of the task system. Notation:
       + `Pmin_LB` - lower bound for minimal `P` among tasks,
       + `Pmin_UB` - upper bound for minimal `P` among tasks. 
       + `Allowed deviation` - size of the deviation of the parameters, value of type float.
    
    3. Specify number of cases to generate.
    
3. Check out randomly generated task sets satisfying given constaints at `task_sets_varying_<param>[<constraints>].tsv`.
   Each row of the file contain parameters of a generated task set in the following order: `n, U, Umax, Pratio, 
   C_1, P_1, C_2, P_2, C_3 ... P_n`, where `Pratio` is `Pmax/Pmin`.
