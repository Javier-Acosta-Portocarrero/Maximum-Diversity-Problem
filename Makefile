CXX = g++
CXXFLAGS =  -std=c++23 -O3 -march=native
TARGET = practice6_DAA

SOURCES =  src/main.cc \
          src/algorithms/branch_and_bound_maximum_diversity_algorithm.cc \
          src/algorithms/branch_and_bound_maximum_diversity_greatest_bound_algorithm.cc \
          src/algorithms/branch_and_bound_maximum_diversity_lowest_bound_algorithm.cc \
          src/algorithms/grasp_algorithm.cc \
          src/algorithms/grasp_maximum_diversity_solver.cc \
          src/explorers/maximum_diversity_swap_neighborhood_explorer.cc \
          src/instances/maximum_diversity_instance_txt_loader.cc \
          src/instances/maximum_diversity_instance.cc \
          src/solutions/maximum_diversity_solution_saver.cc \
          src/solutions/maximum_diversity_solution.cc

OBJECTS = $(SOURCES:.cc=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

cleano:
	rm -f $(OBJECTS)

.PHONY: all clean cleano