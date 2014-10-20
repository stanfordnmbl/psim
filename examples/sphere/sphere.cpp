#include <iostream>

#include <PSim/PSim.h>

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

    pstool.setSerializeAllDefaults(true);
    pstool.print("testing.xml");

    PSim::ParameterValueSet soln = pstool.run();
    soln.print("pvalset.xml");


    return 0;
}
