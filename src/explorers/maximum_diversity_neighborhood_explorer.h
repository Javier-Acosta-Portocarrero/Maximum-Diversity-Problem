// University of La Laguna
// School of Engineering and Technology
// Bachelor's Degree in Computer Engineering
// Subject: Design and Analysis of Algorithms
// Course: 3rd
// Practice 6: Multi-Source Capacitated Facility Location Problem with Customer
// Incompatibilities
// Author: Javier Acosta Portocarrero
// Date: 10/04/2026
// File maximum_diversity_neighborhood_explorer.h: declaration file.
// Contains the declaration of the MaximumDiversityNeighborhoodExplorer class.

#ifndef MAXIMUM_DIVERSITY_NEIGHBOORHOOD_EXPLORER_H_
#define MAXIMUM_DIVERSITY_NEIGHBOORHOOD_EXPLORER_H_

#include "../solutions/maximum_diversity_solution.h"

class MaximumDiversityNeighborhoodExplorer {
 public:
  virtual ~MaximumDiversityNeighborhoodExplorer() = default;

  virtual bool Explore(MaximumDiversitySolution* solution, double amount_tol, double improvement_tol) const = 0;
};

 #endif