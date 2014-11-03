#include <PSim/osimPSim.h>
#include <OpenSim/Common/LoadOpenSimLibrary.h>

// TODO accept an output directory here.

int main(int argc, char* argv[]) {
    LoadOpenSimLibraries(argc, argv);    

    PSimTool pstool(argv[1]);
    pstool.print(pstool.get_results_dir() + "./psim_setup.xml");

    PSimParameterValueSet solution = pstool.run();
    solution.print(pstool.get_results_dir() + "./solution.xml");

    return 0;
}
