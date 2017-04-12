#ifndef Antipatrea_MolecularStructureRosetta_HPP_
#define Antipatrea_MolecularStructureRosetta_HPP_

#include "Components/CfgForwardKinematics/CfgForwardKinematics.hpp"
#include "Utils/Misc.hpp"
#include <vector>

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Forward kinematics for a molecular structure.
     *
     *
     * Gets joints, computes FK
     *
     *@remarks
     *  - The start position of the first link is <tt>(0, 0)</tt>.
     */
    class MolecularStructureRosetta : public CfgForwardKinematics
    {
    public:    
	MolecularStructureRosetta(void);
	
	virtual ~MolecularStructureRosetta(void);

	virtual bool CheckSetup(void) const;
	
	virtual void Info(const char prefix[]) const;

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set the parameter values of the component from the given parameters.
	 */
	virtual void SetupFromParams(Params & params);
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Draw the molecular structure in the current configuration.
	 */
	virtual void Draw(void);
		
    protected:
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Implement the forward kinematics.
	 *
	 *@remarks
	 * - This function always computes the forward kinematics, ignoring flag values.
	 * - Use the values in m_joints as the joint values.
	 */
	virtual void DoFK(void);

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Pointer to Rosetta pose.
	 *
	 *@remarks
	 * - Make sure to set it up correctly before doing any forward kinematics
	 */
	//Rosetta::Pose *m_pose;
	
    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to MolecularStructureRosetta.
     */	 
    ClassContainer(MolecularStructureRosetta, m_molecularStructureRosetta);
    
}

#endif
