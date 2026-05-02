// University of La Laguna
// School of Engineering and Technology
// Bachelor's Degree in Computer Engineering
// Subject: Design and Analysis of Algorithms
// Course: 3rd
// Practice 6: Maximum Diversity Problem
// Author: Javier Acosta Portocarrero
// Date: 02/05/2026
// File maximum_diversity_solution.h: declaration file.
// Contains the declaration of the MaximumDiversitySolution class.

#ifndef MAXIMUM_DIVERSITY_SOLUTION_H_
#define MAXIMUM_DIVERSITY_SOLUTION_H_

#include <vector>

#include "../instances/maximum_diversity_instance.h"
#include "solution.h"

/**
 * @class MaximumDiversitySolution
 *
 * @brief Class that represents a solution of the Maximum Diversity Problem.
 *
 * The class stores both the primary structures of the solution and the
 * auxiliary structures needed to evaluate and modify it efficiently.
 */
class MaximumDiversitySolution : public Solution {
 public:
  MaximumDiversitySolution(const MaximumDiversityInstance& instance, unsigned subset_size);
  MaximumDiversitySolution(const std::vector<bool>& selected_elements, const MaximumDiversityInstance& instance, unsigned subset_size);
  ~MaximumDiversitySolution() override = default;

  const MaximumDiversityInstance& GetInstance() const;
  int GetElementCount() const;
  int GetDimension() const;
  unsigned GetSubsetSize() const;
  int GetSelectedElementCount() const;
  const std::vector<bool>& GetSelectedElements() const;
  const std::vector<int>& GetElementsInSolution() const;
  const std::vector<double>& GetContributionByElement() const;
  double GetContributionOfElement(int element_id) const;
  double GetObjectiveValue() const { return diversity_; } 

  bool AddElement(int element_id);
  bool RemoveElement(int element_id);
  void Clear();

  bool IsElementSelected(int element_id) const;
  bool CanAddElement(int element_id) const;
  bool CanRemoveElement(int element_id) const;

  bool IsFeasible(double tolerance = 1e-8) const;
  // Used to check correct behavior of delta evaluations.
  double ComputeObjectiveFromScratch() const;
  bool CheckObjectiveConsistency(double tolerance = 1e-8) const;

 private:
  bool HasElementInSolution(int element_id) const;
  void AddElementToSolutionList(int element_id);
  void RemoveElementFromSolutionList(int element_id);
  void IncreaseContributionCounters(int element_id);
  void DecreaseContributionCounters(int element_id);
  bool CheckListConsistency() const;
  bool IsValidElementId(int element_id) const;

  const MaximumDiversityInstance& instance_;
  unsigned subset_size_ = 0;
  // Primary structures.
  std::vector<bool> selected_elements_;  // x
  std::vector<int> elements_in_solution_;
  // Auxiliary structures.
  std::vector<double> contribution_by_element_;

  double diversity_ = 0.0;
};

#endif
