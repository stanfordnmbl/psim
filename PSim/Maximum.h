#ifndef PSIM_MAXIMUM_H_
#define PSIM_MAXIMUM_H_

#include <OpenSim/Common/Component.h>

namespace PSim {

class Maximum : public OpenSim::Component
{
OpenSim_DECLARE_ABSTRACT_OBJECT(Maximum, OpenSim::Component);
public:

    Maximum() {
        constructInfrastructure();
    }

    double maximum(const SimTK::State& s) const {
        return m_measure.getValue(s);
    }

    double getInput(const SimTK::State& s) const {
        return getInputVirtual(s);
    }

    SimTK::Stage getDependsOnStage() const {
        return getDependsOnStageVirtual();
    }

protected:

    virtual double getInputVirtual(const SimTK::State& s) const = 0;
    virtual SimTK::Stage getDependsOnStageVirtual() const = 0;

private:

    /*
    void constructInputs() override {
        constructInput<double>("input", SimTK::Stage::Model);
    }

    void constructOutputs() override {
        constructOutput<double>("maximum", &Maximum::maximum,
                SimTK::Stage::Velocity);
    }
    */

    void addToSystem(SimTK::MultibodySystem& system) const override;

    SimTK::Measure m_measure;

};

} // namespace PSim

#endif // PSIM_MAXIMUM_H_
