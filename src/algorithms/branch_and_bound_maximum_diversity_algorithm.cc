// University of La Laguna
// School of Engineering and Technology
// Bachelor's Degree in Computer Engineering
// Subject: Design and Analysis of Algorithms
// Course: 3rd
// Practice 6: Maximum Diversity Problem
// Author: Javier Acosta Portocarrero
// Date: 02/05/2026
// File branch_and_bound_maximum_diversity_algorithm.cc: implementation file.
// Contains the implementation of the BranchAndBoundMaximumDiversityAlgorithm class.

#include <algorithm>
#include <functional>
#include <iostream>
#include <stdexcept>

#include "branch_and_bound_maximum_diversity_algorithm.h"

/**
 * @brief Solves the Maximum Diversity Problem using a branch and bound algorithm.
 *
 * @param initial_solution Initial solution to start the algorithm.
 * @param subset_size Number of elements to select.
 * @return A pointer to the best solution found for the given instance.
 */
MaximumDiversitySolution* BranchAndBoundMaximumDiversityAlgorithm::Solve(const MaximumDiversitySolution& initial_solution, unsigned subset_size) {
  if (subset_size != initial_solution.GetSubsetSize()) {
    throw std::invalid_argument("Invalid subset size.");
  }
  amount_of_branches_explored_ = 0;
  amount_of_branches_generated_ = 0;
  amount_of_branches_bound_ = 0;

  current_lower_bound_ = initial_solution.GetObjectiveValue();  
  current_best_solution_ = initial_solution.GetSelectedElements();
  std::vector<std::pair<std::vector<bool>, double>> open_branches;
  std::vector<bool> first_branch = {false};
  std::vector<bool> second_branch = {true};
  double first_branch_upper_bound = EvaluateBranchBound(first_branch, 0.0, initial_solution);
  double second_branch_upper_bound = EvaluateBranchBound(second_branch, 0.0, initial_solution);
  if (first_branch_upper_bound > current_lower_bound_) {
    AddNewBranch(open_branches, first_branch, first_branch_upper_bound, initial_solution);
  } else {
    ++amount_of_branches_bound_;
  }
  if (second_branch_upper_bound > current_lower_bound_) {
    AddNewBranch(open_branches, second_branch, second_branch_upper_bound, initial_solution);
  } else {
    ++amount_of_branches_bound_;
  }
  amount_of_branches_generated_ += 2;

  while (!open_branches.empty()) {
    std::vector<bool> current_solution = open_branches[0].first;
    double current_solution_bound = open_branches[0].second;
    open_branches.erase(open_branches.begin());
    // This should not happen since we bound the branches when adding them, but we check it just in case.
    if (current_solution_bound <= current_lower_bound_) {
      continue;
    }
    ++amount_of_branches_explored_;

    std::vector<bool> first_new_branch = current_solution;
    std::vector<bool> second_new_branch = current_solution;
    first_new_branch.push_back(false);
    second_new_branch.push_back(true);
    double first_new_branch_upper_bound = EvaluateBranchBound(first_new_branch, current_solution_bound, initial_solution);
    double second_new_branch_upper_bound = EvaluateBranchBound(second_new_branch, current_solution_bound, initial_solution);
    amount_of_branches_generated_ += 2;
    
    if (first_new_branch_upper_bound > current_lower_bound_) {
      int selected_count = 0;
      for (size_t i = 0; i < first_new_branch.size(); ++i) {
        if (first_new_branch[i]) {
          ++selected_count;
        }
      }
      // If the new branch is a complete solution and it is better than the current lower bound, we update the best solution and the lower bound.
      if (selected_count == static_cast<int>(subset_size)) {
        current_lower_bound_ = first_new_branch_upper_bound;
        std::vector<bool> completed_solution = first_new_branch;
        completed_solution.resize(initial_solution.GetElementCount(), false);
        current_best_solution_ = completed_solution;
        BoundBranchsIfNeeded(open_branches);
      } else if (first_new_branch.size() < static_cast<size_t>(initial_solution.GetElementCount())) {
        AddNewBranch(open_branches, first_new_branch, first_new_branch_upper_bound, initial_solution);
      } else {
        ++amount_of_branches_bound_;
      }
    } else {
      ++amount_of_branches_bound_;
    }

    if (second_new_branch_upper_bound > current_lower_bound_) {
      int selected_count = 0;
      for (size_t i = 0; i < second_new_branch.size(); ++i) {
        if (second_new_branch[i]) {
          ++selected_count;
        }
      }
      // If the new branch is a complete solution and it is better than the current lower bound, we update the best solution and the lower bound.
      if (selected_count == static_cast<int>(subset_size)) {
        current_lower_bound_ = second_new_branch_upper_bound;
        std::vector<bool> completed_solution = second_new_branch;
        // Fill the rest of the solution with false to make it a complete solution if its not yet.
        completed_solution.resize(initial_solution.GetElementCount(), false);
        current_best_solution_ = completed_solution;
        BoundBranchsIfNeeded(open_branches);
      } else if (second_new_branch.size() < static_cast<size_t>(initial_solution.GetElementCount())) {
        AddNewBranch(open_branches, second_new_branch, second_new_branch_upper_bound, initial_solution);
      } else {
        ++amount_of_branches_bound_;
      }
    } else {
      ++amount_of_branches_bound_;
    }
  }

  // Basic additional information about the branch and bound execution
  // std::cout << "Branch and Bound: Explored " << amount_of_branches_explored_ << " branches, generated " << amount_of_branches_generated_ << " branches, bounded " << amount_of_branches_bound_ << " branches" << std::endl;
  std::cout << "Branch and Bound: Explored " << amount_of_branches_explored_ << " branches, generated " << amount_of_branches_generated_ << " branches" << std::endl;
  return new MaximumDiversitySolution(current_best_solution_, initial_solution.GetInstance(), initial_solution.GetSubsetSize());
}

