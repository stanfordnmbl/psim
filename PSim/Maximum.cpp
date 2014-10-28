
#include "Maximum.h"

namespace PSim {

// This Measure returns a derivative only at the Acceleration stage.
// Informed by Probe.
template <class T>
class InputMeasure : public SimTK::Measure_<T> {
public:
    SimTK_MEASURE_HANDLE_PREAMBLE(InputMeasure, SimTK::Measure_<T>);
    InputMeasure(SimTK::Subsystem& sub, const Maximum& max)
        : SimTK::Measure(sub, new Implementation(max),
                SimTK::AbstractMeasure::SetHandle()) {}
    SimTK_MEASURE_HANDLE_POSTSCRIPT(InputMeasure, SimTK::Measure_<T>);
};
template <class T>
class InputMeasure<T>::Implementation
        : public SimTK::Measure_<T>::Implementation {
public:
    Implementation(const Maximum& max)
        : SimTK::Measure_<T>::Implementation(1), m_max(max) {}

    // Implementations of virtual methods.
    Implementation* cloneVirtual() const override
    {   return new Implementation(*this); }
    int getNumTimeDerivativesVirtual() const override { return 0; }

    SimTK::Stage getDependsOnStageVirtual(int order) const override
    {   return SimTK::Stage::Position; }// TODO m_max.getInput("input").getConnectee().getDependsOnStage(); }

    void calcCachedValueVirtual(const SimTK::State& s, int derivOrder,
            T& value) const override {
        SimTK_ASSERT1_ALWAYS(derivOrder==0,
                "PSim::InputMeasure<T>::Implementation::"
                "calcCachedValueVirtual(): "
                "derivOrder %d seen but only 0 allowed.", derivOrder);
        // TODO value = m_max.getInputValue<double>(s, "input");
    }
private:
    const Maximum& m_max;
};

void Maximum::addToSystem(SimTK::MultibodySystem& system) const
{
    Super::addToSystem(system);

    InputMeasure<SimTK::Real> inputMeasure(system, *this);

    const_cast<Maximum*>(this)->m_measure =
        SimTK::Measure::Maximum(system, inputMeasure);

}

} // namespace PSim
