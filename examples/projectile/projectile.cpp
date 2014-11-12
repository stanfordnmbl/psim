// What is the optimal angle at which to shoot off a point mass to get it to
// go as far as possible? (45 degrees)

#include <iostream>

#include <PSim/osimPSim.h>
#include <OpenSim/OpenSim.h>
#include <OpenSim/Simulation/SimbodyEngine/PlanarJoint.h>

// Parameters.
// ===========
// Angle of the projectile's initial velocity with the horizontal, in degrees.
class Angle : public OpenSim::PSimParameter {
OpenSim_DECLARE_CONCRETE_OBJECT(Angle, OpenSim::PSimParameter);
    void extendApplyToInitialState(const double param,
            const Model& model, SimTK::State& initState) const override {

        const double angle = param * SimTK::Pi / 180.0;

        const double v = 3;

        const double vx = v * cos(angle);
        const double vy = v * sin(angle);

        const Coordinate& cx = model.getCoordinateSet().get("x");
        cx.setSpeedValue(initState, vx);

        const Coordinate& cy = model.getCoordinateSet().get("y");
        cy.setSpeedValue(initState, vy);
    }
};

// Objectives.
// ===========
class Range : public OpenSim::PSimGoal {
OpenSim_DECLARE_CONCRETE_OBJECT(Range, OpenSim::PSimGoal);
    SimTK::Real extendEvaluate(const OpenSim::PSimParameterValueSet & pvalset,
            const OpenSim::StateTrajectory& states) const override
    {
        const Coordinate& c = getModel().getCoordinateSet().get("x");
        return -c.getValue(states.back());
    }
};

// TODO put this in a different example.
class Test : public OpenSim::IntegratingGoal {
OpenSim_DECLARE_CONCRETE_OBJECT(Test, OpenSim::IntegratingGoal);
    SimTK::Real integrand(const SimTK::State& s) const override {
        return 1;
    }
    void realizeReport(const SimTK::State& s) const { 
        // TODO std::cout << "DEBUG report" << std::endl;
    }
};

class MaxHeight : public OpenSim::PSimGoal {
OpenSim_DECLARE_CONCRETE_OBJECT(MaxHeight, OpenSim::PSimGoal);
public:

    class Max : public OpenSim::Maximum {
    OpenSim_DECLARE_CONCRETE_OBJECT(Max, OpenSim::Maximum);
    public:
        Max(const MaxHeight* mh) : mh(mh) {}
        double getInputVirtual(const SimTK::State& s) const override {
            return mh->height(s);
        }
        SimTK::Stage getDependsOnStageVirtual() const override {
            return SimTK::Stage::Position;
        }
    private:
        const MaxHeight* mh;
    };

    MaxHeight() : m_max(this) {
        //constructInfrastructure();
    }
    MaxHeight(const MaxHeight& mh) : OpenSim::PSimGoal(mh), m_max(this) {}
    SimTK::Real extendEvaluate(const OpenSim::PSimParameterValueSet & pvalset,
            const OpenSim::StateTrajectory& states) const override {
        return -m_max.maximum(states.back());
    }
    double height(const SimTK::State& s) const {
        return getModel().getCoordinateSet().get("y").getValue(s);
    }
private:
    /*
    // TODO should not be necessary.
    void constructProperties() override {}
    void constructOutputs() override {
        constructOutput<double>("height", &MaxHeight::height,
                SimTK::Stage::Position);
    }
    */

    void connectToModel(Model& model) override {
        Super::connectToModel(model);
        addComponent(&m_max);
        // m_max.getInput("input").connect(getOutput("height"));
    }
    Max m_max;
};


// Event handlers.
// ===============
namespace OpenSim {

class TriggeredEventHandler : public ModelComponent
{
OpenSim_DECLARE_ABSTRACT_OBJECT(TriggeredEventHandler, ModelComponent);
public:
    OpenSim_DECLARE_PROPERTY(required_stage, int,
            "The stage at which the event occurs.");

    TriggeredEventHandler() {
        constructProperties();
    }

    virtual void handleEvent(SimTK::State& state, SimTK::Real accuracy,
            bool& shouldTerminate) const = 0;
    virtual SimTK::Real getValue(const SimTK::State& s) const = 0;

private:

    void constructProperties() {
        constructProperty_required_stage(SimTK::Stage::Dynamics);
    }

    void extendAddToSystem(SimTK::MultibodySystem& system) const override {
        // TODO is this okay for memory?
        system.addEventHandler(
                new SimbodyHandler(SimTK::Stage(get_required_stage()), this));
    }

    class SimbodyHandler : public SimTK::TriggeredEventHandler {
    public:
         SimbodyHandler(SimTK::Stage requiredStage,
                 const OpenSim::TriggeredEventHandler* handler) :
             SimTK::TriggeredEventHandler(requiredStage), m_handler(handler) {}
        void handleEvent(SimTK::State& state, SimTK::Real accuracy,
                bool& shouldTerminate) const override
        { m_handler->handleEvent(state, accuracy, shouldTerminate); }
        SimTK::Real getValue(const SimTK::State& s) const override
        { return m_handler->getValue(s); }
    private:
        const OpenSim::TriggeredEventHandler* m_handler;
    };
};

} // namespace OpenSim

// End the simulation when y = 0.
class HitGround : public TriggeredEventHandler {
OpenSim_DECLARE_CONCRETE_OBJECT(HitGround, TriggeredEventHandler);
public:
    void handleEvent(SimTK::State& state, SimTK::Real accuracy,
            bool& shouldTerminate) const override
    { shouldTerminate = true; }
    SimTK::Real getValue(const SimTK::State& s) const override {
        const Coordinate& c = getModel().getCoordinateSet().get("y");
        return c.getValue(s);
    }
};

Model createModel()
{
    Model model;

    Body* body = new Body("projectile", 1, SimTK::Vec3(0), SimTK::Inertia(1));
    model.addBody(body);

    Joint* joint = new PlanarJoint("joint",
            model.getGroundBody(), SimTK::Vec3(0), SimTK::Vec3(0),
            *body, SimTK::Vec3(0), SimTK::Vec3(0));
    joint->getCoordinateSet().get(0).setName("theta");
    joint->getCoordinateSet().get(1).setName("x");
    joint->getCoordinateSet().get(2).setName("y");
    model.addJoint(joint);

    model.addModelComponent(new HitGround());

    return model;
}

int main()
{
    // Set up tool.
    // ============
    OpenSim::PSimTool pstool;
    //pstool.set_visualize(true);

    pstool.setBaseModel(createModel());

    // Set up parameters.
    // ==================
    Angle angle;
    angle.setName("angle");
    angle.set_default_value(80);
    angle.set_lower_limit(1);
    angle.set_upper_limit(90);
    angle.set_lower_opt(0);
    angle.set_upper_opt(90);
    pstool.append_parameters(angle);

    // Set up objectives.
    // ==================
    Range range;
    range.set_weight(1);
    pstool.append_goals(range);
    Test integratingObj;
    integratingObj.set_weight(0);
    pstool.append_goals(integratingObj);
    MaxHeight maxHeight;
    maxHeight.set_weight(0);
    pstool.append_goals(maxHeight);

    // Wrap up.
    // ========
    pstool.setSerializeAllDefaults(true);
    pstool.print("PSimToolSetup.xml");

    OpenSim::PSimParameterValueSet soln = pstool.solve();
    soln.print("solution.xml");

    return 0;
}
