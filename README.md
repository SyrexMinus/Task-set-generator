# Task-set-generator
Generator of random real-time task sets satisfying given constaints

## Prerequisites
1. Install [IBM ILOG CPLEX Optimization Studio](https://www.ibm.com/products/ilog-cplex-optimization-studio). It contain
CPLEX and Concert software that is used in the project

2. Edit makefile: specify path to CPLEX and Concert binaries by replacing CPLEXDIR and CONCERTDIR values with 
corresponding paths
   
3. Compile the tool in terminal:
```
make
```

If `make` produce errors like
```
generate_tasks.cpp:1:10: fatal error: 'iostream' file not found
```
then follow the instructions in warnings

## Usage

0. Satisfy prerequisites
   
1. Run in terminal:
```
./generate_tasks
```
   
2. Follow instructions in the terminal. Choose values of the task systems:
   1. `n` - integer, tasks number;
   2. `U` - float, task set utilization (sum of tasks' utilization rates `U_t`
      where `U_t` = (execution_time of a task `C_t` / `P` ));
   4. `Umax` - float, maximum utilization rate among tasks;
   3. `Pmax/Pmin` - float. `P` is a minimum time separation between consecutive job releases by a sporadic task;
   5. `Pmin_LB` - integer, lower bound for minimal `P` among tasks;
   6. `Pmin_UB` - integer, upper bound for minimal `P` among tasks; 
   7. `Allowed deviation` - float, size of the deviation of the parameters;
   8. `Number of cases to generate` - integer.
    
3. Use generated task sets at `task_sets_varying_<param>[<constraints>].tsv`.
   Each row contains a generated task set in the format: `n, U, Umax, Pratio, 
   C_1, P_1, C_2, P_2, C_3 ... P_n`, where `Pratio` is `Pmax/Pmin`.
