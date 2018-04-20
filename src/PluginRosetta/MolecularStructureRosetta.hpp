#ifndef Antipatrea_MolecularStructureRosetta_HPP_
#define Antipatrea_MolecularStructureRosetta_HPP_

#include "Components/CfgForwardKinematics/CfgForwardKinematics.hpp"
#include "Utils/Misc.hpp"
#include <vector>

#include <core/pose/Pose.hh>
#include <core/import_pose/import_pose.hh>
#include <core/scoring/ScoreFunction.hh>

#include "Utils/point.hpp"

namespace Antipatrea
{


    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Forward kinematics for a molecular structure.
     *
     *
     * Gets joints, computes FK
     *
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
	 *@brief Drawing a molecular configuration is not implemented.
	 */
	virtual void Draw(void);

	/**
	 *@author Kevin Molloy, Erion Plaku, Amarda Shehu
	 *@brief Load the Rosetta pose from a PDB file
  	 */
	virtual void LoadPDBFile(const char fileName[]);

	/**
	 *@author Kevin Molloy, Erion Plaku, Amarda Shehu
	 *@brief Set a cfg to the extended conformation
  	 */
	void SetExtented(Cfg &cfg);

	/**
	 *@author Kevin Molloy, Erion Plaku, Amarda Shehu
	 *@brief Return the number of residues/amino acids
	 *       within the loaded protein
  	 */
	virtual int GetNrResidues(void) const
	{
	    return m_pose.total_residue();
	}

	/**
          *@author Kevin Molloy, Erion Plaku, Amarda Shehu
	  *@brief  Export phi, psi, omega angles from current pose
	  *
	  */
	virtual void SetCfgDOFs(Cfg &cfgToSet);

	/**
          *@author Kevin Molloy, Erion Plaku, Amarda Shehu
	  *@brief  Create a Rosetta scoring/energy function
	  *
	  */
	virtual void CreateEnergyFunction();

	/**
          *@author Kevin Molloy, Erion Plaku, Amarda Shehu
	  *@brief  Export the configuration into the pose object
	  *        and evaluate its energy
	  */
	virtual double EvaluateEnergy(Cfg &cfg);

	/**
          *@author Kevin Molloy, Erion Plaku, Amarda Shehu
	  *@brief  Export the configuration into the pose object
	  */

	virtual void LoadPose(const Cfg &cfg);

	/**
	 *@author Kevin Molloy, Erion Plaku, Amarda Shehu
	 *@brief Return the set of 3-d coordinates corresponding to the
	 *       heavy backbone atoms (N-CA-C-O) of the protein.
  	 */
	virtual std::vector<point>  GetAtomPositions(const Cfg &cfg);

    protected:
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Implement the forward kinematics.
	 *
	 *@remarks
	 * - This function always computes the forward kinematics, ignoring flag values.
	 * - Since FK are implmeneted by Rosetta as a byproduct of setting the
	 * - the proteins dihedral angles, this function does not have to do any work.
	 */
	virtual void DoFK(void);


	/**
	 *@author Erion Plaku, Amarda Shehu, Kevin Molloy
	 *@brief Rosetta pose.
	 *
	 *@remarks
	 * - Make sure to load it within the desired cfg before
	 *   evaluating any forward kinematics
	 */
	core::pose::Pose m_pose;
	
	/**
          *@author Erion Plaku, Amarda Shehu, Kevin Molloy
	  *@brief Rosetta Scoring (energy) function.
	  *
	  */
	core::scoring::ScoreFunctionOP m_scoreFn;
    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to MolecularStructureRosetta.
     */	 
    ClassContainer(MolecularStructureRosetta, m_molecularStructureRosetta);
    
}

#endif
