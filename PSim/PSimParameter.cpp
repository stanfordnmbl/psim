#include "PSimParameter.h"

#include <OpenSim/Common/Array.h>

using namespace OpenSim;

PSimParameter::PSimParameter() {
    constructProperties();
}

void PSimParameter::constructProperties() {
    constructProperty_optimize(true);

    constructProperty_lower_limit(-SimTK::Infinity);
    constructProperty_upper_limit(SimTK::Infinity);
    constructProperty_lower_opt(0);
    constructProperty_upper_opt(1);

    constructProperty_default_value(0);
}

double PSimParameter::normalized(double param) const {
    return (param - get_lower_opt()) / (get_upper_opt() - get_lower_opt());
}

double PSimParameter::unnormalized(double normalizedParam) const {
    return normalizedParam * (get_upper_opt() - get_lower_opt()) +
        get_lower_opt();
}
