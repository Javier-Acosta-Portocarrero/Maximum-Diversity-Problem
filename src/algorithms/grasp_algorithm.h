// University of La Laguna
// School of Engineering and Technology
// Bachelor's Degree in Computer Engineering
// Subject: Design and Analysis of Algorithms
// Course: 3rd
// Practice 6: Maximum Diversity Problem
// Author: Javier Acosta Portocarrero
// Date: 04/05/2026
// File grasp-algorithm.h: declaration file.
// Contains the declaration of the GraspAlgorithm, class.

#ifndef GRAASP_ALGORITHM_H_
#define GRAASP_ALGORITHM_H_

#include "algorithm.h"

/**
  * @class GraspAlgorithm
  * 
  * @brief Abstract class that implements the GRASP metaheuristic, it follows the template method pattern.
  *        The concrete implementations are responsible for defining the specific steps of the algorithm.
  */
class GraspAlgorithm : public Algorithm {
 public:
  Solution* Solve(Instance* input) override;

 protected:
  virtual void Preprocess(Instance* input) = 0;
  virtual Solution* ConstructSolution(Instance* input) = 0;
  virtual Solution* Postprocess(Solution* solution) = 0;
  virtual void UpdateBest(Solution* current, Solution*& best) = 0;
  virtual bool StopCriterion() = 0;
};

#endif