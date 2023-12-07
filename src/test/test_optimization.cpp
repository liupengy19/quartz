#include "test_optimization.h"

#include "quartz/gate/gate_utils.h"

#include <iostream>
#include <string>

using namespace quartz;

int main(int argc, char **argv) {
  
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <circuit_file.qasm> <ecc_file.json>"
              << std::endl;
    return 1;
  }
  std::string circuit_filename = argv[1];
  std::string ecc_filename = argv[2];

  Context ctx({GateType::input_qubit, GateType::cx,
               GateType::h, GateType::t, GateType::tdg,
               GateType::x});
  //   test_optimization(&ctx, "circuit/example-circuits/voqc_fig5.qasm",
  //                     "cmake-build-debug/bfs_verified.json");
  test_optimization(&ctx, circuit_filename, ecc_filename,
                    false /*use_simulated_annealing*/);
}
