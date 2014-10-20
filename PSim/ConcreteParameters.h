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
    OpenSim_DECLARE_PROPERTY(coordinate, std::string,
            "Name of the coordinate.");
    /// @}
    void apply(const double param, Model& model, SimTK::State& initState)
            const override {
        model.getCoordinateSet().get(get_coordinate()).setValue(initState, param);
    }

};

class CoordinateInitialSpeedParameter : public Parameter
{
OpenSim_DECLARE_CONCRETE_OBJECT(PSim::CoordinateInitialSpeedParameter,
        Parameter);
public:
    /// @name Property declarations
    /// @{
    OpenSim_DECLARE_PROPERTY(coordinate, std::string,
            "Name of the coordinate.");
    /// @}
    void apply(const double param, Model& model, SimTK::State& initState)
            const override {
        model.updCoordinateSet().get(get_coordinate()).
            setSpeedValue(initState, param);
    }

};

} // namespace PSim

#endif // PSIM_CONCRETE_PARAMETERS_H_
