#ifndef PSIM_CONCRETE_PARAMETERS_H_
#define PSIM_CONCRETE_PARAMETERS_H_

#include "Parameter.h"

namespace PSim {

class CoordinateInitialValueParameter : public Parameter
{
OpenSim_DECLARE_CONCRETE_OBJECT(PSim::CoordinateInitialValueParameter,
        Parameter);
public:
    /// @name Property declarations
    /// @{
    OpenSim_DECLARE_PROPERTY(coordinate_name, std::string,
            "Name of the coordinate.");
    /// @}

    CoordinateInitialValueParameter()
    {
        constructProperties();
    }

    void apply(const double param, Model& model, SimTK::State& initState)
            const override {
        model.getCoordinateSet().get(get_coordinate_name()).
                setValue(initState, param);
    }

private:
    void constructProperties()
    {
        constructProperty_coordinate_name("");
    }

};

class CoordinateInitialSpeedParameter : public Parameter
{
OpenSim_DECLARE_CONCRETE_OBJECT(PSim::CoordinateInitialSpeedParameter,
        Parameter);
public:
    /// @name Property declarations
    /// @{
    OpenSim_DECLARE_PROPERTY(coordinate_name, std::string,
            "Name of the coordinate.");
    /// @}
    void apply(const double param, Model& model, SimTK::State& initState)
            const override {
        model.updCoordinateSet().get(get_coordinate_name()).
            setSpeedValue(initState, param);
    }

private:
    void constructProperties()
    {
        constructProperty_coordinate_name("");
    }

};

} // namespace PSim

#endif // PSIM_CONCRETE_PARAMETERS_H_
