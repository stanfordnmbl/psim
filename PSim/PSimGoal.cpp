#include "PSimGoal.h"

using namespace OpenSim;

PSimGoal::PSimGoal()
{
    constructProperties();
}

void PSimGoal::constructProperties()
{
    constructProperty_enabled(true);
    constructProperty_weight(1);
}
