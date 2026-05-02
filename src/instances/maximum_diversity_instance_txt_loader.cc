// University of La Laguna
// School of Engineering and Technology
// Bachelor's Degree in Computer Engineering
// Subject: Design and Analysis of Algorithms
// Course: 3rd
// Practice 6: Maximum Diversity Problem
// Author: Javier Acosta Portocarrero
// Date: 02/05/2026
// File maximum_diversity_instance_txt_loader.cc: implementation file.
// Contains the implementation of the MaximumDiversityInstanceTxtLoader class.

#include <cctype>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "maximum_diversity_instance_txt_loader.h"

/**
 * @brief Loads an instance from the configured file.
 *
 * The method reads the number of elements, the dimension and the coordinate
 * matrix of the instance.
 *
 * @return Pointer to the loaded instance.
 *
 * @throw std::runtime_error If the file cannot be opened or its format is not
 * valid.
 */
MaximumDiversityInstance* MaximumDiversityInstanceTxtLoader::Load() const {
  std::vector<std::string> lines = ReadFileLines();
  if (static_cast<int>(lines.size()) < 2) {
    throw std::runtime_error("The instance file is incomplete.");
  }

  int element_count = ReadIntegerLine(lines[0], "element count");
  int dimension = ReadIntegerLine(lines[1], "dimension");

  if (static_cast<int>(lines.size()) != element_count + 2) {
    throw std::runtime_error("Unexpected number of coordinate rows.");
  }

  std::vector<std::vector<double> > coordinates;
  for (int i = 0; i < element_count; ++i) {
    coordinates.push_back(ReadCoordinateLine(lines[i + 2], dimension));
  }

  MaximumDiversityInstance* instance = new MaximumDiversityInstance(
      element_count, dimension, coordinates);
  if (!instance->IsValid()) {
    delete instance;
    throw std::runtime_error("The loaded instance is not valid.");
  }

  return instance;
}

/**
 * @brief Reads the useful lines of the instance file.
 *
 * Empty lines are ignored.
 *
 * @return Vector with the non-empty lines of the file.
 *
 * @throw std::runtime_error If the file cannot be opened.
 */
std::vector<std::string> MaximumDiversityInstanceTxtLoader::ReadFileLines() const {
  std::ifstream input_file(file_path_);
  if (!input_file) {
    throw std::runtime_error("Could not open file: " + file_path_);
  }

  std::vector<std::string> lines;
  std::string line;
  while (std::getline(input_file, line)) {
    line = Trim(line);
    if (!line.empty()) {
      lines.push_back(line);
    }
  }

  return lines;
}

/**
 * @brief Removes leading and trailing blank spaces from a string.
 *
 * @param text Input string.
 * @return Trimmed string.
 */
std::string MaximumDiversityInstanceTxtLoader::Trim(const std::string& text) const {
  std::size_t begin = 0;
  while (begin < text.size() && std::isspace(static_cast<unsigned char>(text[begin]))) {
    ++begin;
  }
  std::size_t end = text.size();
  while (end > begin && std::isspace(static_cast<unsigned char>(text[end - 1]))) {
    --end;
  }

  return text.substr(begin, end - begin);
}

/**
 * @brief Splits a string by blank spaces.
 *
 * @param text Input string.
 * @return Vector of substrings.
 */
std::vector<std::string> MaximumDiversityInstanceTxtLoader::Split(const std::string& text) const {
  std::vector<std::string> parts;
  std::stringstream stream(text);
  std::string token;
  while (stream >> token) {
    parts.push_back(token);
  }

  return parts;
}

/**
 * @brief Normalizes a numeric token using a decimal point.
 *
 * @param token Input token.
 * @return Normalized token.
 */
std::string MaximumDiversityInstanceTxtLoader::NormalizeDoubleToken(const std::string& token) const {
  std::string result = token;
  for (int i = 0; i < static_cast<int>(result.size()); ++i) {
    if (result[i] == ',') {
      result[i] = '.';
    }
  }

  return result;
}

/**
 * @brief Reads an integer value from a line.
 *
 * @param line Input line.
 * @param field_name Name of the field, used in error messages.
 * @return Parsed integer value.
 */
int MaximumDiversityInstanceTxtLoader::ReadIntegerLine(
    const std::string& line, const std::string& field_name) const {
  std::vector<std::string> tokens = Split(line);
  if (tokens.size() != 1) {
    throw std::runtime_error("Invalid integer field: " + field_name);
  }

  return std::stoi(tokens[0]);
}

/**
 * @brief Reads a double value from a token.
 *
 * @param token Input token.
 * @return Parsed double value.
 */
double MaximumDiversityInstanceTxtLoader::ReadDoubleToken(const std::string& token) const {
  return std::stod(NormalizeDoubleToken(Trim(token)));
}

/**
 * @brief Reads a coordinate row of the instance.
 *
 * @param line Input row.
 * @param expected_dimension Expected number of coordinates.
 * @return Parsed vector of coordinates.
 *
 * @throw std::runtime_error If the number of coordinates is not valid.
 */
std::vector<double> MaximumDiversityInstanceTxtLoader::ReadCoordinateLine(
    const std::string& line, int expected_dimension) const {
  std::vector<std::string> tokens = Split(line);
  if (static_cast<int>(tokens.size()) != expected_dimension) {
    throw std::runtime_error("Unexpected number of coordinates in row.");
  }

  std::vector<double> coordinates;
  for (int i = 0; i < static_cast<int>(tokens.size()); ++i) {
    coordinates.push_back(ReadDoubleToken(tokens[i]));
  }

  return coordinates;
}
