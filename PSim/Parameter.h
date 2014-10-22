#ifndef PSIM_PARAMETER_H_
#define PSIM_PARAMETER_H_

#include <OpenSim/Common/Object.h>
#include <OpenSim/Simulation/Model/Model.h>

using namespace OpenSim;

namespace PSim {

// TODO template<int N>
/**
* A description of how an optimizer parameter affects a Model.
* The lower_opt property is the value used in the optimizer to represent
* the lower_limit value. Likewise for upper_opt. These properties
* are used for normalizing the parameter values so that the optimizer
* can operate with values that have similar ranges.
* For example, consider an upper_limit of 10 and upper_opt of 2,
* with lower_limit and lower_opt as 0.
* In this case, the optimizer is given an upper limit of 5.
* A parameter value of 4 would be given to the optimizer as 2.
* By default, the parameters are not normalized when sent to the optimizer.
* For the parameter to remain in the range [0, 1],
* set the lower_opt to lower_limit, and upper_opt to upper_limit.
* This affects how explorative the optimizer is for this parameter.
* For a fixed standard deviation (CMAES step size, sigma),
* a larger difference (upper_opt - lower_opt) causes more exploration.
*/
class Parameter : public OpenSim::Object
{
OpenSim_DECLARE_CONCRETE_OBJECT(PSim::Parameter, OpenSim::Object);
public:

    /// @name Property declarations
    /// @{
    OpenSim_DECLARE_PROPERTY(optimize, bool,
            "Optimize this parameter (false means default value is used).");
    OpenSim_DECLARE_PROPERTY(lower_limit, double,
            "Lower limit for the parameter; for optimizer bounds. Default: -Inf");
    OpenSim_DECLARE_PROPERTY(upper_limit, double,
            "Upper limit for the parameter; for optimizer bounds. Default: Inf");
    OpenSim_DECLARE_PROPERTY(lower_opt, double,
            "The value of lower_limit used in the optimizer. Default: 0");
    OpenSim_DECLARE_PROPERTY(upper_opt, double,
            "The value of upper_limit used in the optimizer. Default: 1");
    OpenSim_DECLARE_PROPERTY(default_value, double,
            "Value used if optimize=false; & default initial guess. Default: 0.");
    /// @}

    Parameter();

    /// The number of scalar parameters represented by this object.
    unsigned int numScalarParameters() const { return 1; }

    /// This is the core method of this class. Apply the given
    /// parameter to the Model and the initial state of the simulation.
    virtual void apply(const double param,
            Model& model, SimTK::State& initState) const {}

    /// This is used to compute a value that can be sent to
    /// to the optimizer (e.g., for initial parameters).
    double normalized(double param) const;

    /// This is used to convert a value coming from an optimizer into one that
    /// can be stored somewhere in the Model or State.
    double unnormalized(double normalizedParam) const;

private:
    void constructProperties();

};

} // namespace PSim

#endif // PSIM_PARAMETER_H_
