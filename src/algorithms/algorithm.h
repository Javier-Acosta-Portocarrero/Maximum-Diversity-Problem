// University of La Laguna
// School of Engineering and Technology
// Bachelor's Degree in Computer Engineering
// Subject: Design and Analysis of Algorithms
// Course: 3rd
// Practice 6: Maximum Diversity Problem
// Author: Javier Acosta Portocarrero
// Date: 04/04/2026
// File algorithm.h: declaration file.
// Contains the declaration of the Algorithm, class.

#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#include "../instances/instance.h"
#include "../solutions/solution.h"

/**
  * @class Algorithm
  * 
  * @brief Abstract interface class for algorithms, which receive an instance
  *        as input and produce a solution as output. It also follows
  *        the strategy pattern.  
  */
class Algorithm {
 public:

  virtual ~Algorithm() = default; 
 /**
   * @brief Pure virtual method to solve any type of algorithm
   *
   * @param input Pointer to any possible instance that an algorithm can receive
   * @return A pointer to the solution of the problem for that specific instance,
   *         it can be any type of solution that an algorithm can return.
   */
  virtual Solution* Solve(Instance* input) = 0;
};

#endif