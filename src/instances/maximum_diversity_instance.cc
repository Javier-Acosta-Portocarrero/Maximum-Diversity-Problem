// University of La Laguna
// School of Engineering and Technology
// Bachelor's Degree in Computer Engineering
// Subject: Design and Analysis of Algorithms
// Course: 3rd
// Practice 6: Maximum Diversity Problem
// Author: Javier Acosta Portocarrero
// Date: 02/05/2026
// File maximum_diversity_instance.cc: implementation file.
// Contains the implementation of the MaximumDiversityInstance class.

#include <algorithm>  // std::sort
#include <cmath>
#include <stdexcept>

#include "maximum_diversity_instance.h"

/**
 * @brief Builds an instance from the essential input data.
 *
 * @param element_count Number of elements.
 * @param dimension Dimension of each element.
 * @param coordinates Coordinate matrix.
 */
MaximumDiversityInstance::MaximumDiversityInstance(
    int element_count, int dimension,
    const std::vector<std::vector<double> >& coordinates)
    : element_count_{element_count},
      dimension_{dimension},
      coordinates_{coordinates} {

  BuildAuxiliaryStructures();
}

/**
 * @brief Returns the number of elements in the instance.
 *
 * @return Number of elements.
 */
int MaximumDiversityInstance::GetElementCount() const {
  return element_count_;
}

/**
 * @brief Returns the dimension of the elements.
 *
 * @return Dimension of the elements.
 */
int MaximumDiversityInstance::GetDimension() const {
  return dimension_;
}

/**
 * @brief Returns a coordinate of a given element.
 *
 * @param element_id Element identifier.
 * @param coordinate_id Coordinate identifier.
 * @return Coordinate value.
 *
 * @throw std::out_of_range If any identifier is not valid.
 */
double MaximumDiversityInstance::GetCoordinate(int element_id, int coordinate_id) const {
  if (!IsValidElementId(element_id)) {
    throw std::out_of_range("Invalid element id.");
  }
  if (!IsValidCoordinateId(coordinate_id)) {
    throw std::out_of_range("Invalid coordinate id.");
  }

  return coordinates_[element_id][coordinate_id];
}

/**
 * @brief Returns the distance between two elements.
 *
 * @param element_1 First element.
 * @param element_2 Second element.
 * @return Euclidean distance between both elements.
 *
 * @throw std::out_of_range If any element identifier is not valid.
 */
double MaximumDiversityInstance::GetDistance(int element_1, int element_2) const {
  if (!IsValidElementId(element_1) || !IsValidElementId(element_2)) {
    throw std::out_of_range("Invalid element id.");
  }

  return distance_matrix_[element_1][element_2];
}

/**
 * @brief Returns the distance from an element to the global center.
 *
 * @param element_id Element identifier.
 * @return Distance from the element to the global center.
 *
 * @throw std::out_of_range If the element identifier is not valid.
 */
double MaximumDiversityInstance::GetDistanceFromGlobalCenter(int element_id) const {
  if (!IsValidElementId(element_id)) {
    throw std::out_of_range("Invalid element id.");
  }

  return distance_from_global_center_[element_id];
}

/**
 * @brief Returns the average distance from an element to the rest of elements.
 *
 * @param element_id Element identifier.
 * @return Average distance of the element.
 *
 * @throw std::out_of_range If the element identifier is not valid.
 */
double MaximumDiversityInstance::GetAverageDistanceByElement(int element_id) const {
  if (!IsValidElementId(element_id)) {
    throw std::out_of_range("Invalid element id.");
  }

  return average_distance_by_element_[element_id];
}

/**
 * @brief Returns the maximum distance between two elements of the instance.
 *
 * @return Maximum distance.
 */
double MaximumDiversityInstance::GetMaxDistance() const {
  return max_distance_;
}

/**
 * @brief Returns the coordinates of an element.
 *
 * @param element_id Element identifier.
 * @return Constant reference to the coordinates of the element.
 *
 * @throw std::out_of_range If the element identifier is not valid.
 */
const std::vector<double>& MaximumDiversityInstance::GetElementCoordinates(int element_id) const {
  if (!IsValidElementId(element_id)) {
    throw std::out_of_range("Invalid element id.");
  }

  return coordinates_[element_id];
}

/**
 * @brief Returns the coordinate matrix.
 *
 * @return Constant reference to the coordinate matrix.
 */
const std::vector<std::vector<double> >& MaximumDiversityInstance::GetCoordinates() const {
  return coordinates_;
}

/**
 * @brief Returns the distance matrix.
 *
 * @return Constant reference to the distance matrix.
 */
const std::vector<std::vector<double> >& MaximumDiversityInstance::GetDistanceMatrix() const {
  return distance_matrix_;
}

