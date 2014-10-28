#ifndef PSIM_MAXIMUM_H_
#define PSIM_MAXIMUM_H_

#include <OpenSim/Common/Component.h>

namespace PSim {

class Maximum : public OpenSim::Component
{
OpenSim_DECLARE_CONCRETE_OBJECT(Maximum, OpenSim::Component);
public:

    Maximum() {
        std::cout << "DEBUGMAXCONSTRUCT" << std::endl;
        constructInfrastructure();
    }

    double maximum(const SimTK::State& s) const {
        return m_measure.getValue(s);
    }

private:

    void constructInputs() override {
        std::cout << "DEBUGMAXINPUT" << std::endl;
        constructInput<double>("input", SimTK::Stage::Position);
    }

    void constructOutputs() override {
        constructOutput<double>("maximum", &Maximum::maximum,
                SimTK::Stage::Velocity);
    }

    void addToSystem(SimTK::MultibodySystem& system) const override;

    SimTK::Measure m_measure;

};

} // namespace PSim

#endif // PSIM_MAXIMUM_H_
