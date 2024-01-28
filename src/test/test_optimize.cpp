#include "quartz/tasograph/substitution.h"
#include "quartz/tasograph/tasograph.h"
#include "test/gen_ecc_set.h"

using namespace quartz;

int main(int argc, char **argv) {
  Context ctx({GateType::input_qubit, GateType::cx, GateType::h, GateType::x});

  auto graph = Graph::from_qasm_file(&ctx, argv[1]);
  assert(graph);

  EquivalenceSet eqs;
  // Load ECC set from file
  if (!eqs.load_json(&ctx, "./ecc.json")) {
    // generate ECC set
    assert(false);
  }

  // Get xfer from the equivalent set
  auto ecc = eqs.get_all_equivalence_sets();
  std::vector<GraphXfer *> xfers;
  for (const auto &eqcs : ecc) {
    for (auto circ_0 : eqcs) {
      for (auto circ_1 : eqcs) {
        if (circ_0 != circ_1) {
          auto xfer = GraphXfer::create_GraphXfer(&ctx, circ_0, circ_1, true);
          if (xfer != nullptr) {
            xfers.push_back(xfer);
          }
        }
      }
    }
  }
  // std::cout << "number of xfers: " << xfers.size() << std::endl;

  auto newgraph = graph->optimize(xfers, graph->gate_count() * 1.05,
                                  "barenco_tof_3", "", false);
  newgraph->to_qasm("", false, false);
  return 0;
}
