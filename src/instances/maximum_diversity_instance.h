// University of La Laguna
// School of Engineering and Technology
// Bachelor's Degree in Computer Engineering
// Subject: Design and Analysis of Algorithms
// Course: 3rd
// Practice 6: Maximum Diversity Problem
// Author: Javier Acosta Portocarrero
// Date: 02/05/2026
// File maximum_diversity_instance.h: declaration file.
// Contains the declaration of the MaximumDiversityInstance class.

#ifndef MAXIMUM_DIVERSITY_INSTANCE_H_
#define MAXIMUM_DIVERSITY_INSTANCE_H_

#include <vector>

#include "instance.h"

/**
 * @class MaximumDiversityInstance
 *
 * @brief Class that represents an instance of the Maximum Diversity Problem.
 * It also stores auxiliary structures to speed up greedy and branch and bound
 * algorithms.
 */
class MaximumDiversityInstance : public Instance {
 public:
  MaximumDiversityInstance(int element_count, int dimension,
                           const std::vector<std::vector<double> >& coordinates);
  ~MaximumDiversityInstance() override = default;

  int GetElementCount() const;
  int GetDimension() const;
  double GetCoordinate(int element_id, int coordinate_id) const;
  double GetDistance(int element_1, int element_2) const;
  double GetDistanceFromGlobalCenter(int element_id) const;
  double GetAverageDistanceByElement(int element_id) const;
  double GetMaxDistance() const;

  const std::vector<double>& GetElementCoordinates(int element_id) const;
  const std::vector<std::vector<double> >& GetCoordinates() const;
  const std::vector<std::vector<double> >& GetDistanceMatrix() const;
  const std::vector<double>& GetGlobalCenter() const;
  const std::vector<int>& GetElementsSortedByDistanceFromGlobalCenter() const;
  const std::vector<int>& GetElementsSortedByAverageDistance() const;

  std::vector<double> ComputeCenter(const std::vector<int>& elements) const;
  double GetDistanceToPoint(int element_id, const std::vector<double>& point) const;

  bool IsValid() const;
  bool IsValidElementId(int element_id) const;
 private:
  void BuildAuxiliaryStructures();
  void BuildDistanceMatrix();
  void BuildGlobalCenter();
  void BuildElementOrderings();

  bool IsValidCoordinateId(int coordinate_id) const;
  bool ValidateDimensions() const;
  bool ValidateCoordinates() const;
  bool ValidateDistanceMatrix() const;
  bool ValidateAuxiliaryStructures() const;

  // Problem size.
  int element_count_ = 0;
  int dimension_ = 0;

  // coordinates_[i][r] = coordinate r of element i.
  std::vector<std::vector<double> > coordinates_;
  // distance_matrix_[i][j] = Euclidean distance between elements i and j.
  std::vector<std::vector<double> > distance_matrix_;

  std::vector<double> global_center_;
  std::vector<double> distance_from_global_center_;
  std::vector<double> average_distance_by_element_;

  std::vector<int> elements_sorted_by_distance_from_global_center_;
  std::vector<int> elements_sorted_by_average_distance_;

  double max_distance_ = 0.0;
};

#endif
