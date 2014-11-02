// This example doesn't actually use the model or states. We are just
// minimizing the scalar function (x - 3)^2.

#include <iostream>

#include <PSim/osimPSim.h>

class Obj : public PSimGoal {
OpenSim_DECLARE_CONCRETE_OBJECT(Obj, PSimGoal);
    SimTK::Real extendEvaluate(const OpenSim::PSimParameterValueSet & pvalset,
            const OpenSim::StateTrajectory& states) const
    {
        return pow(pvalset[0].get_value() - 3, 2);
    }
};

int main()
{
    OpenSim::PSimTool pstool;

    OpenSim::PSimParameter x;
    x.setName("x");
    x.set_default_value(5);
    pstool.append_parameters(x);

    pstool.append_goals(Obj());

    pstool.setSerializeAllDefaults(true);
    pstool.print("PSimToolSetup.xml");

    OpenSim::PSimParameterValueSet soln = pstool.run();
    soln.print("solution.xml");


    return 0;
}
