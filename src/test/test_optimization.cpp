/*
  * @file test_optimization.cpp
  * @brief test optimization of single circuit or directory of circuits
  * 
  * Usage: ./test_optimization <circuit_file.qasm> <ecc_file.json> 
  *       OR ./test_optimization <circuit_directory> <ecc_file.json> <output_file.tsv>
  */

#include "test_optimization.h"

#include "quartz/gate/gate_utils.h"

#include <iostream>
#include <string>
#include <filesystem>

using namespace quartz;

void optimize_directory(std::string circuit_directory, std::string ecc_filename, 
  std::string output_filename) {
  
  // define context with gates
  Context ctx({GateType::input_qubit, GateType::cx,
               GateType::h, GateType::t, GateType::tdg,
               GateType::x});

  std::ofstream output_file(output_filename);
  output_file << "circuit_file\ninitial_depth\tfinal_depth\truntime(s)\n";


  float init_depth;
  float final_depth;
  double runtime;
  for (const auto & entry : std::filesystem::directory_iterator(circuit_directory)) {
    std::string circuit_filename = entry.path();
    // ignore non-qasm files
    if (circuit_filename.substr(circuit_filename.size() - 5) != ".qasm") {
      continue;
    }
    
    std::cout << "Optimizing " << circuit_filename << std::endl;
    test_benchmark(&ctx, circuit_filename, ecc_filename, 
                      init_depth, final_depth, runtime);
    
    // write results to file
    output_file << circuit_filename << "\t" << init_depth << "\t" << final_depth << "\t" << runtime << "\n";
    // print results to console
    std::cout << "Optimized from depth " << init_depth << " to depth " << final_depth \
            << " in " << runtime << "seconds." << std::endl;
  }
  output_file.close();    

}


int main(int argc, char **argv) {
  
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <circuit_file.qasm> <ecc_file.json> "
              << "OR " << argv[0] << " <circuit_directory> <ecc_file.json> <output_file.tsv>"
              << std::endl;
    return 1;
  }

  std::string circuit_arg = argv[1];
  std::string ecc_filename = argv[2];

  // if first argument is a directory, run optimization on all circuits in directory
  if (std::filesystem::is_directory(circuit_arg)) {
    std::string output_filename = argv[3];
    optimize_directory(circuit_arg, ecc_filename, output_filename);
    return 0;
  }

  // otherwise, run optimization on single circuit
  Context ctx({GateType::input_qubit, GateType::cx,
               GateType::h, GateType::t, GateType::tdg,
               GateType::x});
  test_optimization(&ctx, circuit_arg, ecc_filename);
}
