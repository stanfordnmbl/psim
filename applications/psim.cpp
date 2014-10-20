#include <PSim/PSim.h>
#include "../PSim/ParameterValue.h"

int main(int argc, char* argv[]) {
    PSim::Tool pstool(argv[1]);
    PSim::ParameterValueSet solution = pstool.run();
    // TODO.
    solution.print("results.xml");
    return 0;
}