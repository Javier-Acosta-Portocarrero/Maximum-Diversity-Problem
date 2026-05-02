// University of La Laguna
// School of Engineering and Technology
// Bachelor's Degree in Computer Engineering
// Subject: Design and Analysis of Algorithms
// Course: 3rd
// Practice 6: Maximum Diversity Problem
// Author: Javier Acosta Portocarrero
// Date: 02/05/2026
// File maximum_diversity_solution_saver.h: declaration file.
// Contains the declaration of the MaximumDiversitySolutionSaver class.

#ifndef MAXIMUM_DIVERSITY_SOLUTION_SAVER_H_
#define MAXIMUM_DIVERSITY_SOLUTION_SAVER_H_

#include <string>

#include "maximum_diversity_solution.h"

/**
 * @class MaximumDiversitySolutionSaver
 *
 * @brief Class used to save Maximum Diversity solutions into an output file.
 *
 * The class appends the solution information to the selected file, so several
 * solutions can be stored in the same file without overwriting previous ones.
 */
class MaximumDiversitySolutionSaver {
 public:
  explicit MaximumDiversitySolutionSaver(const std::string& file_path) : file_path_(file_path) {}

  void Save(const MaximumDiversitySolution& solution,
            const std::string& instance_name,
            const std::string& algorithm,
            int execution,
            double cpu_time,
            int generated_nodes = -1) const;

 private:
  std::string file_path_;
};

#endif
