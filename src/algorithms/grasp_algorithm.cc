// University of La Laguna
// School of Engineering and Technology
// Bachelor's Degree in Computer Engineering
// Subject: Design and Analysis of Algorithms
// Course: 3rd
// Practice 6: Maximum Diversity Problem
// Author: Javier Acosta Portocarrero
// Date: 04/05/2026
// File grasp-algorithm.cc: implementation file.
// Contains the implementation of the GraspAlgorithm, class.

#include <stdexcept>

#include "grasp_algorithm.h"

/**
 * @brief Solves the problem using the GRASP metaheuristic.
 *
 * The method implements the template pattern, where the specific steps
 * of the algorithm are defined by the concrete implementations of the class.
 *
 * @param input Pointer to the problem instance to solve.
 * @return A pointer to the best solution found for the given instance.
 */
Solution* GraspAlgorithm::Solve(Instance* input) {
  if (input == nullptr) {
    throw std::invalid_argument("Null instance.");
  }
  
  Preprocess(input);
  Solution* best_solution = nullptr;
  do {
    Solution* current_solution = ConstructSolution(input);
    if (current_solution == nullptr) {
      throw std::runtime_error("ConstructSolution returned null, not feasible solution could be found.");
    }
    // Postprocess is responsuble for freeing the original (current) solution if needed.
    current_solution = Postprocess(current_solution);
    if (current_solution == nullptr) {
      throw std::runtime_error("Postprocess returned null.");
    }
    // UpdateBest is responsible for freeing the current solution if it is not better than the best one.
    UpdateBest(current_solution, best_solution);
  } while (!StopCriterion());

  return best_solution;
}