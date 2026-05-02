// University of La Laguna
// School of Engineering and Technology
// Bachelor's Degree in Computer Engineering
// Subject: Design and Analysis of Algorithms
// Course: 3rd
// Practice 6: Maximum Diversity Problem
// Author: Javier Acosta Portocarrero
// Date: 02/05/2026
// File maximum_diversity_solution.cc: implementation file.
// Contains the implementation of the MaximumDiversitySolution class.

#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>

#include "maximum_diversity_solution.h"

namespace {
  constexpr double kCostTolerance = 1e-8;

  double ClampNearZero(double value, double tolerance = kCostTolerance) {
    if (std::fabs(value) <= tolerance) {
      return 0.0;
    }
    return value;
  }
}

/**
 * @brief Builds a solution from a selected element vector.
 *
 * @param selected_elements Binary vector that indicates which elements belong to the solution.
 * @param instance Problem instance associated with the solution.
 * @param subset_size Size of the subset to select.
 *
 * @throw std::invalid_argument If the selected vector is not valid.
 */
MaximumDiversitySolution::MaximumDiversitySolution(const std::vector<bool>& selected_elements, const MaximumDiversityInstance& instance,
    unsigned subset_size)  : instance_(instance), subset_size_(subset_size) {

  int element_count = instance_.GetElementCount();
  if (subset_size_ == 0 || subset_size_ >= static_cast<unsigned>(element_count)) {
    throw std::invalid_argument("Invalid subset size.");
  }
  if (static_cast<int>(selected_elements.size()) != element_count) {
    throw std::invalid_argument(
        "Selected elements vector size does not match the number of elements in the instance.");
  }
  selected_elements_ = selected_elements;
  elements_in_solution_.clear();
  contribution_by_element_.assign(element_count, 0.0);

  int selected_count = 0;
  for (int i = 0; i < element_count; ++i) {
    if (selected_elements_[i]) {
      ++selected_count;
      AddElementToSolutionList(i);
      IncreaseContributionCounters(i);
    }
  }
  if (selected_count != static_cast<int>(subset_size_)) {
    throw std::invalid_argument(
        "The number of selected elements does not match the subset size.");
  }

  diversity_ = ComputeObjectiveFromScratch();
}

/**
 * @brief Builds an empty solution for a given instance and subset size.
 *
 * @param instance Problem instance associated with the solution.
 * @param subset_size Number of elements to select.
 */
MaximumDiversitySolution::MaximumDiversitySolution(
    const MaximumDiversityInstance& instance, unsigned subset_size)
    : instance_(instance),
      subset_size_(subset_size) {
  int element_count = instance_.GetElementCount();
  selected_elements_.assign(element_count, false);
  contribution_by_element_.assign(element_count, 0.0);
}

/**
 * @brief Returns the associated instance.
 *
 * @return Constant reference to the instance.
 */
const MaximumDiversityInstance& MaximumDiversitySolution::GetInstance() const {
  return instance_;
}

/**
 * @brief Returns the number of elements.
 *
 * @return Number of elements.
 */
int MaximumDiversitySolution::GetElementCount() const {
  return instance_.GetElementCount();
}

/**
 * @brief Returns the dimension of the elements.
 *
 * @return Dimension of the elements.
 */
int MaximumDiversitySolution::GetDimension() const {
  return instance_.GetDimension();
}

/**
 * @brief Returns the required subset size.
 *
 * @return Subset size.
 */
unsigned MaximumDiversitySolution::GetSubsetSize() const {
  return subset_size_;
}

/**
 * @brief Returns the current number of selected elements.
 *
 * @return Number of selected elements.
 */
int MaximumDiversitySolution::GetSelectedElementCount() const {
  return static_cast<int>(elements_in_solution_.size());
}

/**
 * @brief Returns the selected elements vector.
 *
 * @return Constant reference to the selected elements vector.
 */
const std::vector<bool>& MaximumDiversitySolution::GetSelectedElements() const {
  return selected_elements_;
}

/**
 * @brief Returns the list of elements in the solution.
 *
 * @return Constant reference to the selected element list.
 */
const std::vector<int>& MaximumDiversitySolution::GetElementsInSolution() const {
  return elements_in_solution_;
}

