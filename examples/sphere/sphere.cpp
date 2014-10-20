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

    OpenSim::Model model;
    model.print("empty.osim");

    pstool.set_model_file("empty.osim");

    PSim::Parameter x;
    x.setName("x");
    x.set_default_value(5);
    pstool.append_parameters(x);

    pstool.append_objectives(Obj());

    pstool.setSerializeAllDefaults(true);
    pstool.print("testing.xml");

    PSim::ParameterValueSet soln = pstool.run();
    soln.print("pvalset.xml");


    return 0;
}
