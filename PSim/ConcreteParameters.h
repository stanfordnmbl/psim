#ifndef OPENSIM_PSIM_CONCRETE_PARAMETERS_H_
#define OPENSIM_PSIM_CONCRETE_PARAMETERS_H_

#include "PSimParameter.h"

namespace OpenSim {

class CoordinateInitialValueParameter : public PSimParameter
{
OpenSim_DECLARE_CONCRETE_OBJECT(CoordinateInitialValueParameter,
        PSimParameter);
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

class CoordinateInitialSpeedParameter : public PSimParameter
{
OpenSim_DECLARE_CONCRETE_OBJECT(CoordinateInitialSpeedParameter,
        PSimParameter);
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

} // namespace OpenSim

#endif // OPENSIM_PSIM_CONCRETE_PARAMETERS_H_
