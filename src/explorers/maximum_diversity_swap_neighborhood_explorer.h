// University of La Laguna
// School of Engineering and Technology
// Bachelor's Degree in Computer Engineering
// Subject: Design and Analysis of Algorithms
// Course: 3rd
// Practice 6: Multi-Source Capacitated Facility Location Problem with Customer
// Incompatibilities
// Author: Javier Acosta Portocarrero
// Date: 10/04/2026
// File maximum_diversity_swap_explorer.h: declaration file.
// Contains the declaration of the MaximumDiversitySwapExplorer class.

#ifndef MAXIMUM_DIVERSITY_SWAP_EXPLORER_H_
#define MAXIMUM_DIVERSITY_SWAP_EXPLORER_H_

#include "maximum_diversity_neighborhood_explorer.h"

class MaximumDiversitySwapExplorer : public MaximumDiversityNeighborhoodExplorer {
 public:
  MaximumDiversitySwapExplorer() = default;
  ~MaximumDiversitySwapExplorer() override = default;

  bool Explore(MaximumDiversitySolution* solution, double amount_tol, double improvement_tol) const override;
 private:
  double EvaluateSwapDelta(const MaximumDiversitySolution& solution, int first_element_id, int second_element_id, double amount_tol, double improvement_tol) const;

  bool CanSwapElements(const MaximumDiversitySolution& solution, int first_element_id , int second_element_id, double amount_tol, double improvement_tol) const;
};

#endif