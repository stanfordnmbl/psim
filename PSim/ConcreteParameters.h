#ifndef OPENSIM_PSIM_CONCRETE_PARAMETERS_H_
#define OPENSIM_PSIM_CONCRETE_PARAMETERS_H_

#include "PSimParameter.h"

namespace OpenSim {

/** The value of a coordinate (translation or rotation) at the start
 * of the motion.
 */
class PSimCoordInitialValueParameter : public PSimParameter
{
OpenSim_DECLARE_CONCRETE_OBJECT(PSimCoordInitialValueParameter, PSimParameter);
public:
    /// @name Property declarations
    /// @{
    OpenSim_DECLARE_PROPERTY(coordinate_name, std::string,
            "Name of the coordinate.");
    /// @}

    PSimCoordInitialValueParameter() { constructProperties(); }

    void extendApplyToInitialState(const double param,
            const Model& model, SimTK::State& initState) const override {
        model.getCoordinateSet().get(get_coordinate_name()).
                setValue(initState, param);
    }

private:
    void constructProperties() { constructProperty_coordinate_name(""); }

};

/** The speed for a coordinate at the start of a motion.
 */
class PSimCoordInitialSpeedParameter : public PSimParameter
{
OpenSim_DECLARE_CONCRETE_OBJECT(PSimCoordInitialSpeedParameter, PSimParameter);
public:
    /// @name Property declarations
    /// @{
    OpenSim_DECLARE_PROPERTY(coordinate_name, std::string,
            "Name of the coordinate.");
    /// @}

    PSimCoordInitialSpeedParameter() { constructProperties(); }

    void extendApplyToInitialState(const double param,
            const Model& model, SimTK::State& initState) const override {
        model.getCoordinateSet().get(get_coordinate_name()).
            setSpeedValue(initState, param);
    }

private:
    void constructProperties() { constructProperty_coordinate_name(""); }

};

} // namespace OpenSim

#endif // OPENSIM_PSIM_CONCRETE_PARAMETERS_H_
