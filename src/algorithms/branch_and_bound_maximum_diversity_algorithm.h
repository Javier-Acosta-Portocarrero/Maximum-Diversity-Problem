// University of La Laguna
// School of Engineering and Technology
// Bachelor's Degree in Computer Engineering
// Subject: Design and Analysis of Algorithms
// Course: 3rd
// Practice 6: Maximum Diversity Problem
// Author: Javier Acosta Portocarrero
// Date: 02/05/2026
// File branch_and_bound_maximum_diversity_algorythm.h: declaration file.
// Contains the declaration of the BranchAndBoundMaximumDiversityAlgorithm class.

#ifndef BRANCH_AND_BOUND_MAXIMUM_DIVERSITY_ALGORITHM_H_
#define BRANCH_AND_BOUND_MAXIMUM_DIVERSITY_ALGORITHM_H_

#include "../solutions/maximum_diversity_solution.h"

class BranchAndBoundMaximumDiversityAlgorithm {
 public:
  BranchAndBoundMaximumDiversityAlgorithm() = default;
  ~BranchAndBoundMaximumDiversityAlgorithm() = default;

  MaximumDiversitySolution* Solve(const MaximumDiversitySolution& initial_solution, unsigned subset_size);
 protected:
  double current_lower_bound_ = 0.0;
  std::vector<bool> current_best_solution_;
  unsigned amount_of_branches_explored_ = 0;
  unsigned amount_of_branches_generated_ = 0;
  unsigned amount_of_branches_bound_ = 0;

  void BoundBranchsIfNeeded(std::vector<std::pair<std::vector<bool>, double>>& open_branches);
  double EvaluateBranchBound(const std::vector<bool>& new_solution, const MaximumDiversitySolution& solution) const;
  virtual void AddNewBranch(std::vector<std::pair<std::vector<bool>, double>>& open_branches, const std::vector<bool>& new_solution, 
                            double new_branch_bound, const MaximumDiversitySolution& solution) = 0;
};
#endif