/**
 * @brief Returns the contribution vector.
 *
 * @return Constant reference to the contribution vector.
 */
const std::vector<double>& MaximumDiversitySolution::GetContributionByElement() const {
  return contribution_by_element_;
}

/**
 * @brief Returns the insertion or removal contribution of an element.
 *
 * If the element is outside the solution, this value is the diversity increase
 * produced by inserting it. If it is inside the solution, this value is the
 * diversity decrease produced by removing it.
 *
 * @param element_id Element identifier.
 * @return Contribution of the element with respect to the current solution.
 *
 * @throw std::out_of_range If the element identifier is not valid.
 */
double MaximumDiversitySolution::GetContributionOfElement(int element_id) const {
  if (!IsValidElementId(element_id)) {
    throw std::out_of_range("Invalid element id.");
  }

  return contribution_by_element_[element_id];
}

/**
 * @brief Adds an element to the solution.
 *
 * @param element_id Element identifier.
 * @return True if the operation is performed, false otherwise.
 */
bool MaximumDiversitySolution::AddElement(int element_id) {
  if (!CanAddElement(element_id)) {
    return false;
  }

  diversity_ += contribution_by_element_[element_id];
  diversity_ = ClampNearZero(diversity_);
  selected_elements_[element_id] = true;
  AddElementToSolutionList(element_id);
  IncreaseContributionCounters(element_id);

  return true;
}

/**
 * @brief Removes an element from the solution.
 *
 * @param element_id Element identifier.
 * @return True if the operation is performed, false otherwise.
 */
bool MaximumDiversitySolution::RemoveElement(int element_id) {
  if (!CanRemoveElement(element_id)) {
    return false;
  }

  diversity_ -= contribution_by_element_[element_id];
  diversity_ = ClampNearZero(diversity_);
  selected_elements_[element_id] = false;
  DecreaseContributionCounters(element_id);
  RemoveElementFromSolutionList(element_id);

  return true;
}

/**
 * @brief Removes all elements from the solution.
 */
void MaximumDiversitySolution::Clear() {
  std::fill(selected_elements_.begin(), selected_elements_.end(), false);
  elements_in_solution_.clear();
  std::fill(contribution_by_element_.begin(), contribution_by_element_.end(), 0.0);
  diversity_ = 0.0;
}

/**
 * @brief Checks whether an element is selected.
 *
 * @param element_id Element identifier.
 * @return True if the element is selected, false otherwise.
 */
bool MaximumDiversitySolution::IsElementSelected(int element_id) const {
  if (!IsValidElementId(element_id)) {
    throw std::out_of_range("Invalid element id.");
  }

  return selected_elements_[element_id];
}

/**
 * @brief Checks whether an element can be added to the solution.
 *
 * @param element_id Element identifier.
 * @return True if the element can be added, false otherwise.
 */
bool MaximumDiversitySolution::CanAddElement(int element_id) const {
  if (!IsValidElementId(element_id)) {
    return false;
  }
  if (selected_elements_[element_id]) {
    return false;
  }
  if (GetSelectedElementCount() >= subset_size_) {
    return false;
  }

  return true;
}

/**
 * @brief Checks whether an element can be removed from the solution.
 *
 * @param element_id Element identifier.
 * @return True if the element can be removed, false otherwise.
 */
bool MaximumDiversitySolution::CanRemoveElement(int element_id) const {
  if (!IsValidElementId(element_id)) {
    return false;
  }
  if (!selected_elements_[element_id]) {
    return false;
  }

  return true;
}

/**
 * @brief Checks whether the whole solution is feasible.
 *
 * The method verifies:
 * - exactly m selected elements,
 * - consistency between the binary vector and the selected element list,
 * - consistency of the objective value.
 *
 * @param tolerance Numerical tolerance.
 * @return True if the solution is feasible, false otherwise.
 */
