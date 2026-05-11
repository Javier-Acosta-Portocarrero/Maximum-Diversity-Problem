// University of La Laguna
// School of Engineering and Technology
// Bachelor's Degree in Computer Engineering
// Subject: Design and Analysis of Algorithms
// Course: 3rd
// Practice 6: Maximum Diversity Problem
// Author: Javier Acosta Portocarrero
// Date: 04/04/2026
// File grasp-maximum-diversity-solver.h: declaration file.
// Contains the declaration of the GraspMaximumDiversitySolver class.

#ifndef GRAASP_MAXIMUM_DIVERSITY_SOLVER_H_
#define GRAASP_MAXIMUM_DIVERSITY_SOLVER_H_

#include "grasp_algorithm.h"
#include "../instances/maximum_diversity_instance.h"
#include "../solutions/maximum_diversity_solution.h"
#include "../explorers/maximum_diversity_neighborhood_explorer.h"

/**
 * @class GraspMaximumDiversitySolver
 *
 * @brief Class that implements the GRASP metaheuristic to solve the Maximum Diversity problem.
 */
class GraspMaximumDiversitySolver : public GraspAlgorithm {
 public:
  GraspMaximumDiversitySolver(unsigned cardinality = 1, std::vector<MaximumDiversityNeighborhoodExplorer*> explorers = {}, unsigned num_elements = 1) : 
                      lcr_cardinality_{cardinality}, neighboorhod_explorers_{explorers}, num_elements_{num_elements} {}
  ~GraspMaximumDiversitySolver() override = default;
 protected:
  void Preprocess(Instance* input) override;
  Solution* ConstructSolution(Instance* input) override;
  Solution* Postprocess(Solution* solution) override;
  void UpdateBest(Solution* current, Solution*& best) override;
  bool StopCriterion() override;
  const std::vector<MaximumDiversityNeighborhoodExplorer*>& GetNeighborhoodExplorers() const { return neighboorhod_explorers_;}
  double GetAmountTolerance() const { return kAmountTolerance_;}
  double GetImprovementTolerance() const { return kImprovementTolerance_;}
  
 private:
  unsigned current_grasp_iteration_ = 0;
  unsigned max_grasp_iterations_ = 1;
  unsigned lcr_cardinality_ = 1;
  unsigned num_elements_ = 1;
  std::vector<MaximumDiversityNeighborhoodExplorer*> neighboorhod_explorers_;
  static constexpr double kAmountTolerance_ = 1e-8;
  static constexpr double kImprovementTolerance_ = 1e-8;

  std::vector<int> GetSortedElementsByDistanceOfCurrentSolution( MaximumDiversitySolution* solution) const;
};

#endif