/**
 * @brief Evaluates the upper bound for a given branch in the search tree.
 *
 * @param new_solution The solution for which to evaluate the bound.
 * @param previous_bound The bound from the previous level.
 * @param solution The current maximum diversity solution.
 * @return The evaluated upper bound for the given branch.
 */
double BranchAndBoundMaximumDiversityAlgorithm::EvaluateBranchBound(const std::vector<bool>& new_solution, double previous_bound, const MaximumDiversitySolution& solution) const {
  const MaximumDiversityInstance& instance = solution.GetInstance();
  int element_count = solution.GetElementCount();
  int subset_size = solution.GetSubsetSize();
  int selected_count = 0;
  for (size_t i = 0; i < new_solution.size(); ++i) {
    if (new_solution[i]) {
      ++selected_count;
    }
  }

  int remaining_elements = element_count - static_cast<int>(new_solution.size());
  int remaining_to_select = subset_size - selected_count;
  if (remaining_to_select < 0 || remaining_to_select > remaining_elements) {
    return -1.0;
  }

  double current_diversity = 0.0;
  for (int i = 0; i < static_cast<int>(new_solution.size()); ++i) {
    if (!new_solution[i]) {
      continue;
    }
    for (int j = i + 1; j < static_cast<int>(new_solution.size()); ++j) {
      if (new_solution[j]) {
        current_diversity += instance.GetDistance(i, j);
      }
    }
  }

  std::vector<double> possible_contributions;
  for (int i = 0; i < static_cast<int>(new_solution.size()); ++i) {
    if (!new_solution[i]) {
      continue;
    }
    for (int j = static_cast<int>(new_solution.size()); j < element_count; ++j) {
      possible_contributions.push_back(instance.GetDistance(i, j));
    }
  }
  for (int i = static_cast<int>(new_solution.size()); i < element_count; ++i) {
    for (int j = i + 1; j < element_count; ++j) {
      possible_contributions.push_back(instance.GetDistance(i, j));
    }
  }

  std::sort(possible_contributions.begin(), possible_contributions.end(), std::greater<double>());
  int maximum_extra_pairs = selected_count * remaining_to_select + remaining_to_select * (remaining_to_select - 1) / 2;
  double upper_bound = current_diversity;
  for (int i = 0; i < maximum_extra_pairs && i < static_cast<int>(possible_contributions.size()); ++i) {
    upper_bound += possible_contributions[i];
  }

  return upper_bound;
}

/**
 * @brief Bounds the open branches if needed based on the current lower bound.
 *
 * @param open_branches The list of open branches to consider.
 */
void BranchAndBoundMaximumDiversityAlgorithm::BoundBranchsIfNeeded(std::vector<std::pair<std::vector<bool>, double>>& open_branches) {
  for (int i = static_cast<int>(open_branches.size()) - 1; i >= 0; --i) {
    if (open_branches[i].second <= current_lower_bound_) {
      ++amount_of_branches_bound_;
      open_branches.erase(open_branches.begin() + i);
    }
  }
}