bool MaximumDiversitySolution::IsFeasible(double tolerance) const {
  if (subset_size_ <= 0 || subset_size_ >= GetElementCount()) {
    std::cerr << "Invalid subset size." << std::endl;
    return false;
  }
  if (GetSelectedElementCount() != subset_size_) {
    std::cerr << "The solution has " << GetSelectedElementCount()
              << " selected elements instead of " << subset_size_ << "." << std::endl;
    return false;
  }
  if (!CheckListConsistency()) {
    std::cerr << "Inconsistency between selected vector and selected list." << std::endl;
    return false;
  }
  if (!CheckObjectiveConsistency(tolerance)) {
    std::cerr << "Inconsistent objective value." << std::endl;
    return false;
  }

  return true;
}

/**
 * @brief Computes the objective value from scratch.
 *
 * @return Recomputed objective value.
 */
double MaximumDiversitySolution::ComputeObjectiveFromScratch() const {
  double diversity = 0.0;
  for (int a = 0; a < static_cast<int>(elements_in_solution_.size()); ++a) {
    for (int b = a + 1; b < static_cast<int>(elements_in_solution_.size()); ++b) {
      diversity += instance_.GetDistance(elements_in_solution_[a], elements_in_solution_[b]);
    }
  }

  return diversity;
}

/**
 * @brief Checks whether the incremental objective matches the recomputed one.
 *
 * @param tolerance Numerical tolerance.
 * @return True if both values match, false otherwise.
 */
bool MaximumDiversitySolution::CheckObjectiveConsistency(double tolerance) const {
  return std::fabs(ComputeObjectiveFromScratch() - diversity_) <= tolerance;
}

/**
 * @brief Checks whether an element identifier is valid.
 *
 * @param element_id Element identifier.
 * @return True if the identifier is valid, false otherwise.
 */
bool MaximumDiversitySolution::IsValidElementId(int element_id) const {
  return element_id >= 0 && element_id < GetElementCount();
}

/**
 * @brief Checks whether an element appears in the solution list.
 *
 * @param element_id Element identifier.
 * @return True if the element is already in the solution list, false otherwise.
 */
bool MaximumDiversitySolution::HasElementInSolution(int element_id) const {
  return std::find(elements_in_solution_.begin(), elements_in_solution_.end(), element_id) !=
         elements_in_solution_.end();
}

/**
 * @brief Adds an element to the selected element list.
 *
 * @param element_id Element identifier.
 */
void MaximumDiversitySolution::AddElementToSolutionList(int element_id) {
  if (!HasElementInSolution(element_id)) {
    elements_in_solution_.push_back(element_id);
  }
}

/**
 * @brief Removes an element from the selected element list.
 *
 * @param element_id Element identifier.
 */
void MaximumDiversitySolution::RemoveElementFromSolutionList(int element_id) {
  elements_in_solution_.erase(
      std::remove(elements_in_solution_.begin(), elements_in_solution_.end(), element_id),
      elements_in_solution_.end());
}

/**
 * @brief Increases contribution counters when an element is selected.
 *
 * @param element_id Element identifier.
 */
void MaximumDiversitySolution::IncreaseContributionCounters(int element_id) {
  for (int i = 0; i < GetElementCount(); ++i) {
    contribution_by_element_[i] += instance_.GetDistance(i, element_id);
    contribution_by_element_[i] = ClampNearZero(contribution_by_element_[i]);
  }
}

/**
 * @brief Decreases contribution counters when an element is removed.
 *
 * @param element_id Element identifier.
 */
void MaximumDiversitySolution::DecreaseContributionCounters(int element_id) {
  for (int i = 0; i < GetElementCount(); ++i) {
    contribution_by_element_[i] -= instance_.GetDistance(i, element_id);
    contribution_by_element_[i] = ClampNearZero(contribution_by_element_[i]);
  }
}

/**
 * @brief Checks consistency between selected vector and selected list.
 *
 * @return True if both structures represent the same solution, false otherwise.
 */
bool MaximumDiversitySolution::CheckListConsistency() const {
  std::vector<bool> found(GetElementCount(), false);
  for (int i = 0; i < static_cast<int>(elements_in_solution_.size()); ++i) {
    int element_id = elements_in_solution_[i];
    if (!IsValidElementId(element_id)) {
      return false;
    }
    if (found[element_id]) {
      return false;
    }
    found[element_id] = true;
  }

  for (int i = 0; i < GetElementCount(); ++i) {
    if (found[i] != selected_elements_[i]) {
      return false;
    }
  }

  return true;
}
