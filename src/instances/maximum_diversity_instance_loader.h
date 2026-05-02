// University of La Laguna
// School of Engineering and Technology
// Bachelor's Degree in Computer Engineering
// Subject: Design and Analysis of Algorithms
// Course: 3rd
// Practice 6: Maximum Diversity Problem
// Author: Javier Acosta Portocarrero
// Date: 02/05/2026
// File maximum_diversity_instance_loader.h: declaration file.
// Contains the declaration of the MaximumDiversityInstanceLoader class.

#ifndef MAXIMUM_DIVERSITY_INSTANCE_LOADER_H_
#define MAXIMUM_DIVERSITY_INSTANCE_LOADER_H_

#include "maximum_diversity_instance.h"

/**
 * @class MaximumDiversityInstanceLoader
 *
 * @brief Abstract class responsible for loading instances of the Maximum
 * Diversity Problem, the concrete implementations are responsible for the
 * actual reading.
 */
class MaximumDiversityInstanceLoader {
 public:
  virtual ~MaximumDiversityInstanceLoader() = default;

  /**
   * @brief Loads an instance of the Maximum Diversity Problem.
   * @return A pointer to the loaded instance.
   */
  virtual MaximumDiversityInstance* Load() const = 0;
};

#endif
