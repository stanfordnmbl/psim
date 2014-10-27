#ifndef PSIM_OBJECTIVE_H_
#define PSIM_OBJECTIVE_H_

#include <OpenSim/Common/Object.h>
#include <OpenSim/Simulation/Model/Model.h>
#include "ParameterValue.h"

using namespace OpenSim;

namespace PSim {

// TODO use lepton!!!

class Objective : public OpenSim::ModelComponent
{
OpenSim_DECLARE_ABSTRACT_OBJECT(PSim::Objective, OpenSim::ModelComponent);
public:

    /// @name Property declarations
    /// @{
    OpenSim_DECLARE_PROPERTY(enabled, bool,
        "If true, this term is added into the objective function.");
    OpenSim_DECLARE_PROPERTY(weight, double,
        "Coefficient of this term in the sum across objectives.");
    /// @}

    Objective();

    /// Evaluate this objective. This quantity is multiplied by the weight
    /// property of this Objective, and is then added to the objective
    /// function.
    virtual SimTK::Real evaluate(const ParameterValueSet& pvalset,
            const Model& model,
            const SimTK::State& finalState) const = 0;

private:
    void constructProperties();

};

class IntegratingObjective : public Objective
{
OpenSim_DECLARE_ABSTRACT_OBJECT(PSim::IntegratingObjective, Objective);
public:

    virtual double derivative(const SimTK::State& s) const = 0;
    SimTK::Real evaluate(const ParameterValueSet& pvalset,
            const Model& model,
            const SimTK::State& finalState) const override
    {
        return m_integrateMeasure.getValue(finalState);
    }

private:

    void addToSystem(SimTK::MultibodySystem& system) const override {
        Super::addToSystem(system);

        DerivMeasure derivMeasure(system, *this);
        SimTK::Measure::Constant initCond(system, 0.0);

        const_cast<IntegratingObjective*>(this)->m_integrateMeasure =
            SimTK::Measure::Integrate(system, derivMeasure, initCond);

    }

    // This Measure returns a derivative only at the Acceleration stage.
    // Informed by Probe.
    class DerivMeasure : public SimTK::Measure {
    public:
        SimTK_MEASURE_HANDLE_PREAMBLE(DerivMeasure, SimTK::Measure);
        DerivMeasure(SimTK::Subsystem& sub, const IntegratingObjective& obj)
            : SimTK::Measure(sub, new Implementation(obj),
                    AbstractMeasure::SetHandle()) {}
        SimTK_MEASURE_HANDLE_POSTSCRIPT(DerivMeasure, SimTK::Measure);
    };
    class DerivMeasure::Implementation : public SimTK::Measure::Implementation {
    public:
        Implementation(const IntegratingObjective& obj)
            : SimTK::Measure::Implementation(1), m_obj(obj) {}

        // Implementations of virtual methods.
        Implementation* cloneVirtual() const override
        {   return new Implementation(*this); }
        int getNumTimeDerivativesVirtual() const override { return 0; }
        SimTK::Stage getDependsOnStageVirtual(int order) const override
        {   return SimTK::Stage::Acceleration; }
        void calcCachedValueVirtual(const SimTK::State& s, int derivOrder,
                SimTK::Real& value) const override {
            SimTK_ASSERT1_ALWAYS(derivOrder==0,
                    "IntegratingObjective::DerivMeasure::Implementation::"
                    "calcCachedValueVirtual(): "
                    "derivOrder %d seen but only 0 allowed.", derivOrder);
            value = m_obj.derivative(s);
        }
    private:
        const IntegratingObjective& m_obj;
    };

    SimTK::Measure m_integrateMeasure;
 
};

/// An Objective whose value depends on a Probe in the Model.
class ProbeObjective : public Objective
{
    void constructConnectors() {
        constructConnector<Probe>("probe");
    }
};

} // namespace PSim

#endif // PSIM_PARAMETER_H_