/**
 * @brief Returns the center of gravity of all elements.
 *
 * @return Constant reference to the global center.
 */
const std::vector<double>& MaximumDiversityInstance::GetGlobalCenter() const {
  return global_center_;
}

/**
 * @brief Returns the elements sorted by decreasing distance from the global center.
 *
 * @return Constant reference to the sorted elements.
 */
const std::vector<int>& MaximumDiversityInstance::GetElementsSortedByDistanceFromGlobalCenter() const {
  return elements_sorted_by_distance_from_global_center_;
}

/**
 * @brief Returns the elements sorted by decreasing average distance.
 *
 * @return Constant reference to the sorted elements.
 */
const std::vector<int>& MaximumDiversityInstance::GetElementsSortedByAverageDistance() const {
  return elements_sorted_by_average_distance_;
}

/**
 * @brief Computes the center of gravity of a subset of elements.
 *
 * @param elements Elements used to compute the center.
 * @return Center of gravity of the selected elements.
 */
std::vector<double> MaximumDiversityInstance::ComputeCenter(const std::vector<int>& elements) const {
  std::vector<double> center(dimension_, 0.0);
  if (elements.empty()) {
    return center;
  }

  for (int i = 0; i < static_cast<int>(elements.size()); ++i) {
    int element_id = elements[i];
    if (!IsValidElementId(element_id)) {
      throw std::out_of_range("Invalid element id.");
    }
    for (int r = 0; r < dimension_; ++r) {
      center[r] += coordinates_[element_id][r];
    }
  }
  for (int r = 0; r < dimension_; ++r) {
    center[r] /= static_cast<double>(elements.size());
  }

  return center;
}

/**
 * @brief Computes the Euclidean distance from an element to a point.
 *
 * @param element_id Element identifier.
 * @param point Coordinates of the point.
 * @return Euclidean distance from the element to the point.
 *
 * @throw std::out_of_range If the element identifier is not valid.
 * @throw std::runtime_error If the point dimension is not valid.
 */
double MaximumDiversityInstance::GetDistanceToPoint(int element_id, const std::vector<double>& point) const {
  if (!IsValidElementId(element_id)) {
    throw std::out_of_range("Invalid element id.");
  }
  if (static_cast<int>(point.size()) != dimension_) {
    throw std::runtime_error("Invalid point dimension.");
  }

  double squared_distance = 0.0;
  for (int r = 0; r < dimension_; ++r) {
    const double difference = coordinates_[element_id][r] - point[r];
    squared_distance += difference * difference;
  }

  return std::sqrt(squared_distance);
}

/**
 * @brief Validates the consistency of the instance data.
 *
 * @return True if the instance is valid, false otherwise.
 */
bool MaximumDiversityInstance::IsValid() const {
  return ValidateDimensions() &&
         ValidateCoordinates() &&
         ValidateDistanceMatrix() &&
         ValidateAuxiliaryStructures();
}

/**
 * @brief Builds all auxiliary structures of the instance.
 *
 * This method should be called once the raw data of the instance has already
 * been loaded.
 */
void MaximumDiversityInstance::BuildAuxiliaryStructures() {
  BuildDistanceMatrix();
  BuildGlobalCenter();
  BuildElementOrderings();
}

/**
 * @brief Builds the symmetric Euclidean distance matrix.
 */
void MaximumDiversityInstance::BuildDistanceMatrix() {
  distance_matrix_.clear();
  distance_matrix_.resize(element_count_, std::vector<double>(element_count_, 0.0));
  max_distance_ = 0.0;
  average_distance_by_element_.assign(element_count_, 0.0);

  for (int i = 0; i < element_count_; ++i) {
    for (int j = i + 1; j < element_count_; ++j) {
      double squared_distance = 0.0;
      for (int r = 0; r < dimension_; ++r) {
        const double difference = coordinates_[i][r] - coordinates_[j][r];
        squared_distance += difference * difference;
      }
      const double distance = std::sqrt(squared_distance);
      distance_matrix_[i][j] = distance;
      distance_matrix_[j][i] = distance;
      average_distance_by_element_[i] += distance;
      average_distance_by_element_[j] += distance;
      if (distance > max_distance_) {
        max_distance_ = distance;
      }
    }
  }

  if (element_count_ > 1) {
    for (int i = 0; i < element_count_; ++i) {
      average_distance_by_element_[i] /= static_cast<double>(element_count_ - 1);
    }
  }
}

/**
 * @brief Builds the center of gravity of the whole set of elements.
 */
