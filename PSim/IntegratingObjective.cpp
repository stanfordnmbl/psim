
#include "IntegratingObjective.h"

namespace PSim {

// This Measure returns a derivative only at the Acceleration stage.
// Informed by Probe.
template <class T>
class DerivMeasure : public SimTK::Measure_<T> {
public:
    SimTK_MEASURE_HANDLE_PREAMBLE(DerivMeasure, SimTK::Measure_<T>);
    DerivMeasure(SimTK::Subsystem& sub, const IntegratingObjective& obj)
        : SimTK::Measure(sub, new Implementation(obj),
                SimTK::AbstractMeasure::SetHandle()) {}
    SimTK_MEASURE_HANDLE_POSTSCRIPT(DerivMeasure, SimTK::Measure_<T>);
};
template <class T>
class DerivMeasure<T>::Implementation
        : public SimTK::Measure_<T>::Implementation {
public:
    Implementation(const IntegratingObjective& obj)
        : SimTK::Measure_<T>::Implementation(1), m_obj(obj) {}

    // Implementations of virtual methods.
    Implementation* cloneVirtual() const override
    {   return new Implementation(*this); }
    int getNumTimeDerivativesVirtual() const override { return 0; }
    SimTK::Stage getDependsOnStageVirtual(int order) const override
    {   return SimTK::Stage::Acceleration; }
    void calcCachedValueVirtual(const SimTK::State& s, int derivOrder,
            T& value) const override {
        SimTK_ASSERT1_ALWAYS(derivOrder==0,
                "DerivMeasure<T>::Implementation::"
                "calcCachedValueVirtual(): "
                "derivOrder %d seen but only 0 allowed.", derivOrder);
        value = m_obj.derivative(s);
    }
private:
    const IntegratingObjective& m_obj;
};

void IntegratingObjective::addToSystem(SimTK::MultibodySystem& system) const
{
    Super::addToSystem(system);

    DerivMeasure<SimTK::Real> derivMeasure(system, *this);
    SimTK::Measure::Constant initCond(system, 0.0);

    const_cast<IntegratingObjective*>(this)->m_integrateMeasure =
        SimTK::Measure::Integrate(system, derivMeasure, initCond);

}

} // namespace PSim
