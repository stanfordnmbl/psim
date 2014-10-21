#include "Parameter.h"

#include <OpenSim/Common/Array.h>

namespace PSim {

Parameter::Parameter() {
    constructProperties();
}

void Parameter::constructProperties() {
    constructProperty_optimize(true);

    constructProperty_lower_limit(-SimTK::Infinity);
    constructProperty_upper_limit(SimTK::Infinity);
    constructProperty_lower_opt(0);
    constructProperty_upper_opt(1);

    constructProperty_default_value(0);
}

double Parameter::normalized(double param) const {
    return (param - get_lower_opt()) / (get_upper_opt() - get_lower_opt());
}

double Parameter::unnormalized(double normalizedParam) const {
    return normalizedParam * (get_upper_opt() - get_lower_opt()) +
        get_lower_opt();
}

} // namespace PSim