void MaximumDiversityInstance::BuildGlobalCenter() {
  global_center_.assign(dimension_, 0.0);
  distance_from_global_center_.assign(element_count_, 0.0);

  if (element_count_ == 0) {
    return;
  }

  for (int i = 0; i < element_count_; ++i) {
    for (int r = 0; r < dimension_; ++r) {
      global_center_[r] += coordinates_[i][r];
    }
  }
  for (int r = 0; r < dimension_; ++r) {
    global_center_[r] /= static_cast<double>(element_count_);
  }
  for (int i = 0; i < element_count_; ++i) {
    distance_from_global_center_[i] = GetDistanceToPoint(i, global_center_);
  }
}

/**
 * @brief Builds element orderings used by constructive algorithms.
 * Two orderings are generated:
 * - elements sorted by decreasing distance from the global center,
 * - elements sorted by decreasing average distance to the rest of elements.
 */
void MaximumDiversityInstance::BuildElementOrderings() {
  elements_sorted_by_distance_from_global_center_.clear();
  elements_sorted_by_average_distance_.clear();
  elements_sorted_by_distance_from_global_center_.resize(element_count_);
  elements_sorted_by_average_distance_.resize(element_count_);

  for (int i = 0; i < element_count_; ++i) {
    elements_sorted_by_distance_from_global_center_[i] = i;
    elements_sorted_by_average_distance_[i] = i;
  }

  std::sort(elements_sorted_by_distance_from_global_center_.begin(),
            elements_sorted_by_distance_from_global_center_.end(),
            [this](int element_1, int element_2) {
              if (distance_from_global_center_[element_1] ==
                  distance_from_global_center_[element_2]) {
                return element_1 < element_2;
              }
              return distance_from_global_center_[element_1] >
                     distance_from_global_center_[element_2];
            });

  std::sort(elements_sorted_by_average_distance_.begin(),
            elements_sorted_by_average_distance_.end(),
            [this](int element_1, int element_2) {
              if (average_distance_by_element_[element_1] ==
                  average_distance_by_element_[element_2]) {
                return element_1 < element_2;
              }
              return average_distance_by_element_[element_1] >
                     average_distance_by_element_[element_2];
            });
}

/**
 * @brief Checks whether an element identifier is valid.
 *
 * @param element_id Element identifier.
 * @return True if the identifier is valid, false otherwise.
 */
bool MaximumDiversityInstance::IsValidElementId(int element_id) const {
  return element_id >= 0 && element_id < element_count_;
}

/**
 * @brief Checks whether a coordinate identifier is valid.
 *
 * @param coordinate_id Coordinate identifier.
 * @return True if the identifier is valid, false otherwise.
 */
bool MaximumDiversityInstance::IsValidCoordinateId(int coordinate_id) const {
  return coordinate_id >= 0 && coordinate_id < dimension_;
}

/**
 * @brief Validates basic dimensions of the instance.
 */
bool MaximumDiversityInstance::ValidateDimensions() const {
  if (element_count_ <= 0 || dimension_ <= 0) {
    return false;
  }
  if (static_cast<int>(coordinates_.size()) != element_count_) {
    return false;
  }

  return true;
}

/**
 * @brief Validates the coordinate matrix.
 */
bool MaximumDiversityInstance::ValidateCoordinates() const {
  for (int i = 0; i < element_count_; ++i) {
    if (static_cast<int>(coordinates_[i].size()) != dimension_) {
      return false;
    }
  }

  return true;
}

/**
 * @brief Validates the distance matrix.
 */
bool MaximumDiversityInstance::ValidateDistanceMatrix() const {
  if (static_cast<int>(distance_matrix_.size()) != element_count_) {
    return false;
  }
  for (int i = 0; i < element_count_; ++i) {
    if (static_cast<int>(distance_matrix_[i].size()) != element_count_) {
      return false;
    }
    if (distance_matrix_[i][i] != 0.0) {
      return false;
    }
    for (int j = i + 1; j < element_count_; ++j) {
      if (distance_matrix_[i][j] < 0.0) {
        return false;
      }
      if (std::fabs(distance_matrix_[i][j] - distance_matrix_[j][i]) > 1e-8) {
        return false;
      }
    }
  }

  return true;
}

/**
 * @brief Validates auxiliary preprocessed structures.
 */
bool MaximumDiversityInstance::ValidateAuxiliaryStructures() const {
  if (static_cast<int>(global_center_.size()) != dimension_) {
    return false;
  }
  if (static_cast<int>(distance_from_global_center_.size()) != element_count_) {
    return false;
  }
  if (static_cast<int>(average_distance_by_element_.size()) != element_count_) {
    return false;
  }
  if (static_cast<int>(elements_sorted_by_distance_from_global_center_.size()) != element_count_ ||
      static_cast<int>(elements_sorted_by_average_distance_.size()) != element_count_) {
    return false;
  }

  return true;
}
