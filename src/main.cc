// University of La Laguna
// School of Engineering and Technology
// Bachelor's Degree in Computer Engineering
// Subject: Design and Analysis of Algorithms
// Course: 3rd
// Practice 6: Maximum Diversity Problem
// Author: Javier Acosta Portocarrero
// Date: 02/05/2026
// File main.cc: implementation file.
// Contains the implementation of the main function of the program.

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "instances/maximum_diversity_instance.h"
#include "instances/maximum_diversity_instance_txt_loader.h"
#include "solutions/maximum_diversity_solution.h"
#include "solutions/maximum_diversity_solution_saver.h"
#include "algorithms/grasp_maximum_diversity_solver.h"
#include "algorithms/branch_and_bound_maximum_diversity_lowest_bound_algorithm.h"
#include "algorithms/branch_and_bound_maximum_diversity_greatest_bound_algorithm.h"
#include "explorers/maximum_diversity_swap_neighborhood_explorer.h"

/**
 * @brief Extracts the file name from a path.
 *
 * @param path Full path to the file.
 * @return File name only.
 */
std::string GetInstanceName(const std::string& path) {
  std::size_t pos = path.find_last_of("/\\");
  if (pos == std::string::npos) {
    return path;
  }
  return path.substr(pos + 1);
}

/**
 * @brief Prints the table header.
 */
void PrintTableHeader() {
  std::cout
      << std::left
      << std::setw(18) << "Problema"
      << std::setw(14) << "Algoritmo"
      << std::setw(8) << "n"
      << std::setw(8) << "K"
      << std::setw(8) << "m"
      << std::setw(8) << "|LRC|"
      << std::setw(8) << "Ejec."
      << std::setw(14) << "z"
      << std::setw(18) << "CPU_Time"
      << std::endl;

  std::cout << std::string(100, '-') << std::endl;
}

/**
 * @brief Main function of the program, responsible for loading an instance from a file,
 *        solving it using a selected strategy, and printing a results table.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 * @return Exit code.
 */
int main(int argc, char* argv[]) {
  if (argc != 3 && argc != 4 && argc != 5) {
    std::cerr << "Usage: " << argv[0]
              << " <instance_file.txt> <greedy|grasp|bnb-lowest|bnb-greatest> [lrc_size] [output_file]"
              << std::endl;
    return 1;
  }

  std::srand(static_cast<unsigned>(std::time(nullptr)));
  try {
    const std::string instance_path = argv[1];
    const std::string algorithm = argv[2];
    const std::vector<int> subset_sizes = {2, 3, 4, 5, 6, 8, 10};
    const int lrc_size = (algorithm == "greedy") ? 1 : ((argc >= 4) ? std::stoi(argv[3]) : 3);
    const std::string output_file = (argc == 5) ? argv[4] : "outputs/saved_solutions.txt";
    const std::string instance_name = GetInstanceName(instance_path);
    const int executions = 1;
    MaximumDiversityInstanceTxtLoader loader(instance_path);
    MaximumDiversityInstance* instance = loader.Load();

    for (int subset_size : subset_sizes) {
      if (subset_size <= 0 || subset_size >= instance->GetElementCount()) {
        delete instance;
        throw std::invalid_argument("Invalid subset size.");
      }
    }

    PrintTableHeader();
    for (std::size_t subset_index = 0; subset_index < subset_sizes.size(); ++subset_index) {
      const int subset_size = subset_sizes[subset_index];

      for (int execution = 1; execution <= executions; ++execution) {
        std::vector<MaximumDiversityNeighborhoodExplorer*> explorers;
        // Changing this to false and its possible to do bnb with the greedy solution as initial solution.
        if (false) {
        //if (algorithm != "greedy") {
          explorers.push_back(new MaximumDiversitySwapExplorer());
        }

        auto start = std::chrono::high_resolution_clock::now();
        GraspMaximumDiversitySolver grasp_solver(lrc_size, explorers, subset_size);
        Solution* initial_generic_solution = grasp_solver.Solve(instance);
        MaximumDiversitySolution* initial_solution = dynamic_cast<MaximumDiversitySolution*>(initial_generic_solution);
        if (initial_solution == nullptr) {
          delete initial_generic_solution;
          for (MaximumDiversityNeighborhoodExplorer* explorer : explorers) {
            delete explorer;
          }
          delete instance;
          throw std::runtime_error("GRASP did not return a MaximumDiversitySolution.");
        }
        MaximumDiversitySolution* solution = nullptr;
        if (algorithm == "greedy" || algorithm == "grasp") {
          solution = initial_solution;
          initial_solution = nullptr;
        } else if (algorithm == "bnb-lowest") {
          BranchAndBoundMaximumDiversityLowestBoundAlgorithm solver;
          solution = solver.Solve(*initial_solution, subset_size);
        } else if (algorithm == "bnb-greatest") {
          BranchAndBoundMaximumDiversityGreatestBoundAlgorithm solver;
          solution = solver.Solve(*initial_solution, subset_size);
        } else {
          delete initial_solution;
          for (MaximumDiversityNeighborhoodExplorer* explorer : explorers) {
            delete explorer;
          }
          delete instance;
          throw std::invalid_argument("Unknown algorithm: " + algorithm);
        }
        auto end = std::chrono::high_resolution_clock::now();

        double cpu_time = std::chrono::duration<double>(end - start).count();
        if (initial_solution != nullptr) {
          delete initial_solution;
        }
        for (MaximumDiversityNeighborhoodExplorer* explorer : explorers) {
          delete explorer;
        }

        if (solution == nullptr) {
          std::cout
              << std::left
              << std::setw(18) << instance_name
              << std::setw(14) << algorithm
              << std::setw(8) << instance->GetElementCount()
              << std::setw(8) << instance->GetDimension()
              << std::setw(8) << subset_size
              << std::setw(8) << lrc_size
              << std::setw(8) << execution
              << std::setw(14) << "-"
              << std::setw(18) << std::fixed << std::setprecision(6) << cpu_time
              << std::endl;
          continue;
        }
        if (!solution->IsFeasible()) {
          std::cerr << "ERROR: SOLUTION NOT FEASIBLE." << std::endl;
        }
        std::cout
            << std::left
            << std::setw(18) << instance_name
            << std::setw(14) << algorithm
            << std::setw(8) << instance->GetElementCount()
            << std::setw(8) << instance->GetDimension()
            << std::setw(8) << subset_size
            << std::setw(8) << lrc_size
            << std::setw(8) << execution
            << std::setw(14) << std::fixed << std::setprecision(4) << solution->GetObjectiveValue()
            << std::setw(18) << std::fixed << std::setprecision(6) << cpu_time
            << std::endl
            << "Object value from scratch: " << solution->ComputeObjectiveFromScratch()
            << std::endl;
        MaximumDiversitySolutionSaver solution_saver(output_file);
        solution_saver.Save(*solution, instance_name, algorithm, lrc_size, execution, cpu_time);

        delete solution;
      }

      if (subset_index + 1 < subset_sizes.size()) {
        std::cout << std::string(100, '-') << std::endl;
      }
    }

    delete instance;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}