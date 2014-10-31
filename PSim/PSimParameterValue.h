#ifndef OPENSIM_PSIM_PARAMETER_VALUE_H_
#define OPENSIM_PSIM_PARAMETER_VALUE_H_

#include <OpenSim/Common/Object.h>
#include <OpenSim/Common/Set.h>

using namespace OpenSim;

namespace OpenSim {
/**
 * Stores the value of a parameter. This value is *not* scaled.
 * That is, it could be used directly in the model, and is not suited to be
 * used directly in the optimization. The name of the PSimParameterValue object
 * must match the name of a PSimParameter; otherwise, the value might be ignored.
 * TODO help detect unused values.
 */
class PSimParameterValue : public Object
{
OpenSim_DECLARE_CONCRETE_OBJECT(PSimParameterValue, Object);
public:

    /// @name Property declarations
    /// @{
    OpenSim_DECLARE_PROPERTY(value, double, "Value of the PSimParameter.")
    /// @}

    PSimParameterValue()
    {
        constructProperties();
    }

private:
    void constructProperties()
    {
        constructProperty_value(0);
    }

};

// TODO do I need "typename"?
// TODO for serialization, make this a class? or just register it?
typedef typename OpenSim::Set<OpenSim::PSimParameterValue> PSimParameterValueSet;

} // namespace OpenSim
#endif // OPENSIM_PSIM_PARAMETER_VALUE_H_
