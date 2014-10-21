// What is the optimal angle at which to shoot off a point mass to get it to
// go as far as possible? (45 degrees)
//

#include <iostream>

#include <PSim/PSim.h>
#include <OpenSim/OpenSim.h>

class Obj : public PSim::Objective {
OpenSim_DECLARE_CONCRETE_OBJECT(Obj, PSim::Objective);
    SimTK::Real evaluate(const PSim::ParameterValueSet& pvalset,
            const Model& model,
            const SimTK::State& finalState) const
    {
        return pow(pvalset[0].get_value() - 3, 2);
    }
};

Model createModel()
{
    Model model;

    Body* body = new Body("projectile", 1, SimTK::Vec3(0), SimTK::Inertia(0));
    model.addBody(body);

    Joint* joint = new FreeJoint("free",
            model.getGroundBody(), SimTK::Vec3(0), SimTK::Vec3(0),
            *body, SimTK::Vec3(0), SimTK::Vec3(0));

    model.addJoint(joint);

    // TODO model.addModelComponent(HitGround());

    return model;
}

int main()
{
    PSim::Tool pstool;

    pstool.set_model(createModel());

    PSim::Parameter x;
    x.setName("x");
    x.set_default_value(5);
    pstool.append_parameters(x);

    pstool.append_objectives(Obj());

    pstool.setSerializeAllDefaults(true);
    pstool.print("testing.xml");

    PSim::ParameterValueSet soln = pstool.run();
    soln.print("pvalset.xml");

    return 0;
}
/**

#include <iostream>

#include <PSim/PSim.h>
#include <OpenSim/OpenSim.h>

class Obj : public PSim::Objective {
OpenSim_DECLARE_CONCRETE_OBJECT(Obj, PSim::Objective);
    SimTK::Real evaluate(const PSim::ParameterValueSet& pvalset,
            const Model& model,
            const SimTK::State& finalState) const
    {
        return pow(pvalset[0].get_value() - 3, 2);
    }
};

class Angle : public PSim::Parameter {
OpenSim_DECLARE_CONCRETE_OBJECT(Angle, PSim::Parameter);
//    virtual void apply(const double param,
//            Model& model, SimTK::State& initState) const {
//
//        const double v = 1;
//
//        const double vx = v * cos(param);
//        const double vy = v * sin(param);
//
//        const Coordinate& cx = model.getCoordinateSet().get("free_xTranslation");
//        cx.setSpeedValue(initState, vx);
//
//        const Coordinate& cy = model.getCoordinateSet().get("free_yTranslation");
//        cy.setSpeedValue(initState, vy);
//    }

};

class Range : public PSim::Objective {
OpenSim_DECLARE_CONCRETE_OBJECT(Range, PSim::Objective);
    SimTK::Real evaluate(const PSim::ParameterValueSet& pvalset,
            const Model& model,
            const SimTK::State& finalState) const
    {
//        const Coordinate& c = model.getCoordinateSet().get("free_xTranslation");
        return pow(pvalset[0].get_value(), 2);
//        return -c.getValue(finalState);
    }
};

// Event handlers.
// ===============
namespace OpenSim {
class TriggeredEventHandler : public ModelComponent
{
OpenSim_DECLARE_ABSTRACT_OBJECT(TriggeredEventHandler, ModelComponent);
public:
private:

};

class HitGround : public TriggeredEventHandler {
public:
    handleEvent(SimTK::State &state, SimTK::Real accuracy,
            bool &shouldTerminate) const
    {
    }
};

Model createModel()
{
    Model model;

    Body* body = new Body("projectile", 1, SimTK::Vec3(0), SimTK::Inertia(0));
    model.addBody(body);

    Joint* joint = new FreeJoint("free",
            model.getGroundBody(), SimTK::Vec3(0), SimTK::Vec3(0),
            *body, SimTK::Vec3(0), SimTK::Vec3(0));

    model.addJoint(joint);

    // TODO model.addModelComponent(HitGround());

    return model;
}

int main()
{
    PSim::Tool pstool;

    pstool.set_model(createModel());

    // Set up the optimization.
    // ========================
    PSim::Parameter angle;
    angle.setName("angle");
    angle.set_default_value(5);
    pstool.append_parameters(angle);

//    pstool.append_objectives(Range());
    pstool.append_objectives(Obj());

    pstool.setSerializeAllDefaults(true);
    pstool.print("testing.xml");

    PSim::ParameterValueSet soln = pstool.run();
    soln.print("pvalset.xml");


    return 0;
}
*/
