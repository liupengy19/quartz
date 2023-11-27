#include "quartz/circuitseq/circuitseq.h"

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

using namespace quartz;
int test() {
  std::cout << "test" << std::endl;
  int num_qubits = 5;
  int num_input_parameters = 2;

  CircuitSeq circ = CircuitSeq(num_qubits, num_input_parameters);
  bool success = circ.add_gate(std::vector<int>{0}, std::vector<int>{},
                               new Gate(GateType::h, 1, 0), nullptr);
  if (!success) {
    std::cout << "failed to add gate" << std::endl;
  }
  circ.layerize();
  int num_layers = circ.get_depth();
  std::vector<int> layer_indices = circ.applicable_qubit_index();
  std::cout << "num_layers: " << num_layers << std::endl;
  std::cout << "layer_indices: ";
  for (std::vector<int>::size_type i = 0; i < layer_indices.size(); i++) {
    std::cout << layer_indices[i] << " ";
  }

  return 0;
}
int main() {
  test();
  return 0;
}
