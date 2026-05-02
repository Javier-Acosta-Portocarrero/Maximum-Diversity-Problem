// University of La Laguna
// School of Engineering and Technology
// Bachelor's Degree in Computer Engineering
// Subject: Design and Analysis of Algorithms
// Course: 3rd
// Practice 6: Maximum Diversity Problem
// Author: Javier Acosta Portocarrero
// Date: 02/05/2026
// File branch_and_bound_maximum_diversity_lowest_bound_algorythm.cc: implementation file.
// Contains the implementation of the BranchAndBoundMaximumDiversitGreatestBoundAlgorithm class.

#include "branch_and_bound_maximum_diversity_greatest_bound_algorithm.h"

/**
 * @brief Adds a new branch to the list of open branches, maintaining the order based on the branch bound, highest first.
 *
 * @param open_branches The list of open branches to which to add the new branch.
 * @param new_solution The solution for the new branch.
 * @param new_branch_bound The bound for the new branch.
 * @param solution The current maximum diversity solution.
 */
void BranchAndBoundMaximumDiversityGreatestBoundAlgorithm::AddNewBranch(std::vector<std::pair<std::vector<bool>, double>>& open_branches, const std::vector<bool>& new_solution, 
                                                                      double new_branch_bound, const MaximumDiversitySolution& solution) {
  
  for (size_t i = 0; i < open_branches.size(); ++i) {
    if (open_branches[i].second < new_branch_bound) {
      open_branches.insert(open_branches.begin() + i, {new_solution, new_branch_bound});
      return;
    }
  }
  open_branches.push_back({new_solution, new_branch_bound});
}
