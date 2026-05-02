// University of La Laguna
// School of Engineering and Technology
// Bachelor's Degree in Computer Engineering
// Subject: Design and Analysis of Algorithms
// Course: 3rd
// Practice 6: Maximum Diversity Problem
// Author: Javier Acosta Portocarrero
// Date: 10/04/2026
// File maximum_diversity_swap_neighborhood_explorer.cc: implementation file.
// Contains the implementation of the MaximumDiversitySwapExplorer class.

#include <iostream>
#include <cmath>

#include "maximum_diversity_swap_neighborhood_explorer.h"

/**
 * @brief Explores the neighborhood of a solution using element swap moves.
 *
 * @param solution Initial solution.
 * @param amount_tol Amount tolerance.
 * @param improvement_tol Improvement tolerance.
 * @return True if an improving movement was performed, false otherwise.
 */
bool MaximumDiversitySwapExplorer::Explore(MaximumDiversitySolution* solution, double amount_tol, double improvement_tol) const {
  int used_element = -1;
  int unused_element = -1;
  double best_delta = 0.0;

  const std::vector<int>& elements_in_solution = solution->GetElementsInSolution();
  for (int i = 0; i < static_cast<int>(elements_in_solution.size()); ++i) {
    const int element_in_solution = elements_in_solution[i];
    for (int element_not_in_solution = 0;
         element_not_in_solution < solution->GetElementCount();
         ++element_not_in_solution) {
      if (solution->IsElementSelected(element_not_in_solution)) {
        continue;
      }
      if (!CanSwapElements(*solution, element_in_solution, element_not_in_solution, amount_tol, improvement_tol)) {
        continue;
      }
      const double delta = EvaluateSwapDelta(*solution, element_in_solution, element_not_in_solution,  amount_tol, improvement_tol);
      if (delta > best_delta) {
        best_delta = delta;
        used_element = element_in_solution;
        unused_element = element_not_in_solution;
      }
    }
  }

  if (used_element != -1 && best_delta > improvement_tol) {
    solution->RemoveElement(used_element);
    solution->AddElement(unused_element);
    return true;
  }
  return false;
}

/**
 * @brief Checks whether two elements can be swapped.
 *
 * @param solution Solution to evaluate.
 * @param first_element_id Element currently selected.
 * @param second_element_id Element currently not selected.
 * @param amount_tol Amount tolerance.
 * @param improvement_tol Improvement tolerance.
 * @return True if the swap can be performed, false otherwise.
 */
bool MaximumDiversitySwapExplorer::CanSwapElements(const MaximumDiversitySolution& solution, int first_element_id, int second_element_id,
                                                   double amount_tol, double improvement_tol) const {
  if (first_element_id == second_element_id) {
    return false;
  }
  if (!solution.GetInstance().IsValidElementId(first_element_id) ||
      !solution.GetInstance().IsValidElementId(second_element_id)) {
    return false;
  }
  if (!solution.IsElementSelected(first_element_id)) {
    return false;
  }
  if (solution.IsElementSelected(second_element_id)) {
    return false;
  }

  return true;
}

/**
 * @brief Evaluates the cost delta of swapping two elements.
 *
 * @param solution Solution to evaluate.
 * @param first_element_id Selected element to remove.
 * @param second_element_id Unselected element to insert.
 * @param amount_tol Amount tolerance.
 * @param improvement_tol Improvement tolerance.
 * @return Cost delta of performing the swap move.
 */
double MaximumDiversitySwapExplorer::EvaluateSwapDelta(const MaximumDiversitySolution& solution, int first_element_id, int second_element_id,
                                                       double amount_tol, double improvement_tol) const {
  const MaximumDiversityInstance& instance = solution.GetInstance();
  double delta = 0.0;
  const std::vector<int>& elements_in_solution = solution.GetElementsInSolution();
  for (int i = 0; i < static_cast<int>(elements_in_solution.size()); ++i) {
    const int element_id = elements_in_solution[i];
    if (element_id == first_element_id) {
      continue;
    }
    const double removed_contribution = instance.GetDistance(first_element_id, element_id);
    const double added_contribution = instance.GetDistance(second_element_id, element_id);

    delta += added_contribution - removed_contribution;
  }

  return delta;
}