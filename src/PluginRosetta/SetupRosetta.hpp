#ifndef Antipatrea_SetupRosetta_HPP_
#define Antipatrea_SetupRosetta_HPP_

#include "Setup/Setup.hpp"
#include "PluginRosetta/MolecularStructureRosetta.hpp"
#include "PluginRosetta/CfgEnergyEvaluatorRosetta.hpp"
#include "PluginRosetta/CfgImproverRosetta.hpp"
#include "PluginRosetta/CfgOffspringGeneratorRosetta.hpp"
#include "PluginRosetta/CfgSamplerRosetta.hpp"
#include "PluginRosetta/CfgProjectorUSR.hpp"
#include "PluginRosetta/CfgProjectorDeltaR.hpp"


namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Setup the planning problem for a molecular structure based on Rosetta representation 
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
    class SetupRosetta : public Setup
    {
    public:    
	SetupRosetta(void) : Setup()
	{
	}
		
	virtual ~SetupRosetta(void)
	{
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get a pointer to the molecular structure.
  	 */
	virtual const MolecularStructureRosetta* GetMolecularStructureRosetta(void) const
	{
	    return dynamic_cast<const MolecularStructureRosetta*>(GetCfgForwardKinematics());
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get a pointer to the molecular structure.
  	 */
	virtual MolecularStructureRosetta* GetMolecularStructureRosetta(void) 
	{
	    return dynamic_cast<MolecularStructureRosetta*>(GetCfgForwardKinematics());
	}

	
	virtual void Prepare(Params & params);

	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Write whatever you would like for testing purposes.
	 *
	 *@remark
	 * - Add code to test various components. 
  	 */
	virtual void Test(void);
	
	
    protected:
	virtual void NewCfgForwardKinematics(Params & params)
	{
	    auto name = params.GetValue(Constants::KW_UseCfgForwardKinematics);
	    if(StrSameContent(name, Constants::KW_MolecularStructureRosetta))
	    {
		SetCfgForwardKinematics(new MolecularStructureRosetta());
		OnNewInstance(GetCfgForwardKinematics());
	    }
	    else
		Setup::NewCfgForwardKinematics(params);
	}
	
	virtual void NewCfgEnergyEvaluator(Params & params)
	{
	    auto name = params.GetValue(Constants::KW_UseCfgEnergyEvaluator);
	    if(StrSameContent(name, Constants::KW_CfgEnergyEvaluatorRosetta))
	    {
		SetCfgEnergyEvaluator(new CfgEnergyEvaluatorRosetta());
		OnNewInstance(GetCfgEnergyEvaluator());
	    }
	    else
		Setup::NewCfgEnergyEvaluator(params);
	}
	
	virtual void NewCfgImprover(Params & params)
	{
	    auto name = params.GetValue(Constants::KW_UseCfgImprover);
	    if(StrSameContent(name, Constants::KW_CfgImproverRosetta))
	    {
		SetCfgImprover(new CfgImproverRosetta());
		OnNewInstance(GetCfgImprover());
	    }
	    else
		Setup::NewCfgImprover(params);
	}

	virtual void NewCfgOffspringGenerator(Params & params)
	{
	    auto name = params.GetValue(Constants::KW_UseCfgOffspringGenerator);
	    if(StrSameContent(name, Constants::KW_CfgOffspringGeneratorRosetta))
	    {
		SetCfgOffspringGenerator(new CfgOffspringGeneratorRosetta());
		OnNewInstance(GetCfgOffspringGenerator());
	    }
	    else
		Setup::NewCfgOffspringGenerator(params);
	}

	
	virtual void NewCfgSampler(Params & params)
	{
	    auto name = params.GetValue(Constants::KW_UseCfgSampler);
	    if(StrSameContent(name, Constants::KW_CfgSamplerRosetta))
	    {
		SetCfgSampler(new CfgSamplerRosetta());
		OnNewInstance(GetCfgSampler());
	    }
	    else
		Setup::NewCfgSampler(params);
	}

	
	virtual void NewCfgProjector(Params & params)
	{
	    auto name = params.GetValue(Constants::KW_UseCfgProjector);
	    if(StrSameContent(name, Constants::KW_CfgProjectorUSR))
	    {
		SetCfgProjector(new CfgProjectorUSR());
		OnNewInstance(GetCfgProjector());
	    }
	    else if(StrSameContent(name, Constants::KW_CfgProjectorDeltaR))
	    {
		SetCfgProjector(new CfgProjectorDeltaR());
		OnNewInstance(GetCfgProjector());
	    }
	    else
		Setup::NewCfgProjector(params);
	}
	
	virtual void SetupPointersCfgEnergyEvaluator(void)
	{
	    Setup::SetupPointersCfgEnergyEvaluator();
	    
	    if(dynamic_cast<CfgEnergyEvaluatorRosetta*>(GetCfgEnergyEvaluator()))
	    {
		dynamic_cast<CfgEnergyEvaluatorRosetta*>(GetCfgEnergyEvaluator())->SetCfgManager(GetCfgManager());
		dynamic_cast<CfgEnergyEvaluatorRosetta*>(GetCfgEnergyEvaluator())->SetMolecularStructureRosetta(GetMolecularStructureRosetta());
	    }
	}

	virtual void SetupPointersCfgImprover(void)
	{
	    Setup::SetupPointersCfgImprover();
	    
	    if(dynamic_cast<CfgImproverRosetta*>(GetCfgImprover()))
	    {
		dynamic_cast<CfgImproverRosetta*>(GetCfgImprover())->SetCfgManager(GetCfgManager());
		dynamic_cast<CfgImproverRosetta*>(GetCfgImprover())->SetMolecularStructureRosetta(GetMolecularStructureRosetta());
	    }
	}

	
	virtual void SetupPointersCfgOffspringGenerator(void)
	{
	    Setup::SetupPointersCfgOffspringGenerator();
	    
	    if(dynamic_cast<CfgOffspringGeneratorRosetta*>(GetCfgOffspringGenerator()))
	    {
		dynamic_cast<CfgOffspringGeneratorRosetta*>(GetCfgOffspringGenerator())->SetCfgManager(GetCfgManager());
		dynamic_cast<CfgOffspringGeneratorRosetta*>(GetCfgOffspringGenerator())->SetMolecularStructureRosetta(GetMolecularStructureRosetta());
	    }
	}

	virtual void SetupPointersCfgSampler(void)
	{
	    Setup::SetupPointersCfgSampler();
	    
	    if(dynamic_cast<CfgSamplerRosetta*>(GetCfgSampler()))
	    {
		dynamic_cast<CfgSamplerRosetta*>(GetCfgSampler())->SetCfgManager(GetCfgManager());
		dynamic_cast<CfgSamplerRosetta*>(GetCfgSampler())->SetMolecularStructureRosetta(GetMolecularStructureRosetta());
	    }
	}

	virtual void SetupPointersCfgProjector(void)
	{
	    Setup::SetupPointersCfgProjector();
	    
	    if(dynamic_cast<CfgProjectorUSR*>(GetCfgProjector()))
	    {
		dynamic_cast<CfgProjectorUSR*>(GetCfgProjector())->SetCfgManager(GetCfgManager());
		dynamic_cast<CfgProjectorUSR*>(GetCfgProjector())->SetMolecularStructureRosetta(GetMolecularStructureRosetta());
	    }
	    else if(dynamic_cast<CfgProjectorDeltaR*>(GetCfgProjector()))
	    {
		dynamic_cast<CfgProjectorDeltaR*>(GetCfgProjector())->SetCfgManager(GetCfgManager());
		dynamic_cast<CfgProjectorDeltaR*>(GetCfgProjector())->SetMolecularStructureRosetta(GetMolecularStructureRosetta());
	    }
	}



    };
    
}

#endif
