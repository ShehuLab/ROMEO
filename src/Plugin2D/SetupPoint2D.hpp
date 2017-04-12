#ifndef Antipatrea_SetupPoint2D_HPP_
#define Antipatrea_SetupPoint2D_HPP_

#include "Plugin2D/Setup2D.hpp"
#include "Plugin2D/CfgAcceptorPoint2D.hpp"
#include "Plugin2D/CfgSamplerPoint2D.hpp"
#include "Plugin2D/CfgProjectorPoint2D.hpp"

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Setup the planning problem for a point robot 
     *       operating in a 2D scene as well as the planner and its components.
     *
     *@remarks
     * - Setup creates instances of the planning components based on
     *   keywords provided by the user or default settings
     *   when the user does not provide any keywords.
     * - Only one instance per class is created.
     * - After creating the components, Setup sets the pointers
     *   so that components/planners that depend on, for example, CfgDistance
     *   will have the CfgDistance pointer set to the CfgDistance
     *   instance created in the first step.
     * - After setting the pointers, Setup iterates over all the instances
     *   that were created in step 1, and calls the function SetupFromParams
     *   in order to set the parameter values for each instance.
     */ 
    class SetupPoint2D : public Setup2D
    {
    public:    
	SetupPoint2D(void) : Setup2D()
	{
	}
	
	
	virtual ~SetupPoint2D(void)
	{
	}
	
	virtual void Prepare(Params & params);
	
    protected:
	virtual void NewCfgAcceptor(Params & params)
	{
	    SetCfgAcceptor(new CfgAcceptorPoint2D());
	    OnNewInstance(GetCfgAcceptor());
	}
	
	virtual void NewCfgSampler(Params & params)
	{
	    SetCfgSampler(new CfgSamplerPoint2D());
	    OnNewInstance(GetCfgSampler());
	}
	
	virtual void NewCfgProjector(Params & params)
	{
	    SetCfgProjector(new CfgProjectorPoint2D());
	    OnNewInstance(GetCfgProjector());
	}
	
	virtual void SetupPointersCfgAcceptor(void)
	{
	    Setup::SetupPointersCfgAcceptor();

	    if(dynamic_cast<CfgAcceptorPoint2D*>(GetCfgAcceptor()))
		dynamic_cast<CfgAcceptorPoint2D*>(GetCfgAcceptor())->SetScene2D(GetScene2D());
	}
	
	virtual void SetupPointersCfgSampler(void)
	{
	    Setup::SetupPointersCfgSampler();
	    if(dynamic_cast<CfgSamplerPoint2D*>(GetCfgSampler()))
		dynamic_cast<CfgSamplerPoint2D*>(GetCfgSampler())->SetScene2D(GetScene2D());
	}
	
    };

}

#endif
