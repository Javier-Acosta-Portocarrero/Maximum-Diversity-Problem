// University of La Laguna
// School of Engineering and Technology
// Bachelor's Degree in Computer Engineering
// Subject: Design and Analysis of Algorithms
// Course: 3rd
// Practice 6: Maximum Diversity Problem
// Author: Javier Acosta Portocarrero
// Date: 02/05/2026
// File branch_and_bound_maximum_diversity_lowest_bound_algorithm.h: declaration file.
// Contains the declaration of the BranchAndBoundMaximumDiversityLowestBoundAlgorithm class.

#ifndef BRANCH_AND_BOUND_MAXIMUM_DIVERSITY_ALGORITHM_LOWEST_BOUND_H_
#define BRANCH_AND_BOUND_MAXIMUM_DIVERSITY_ALGORITHM_LOWEST_BOUND_H_

#include "../solutions/maximum_diversity_solution.h"
#include "branch_and_bound_maximum_diversity_algorithm.h"

class BranchAndBoundMaximumDiversityLowestBoundAlgorithm : public BranchAndBoundMaximumDiversityAlgorithm {
 public:
  BranchAndBoundMaximumDiversityLowestBoundAlgorithm() = default;
  ~BranchAndBoundMaximumDiversityLowestBoundAlgorithm() = default;

 protected:
  void AddNewBranch(std::vector<std::pair<std::vector<bool>, double>>& open_branches, const std::vector<bool>& new_solution, 
                    double new_branch_bound, const MaximumDiversitySolution& solution) override;
};

#endif