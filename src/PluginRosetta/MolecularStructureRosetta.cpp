#include "PluginRosetta/MolecularStructureRosetta.hpp"
#include "Setup/Defaults.hpp"
#include "Utils/GDraw.hpp"
#include <cmath>

namespace Antipatrea
{
    MolecularStructureRosetta::MolecularStructureRosetta(void) : CfgForwardKinematics()
    {
	//add additional initialization code
    }
    
    MolecularStructureRosetta::~MolecularStructureRosetta(void)
    {
	//delete any memory allocated by this class
    }

    bool MolecularStructureRosetta::CheckSetup(void) const
    {
	return CfgForwardKinematics::CheckSetup();
	//do additional checks if necessary
	//see src/Components/CfgDistances/CfgDistanceLp.hpp for an example
    }
    
    void MolecularStructureRosetta::Info(const char prefix[]) const
    {
	CfgForwardKinematics::Info(prefix);
	//print any additional info
	//see src/Components/CfgDistances/CfgDistanceLp.hpp for an example
    }
    
    void MolecularStructureRosetta::SetupFromParams(Params & params)
    {
	CfgForwardKinematics::SetupFromParams(params);

	auto data = params.GetData(Constants::KW_MolecularStructureRosetta);
	if(data && data->m_params)
	{
	    //setup any other parameters that you may have
	    //see src/Components/CfgDistances/CfgDistanceLp.hpp for an example
	}
    }
    
    void MolecularStructureRosetta::DoFK(void)
    {
	//implement forward kinematics
	//use the vector m_joints to get the joint values
    }

    void MolecularStructureRosetta::Draw(void)
    {
	//draw the molecular structure in its current configuration
	//leave empty if drawing cannot be done
    }
    
}
