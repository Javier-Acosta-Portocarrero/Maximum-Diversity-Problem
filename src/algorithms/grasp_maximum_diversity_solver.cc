// University of La Laguna
// School of Engineering and Technology
// Bachelor's Degree in Computer Engineering
// Subject: Design and Analysis of Algorithms
// Course: 3rd
// Practice 6: Multi-Source Capacitated Facility Location Problem with Customer Incompatibilities
// Author: Javier Acosta Portocarrero
// Date: 04/04/2026
// File grasp-maximum-diversity-solver.cc: implementation file.
// Contains the implementation of the GraspMaximumDiversitySolver class.

#include <vector>
#include <stdexcept>
#include <algorithm>
#include <random>
#include <iostream>
#include <cmath>

#include "grasp_maximum_diversity_solver.h"
#include "../solutions/maximum_diversity_solution.h"
#include "../instances/maximum_diversity_instance.h"

/**
 * @brief Preprocesses the input instance.
 *
 * @param input The input instance to preprocess.
 */
void GraspMaximumDiversitySolver::Preprocess(Instance* input) {
  return;
}

/**
 * @brief Constructs a solution for the input instance.
 *
 * @param input The input instance for which to construct a solution.
 * @return A pointer to the constructed solution.
 */
Solution* GraspMaximumDiversitySolver::ConstructSolution(Instance* input) {
  MaximumDiversityInstance* instance = dynamic_cast<MaximumDiversityInstance*>(input);
  if (instance == nullptr) {
    throw std::invalid_argument("Input instance is not of type MaximumDiversityInstance.");
  }
  MaximumDiversitySolution* constructive_solution = new MaximumDiversitySolution(*instance, num_elements_);
  if (constructive_solution->GetElementCount() == 0) {
    return constructive_solution;
  }

  constructive_solution->AddElement(constructive_solution->GetInstance().GetElementsSortedByDistanceFromGlobalCenter()[0]);
  for (size_t i = 1; i < num_elements_; ++i) {
    std::vector<int> sorted_elements = GetSortedElementsByDistanceOfCurrentSolution(constructive_solution);
    if (sorted_elements.size() == 0) {
      break;
    }
    size_t random_element_index = rand() % std::min(lcr_cardinality_, static_cast<unsigned>(sorted_elements.size()));
    constructive_solution->AddElement(sorted_elements[random_element_index]);
  }

  ++current_grasp_iteration_;
  return constructive_solution;
}

/**
 * @brief Updates the best solution found so far.
 *
 * @param current The current solution to compare against the best.
 * @param best A reference to the best solution found so far.
 */
void GraspMaximumDiversitySolver::UpdateBest(Solution* current, Solution*& best) {
  MaximumDiversitySolution* current_solution = dynamic_cast<MaximumDiversitySolution*>(current);
  MaximumDiversitySolution* best_solution = dynamic_cast<MaximumDiversitySolution*>(best);
  if (current_solution == nullptr) {
    throw std::invalid_argument("Current solution is not of type MaximumDiversitySolution.");
  }
  if (best_solution == nullptr || current_solution->GetObjectiveValue() > best_solution->GetObjectiveValue()) {
    delete best;
    best = new MaximumDiversitySolution(*current_solution);
  } else {
    delete current_solution;
  }
}

/**
 * @brief Checks if the stopping criterion for the GRASP algorithm is met.
 *
 * @return true if the stopping criterion is met, false otherwise.
 */
bool GraspMaximumDiversitySolver::StopCriterion() {
  if (current_grasp_iteration_ >= max_grasp_iterations_) {
    current_grasp_iteration_ = 0;
    return true;
  }
  return false;
}

/**
 * @brief Gets the facilities sorted by their costs for a given customer.
 *
 * @param solution The solution for which to sort facilities.
 * @return A vector containing the facility IDs sorted by their costs.
 */
std::vector<int> GraspMaximumDiversitySolver::GetSortedElementsByDistanceOfCurrentSolution(MaximumDiversitySolution* solution) const {
  std::vector<std::pair<int, double>> elements_with_distance;
  std::vector<double> contribution_by_element = solution->GetContributionByElement();
  for (int element_id = 0; element_id < solution->GetElementCount(); ++element_id) {
    if (solution->IsElementSelected(element_id)) {
      continue;
    }
    elements_with_distance.emplace_back(element_id, contribution_by_element[element_id]);
  }
  std::sort(elements_with_distance.begin(), elements_with_distance.end(),
            [](const std::pair<int, double>& a, const std::pair<int, double>& b) {
              return a.second > b.second;
            });
  std::vector<int> elements_by_distance;
  for (const auto& pair : elements_with_distance) {
    elements_by_distance.push_back(pair.first);
  }

  return elements_by_distance;
}

/**
 * @brief Postprocesses a solution after construction using a VND strategy.
 *
 * @param solution The solution to postprocess.
 * @return A pointer to the postprocessed solution.
 */
Solution* GraspMaximumDiversitySolver::Postprocess(Solution* solution) {
  MaximumDiversitySolution* current_solution = dynamic_cast<MaximumDiversitySolution*>(solution);
  if (current_solution == nullptr) {
    throw std::invalid_argument("Current solution is not of type MaximumDiversitySolution.");
  }
  for (const auto& explorer : neighboorhod_explorers_) {
    bool improvement = true;
    while (improvement) {
      improvement = explorer->Explore(current_solution, GetAmountTolerance(), GetImprovementTolerance());
    }
  }
  return current_solution;
}