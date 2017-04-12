#ifndef Antipatrea_Setup2D_HPP_
#define Antipatrea_Setup2D_HPP_

#include "Setup/Setup.hpp"
#include "Plugin2D/Scene2D.hpp"
#include "Planners/PGT.hpp"

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Facilitate setting up planning problems in 2D workspaces
     *       as well as the planner and its components.
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
    class Setup2D : public Setup,
		    public Scene2DContainer
    {
    public:    
	Setup2D(void) : Setup(),
			Scene2DContainer()
	{
	}
	
	
	virtual ~Setup2D(void)
	{
	}

    protected:
	virtual void NewInstances(Params & params)
	{
	    NewScene2D(params);
	    Setup::NewInstances(params);
	}
	
	/**
	 *@copydoc Setup::NewCfgAcceptor(Params &)
	 */
	virtual void NewScene2D(Params & params)
	{
	    SetScene2D(new Scene2D());
	    OnNewInstance(GetScene2D());
	}

	virtual void SetupPointers(void)
	{
	    SetupPointersScene2D();
	    Setup::SetupPointers();
	}
	
	/**
	 *@copydoc Setup::SetupPointersComponent(Component * const)
	 */
	virtual void SetupPointersScene2D(void)
	{
	    SetupPointersComponent(GetScene2D());
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Fix grid dimensions.
	 *
	 *@remarks
	 *  - If PGT has been selected as the planner, 
	 *    fix the grid dimensions to match the bounding box of the scene.
	 */
	virtual void FixGrid(void)
	{
	    auto pgt = dynamic_cast<PGT*>(GetSamplingBasedPlanner());
	    if(pgt)
	    {
		pgt->GetProjectionGrid()->Setup(2, pgt->GetProjectionGrid()->GetDims(),
				      GetScene2D()->GetBoundingBox(),
				      &(GetScene2D()->GetBoundingBox()[2]));
	    }
	    
	}
	
	
    };

}

#endif
