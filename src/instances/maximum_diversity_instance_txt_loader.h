// University of La Laguna
// School of Engineering and Technology
// Bachelor's Degree in Computer Engineering
// Subject: Design and Analysis of Algorithms
// Course: 3rd
// Practice 6: Maximum Diversity Problem
// Author: Javier Acosta Portocarrero
// Date: 02/05/2026
// File maximum_diversity_instance_txt_loader.h: declaration file.
// Contains the declaration of the MaximumDiversityInstanceTxtLoader class.

#ifndef MAXIMUM_DIVERSITY_INSTANCE_TXT_LOADER_H_
#define MAXIMUM_DIVERSITY_INSTANCE_TXT_LOADER_H_

#include <string>
#include <vector>

#include "maximum_diversity_instance.h"
#include "maximum_diversity_instance_loader.h"

/**
 * @class MaximumDiversityInstanceTxtLoader
 *
 * @brief Class responsible for loading Maximum Diversity instances from a text
 * file.
 */
class MaximumDiversityInstanceTxtLoader : public MaximumDiversityInstanceLoader {
 public:
  /**
   * @brief Builds a loader for a given file path.
   *
   * @param file_path Path of the instance file.
   */
  explicit MaximumDiversityInstanceTxtLoader(const std::string& file_path) : file_path_(file_path) {}
  ~MaximumDiversityInstanceTxtLoader() override = default;

  /**
   * @brief Loads an instance from the configured file.
   *
   * @return Pointer to the loaded instance.
   */
  MaximumDiversityInstance* Load() const override;

  /**
   * @brief Sets the file path.
   *
   * @param file_path Path of the instance file.
   */
  void SetFilePath(const std::string& file_path) { file_path_ = file_path; }

 private:
  std::vector<std::string> ReadFileLines() const;
  std::string Trim(const std::string& text) const;
  std::vector<std::string> Split(const std::string& text) const;
  std::string NormalizeDoubleToken(const std::string& token) const;

  int ReadIntegerLine(const std::string& line, const std::string& field_name) const;
  double ReadDoubleToken(const std::string& token) const;
  std::vector<double> ReadCoordinateLine(const std::string& line, int expected_dimension) const;

  std::string file_path_;
};

#endif
