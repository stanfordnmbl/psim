#include "Objective.h"

namespace PSim {

Objective::Objective()
{
    constructProperties();
}

void Objective::constructProperties()
{
    constructProperty_enabled(true);
    constructProperty_weight(1);
}

} // namespace PSim