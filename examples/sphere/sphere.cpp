// This example doesn't actually use the model or states. We are just
// minimizing the scalar function (x - 3)^2.

#include <iostream>

#include <PSim/PSim.h>

class Obj : public PSim::Objective {
OpenSim_DECLARE_CONCRETE_OBJECT(Obj, PSim::Objective);
    SimTK::Real evaluate(const PSim::ParameterValueSet& pvalset,
            const Model& model,
            const SimTK::State& finalState) const
    {
        return pow(pvalset[0].get_value() - 3, 2);
    }
};

int main()
{
    PSim::Tool pstool;

    PSim::Parameter x;
    x.setName("x");
    x.set_default_value(5);
    pstool.append_parameters(x);

    pstool.append_objectives(Obj());

    pstool.setSerializeAllDefaults(true);
    pstool.print("PSimToolSetup.xml");

    PSim::ParameterValueSet soln = pstool.run();
    soln.print("solution.xml");


    return 0;
}
