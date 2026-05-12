# DAA Practice - Branch and Bound

# Maximum Diversity Problem

Implementation of a B&B algorithm to solve the Maximum Diversity problem.


## Project Directory Structure

The project is fully implemented in C++ and organized in a modular way:

```text
.
├── Makefile
├── README.md
├── inputs
│   └── max_div_XX.txt              # Problem instances
├── outputs
│   ├── *.txt                   # Execution results and experiments
└── src
    ├── algorythms              # Solvers 
    ├── explorers               # Neighborhood structures
    ├── instances               # Instance parsing and representation
    ├── solutions               # Solution representation
    └── main.cc                 # Entry point
```

## Implemented Algorithms

The following approaches have been implemented:

* Greedy Solver
* GRASP + VND (only with 1 neighborhood)
* Branch and Bound (allowing to select de algorithm used to generate the initial solution)

## Building and Running the code

To compile the project, from the root directory run ```make```

To run an instance:

```bash
./practice6_DAA inputs/max_div_15_2.txt bnb-greatest 5


## Author

Javier Acosta Portocarrero
alu0101660769@ull.edu.es / javier.acosta.porto@gmail.com (personal email)