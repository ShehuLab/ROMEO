#include "PluginRosetta/MolecularStructureRosetta.hpp"
#include "Setup/Defaults.hpp"
#include "Utils/GDraw.hpp"
#include "Utils/Algebra2D.hpp"
#include <cmath>
#include <iostream>

#include <core/scoring/ScoreFunctionFactory.hh>
#include <core/pose/util.hh>

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

    void MolecularStructureRosetta::LoadPDBFile(const char fileName[])
    {
    	std::cout << "Loading pose" << std::endl;
    	core::import_pose::centroid_pose_from_pdb(m_pose,fileName);
    	std::cout << "Pose loading completed." << std::endl;
    	auto dofs = m_pose.total_residue() * 3;
    	std::cout << "dofs is:" << dofs << std::endl;
    }

    void MolecularStructureRosetta::CreateEnergyFunction()
    {
    	m_scoreFn = core::scoring::ScoreFunctionFactory::create_score_function("score3.wts");
    }

    void MolecularStructureRosetta::LoadPose(const Cfg &cfg)
    {
        const double *vals = cfg.GetValues();

        for (unsigned int i=0;i < GetNrResidues();++i)
	{
	    m_pose.set_phi(i+1,Algebra2D::RadiansToDegrees(vals[i*3 + 0]));
            m_pose.set_psi(i+1,Algebra2D::RadiansToDegrees(vals[i*3 + 1]));
	    m_pose.set_omega(i+1,Algebra2D::RadiansToDegrees(vals[i*3 + 2 ]));
	}
    }

    double MolecularStructureRosetta::EvaluateEnergy(Cfg &cfg)
    {
    	// load Pose with angles
    	LoadPose(cfg);

    	//set SS in pose
	core::pose::set_ss_from_phipsi(m_pose);
	double energy = m_scoreFn->score(m_pose);

	return(energy);
    }



    void MolecularStructureRosetta::SetCfgDOFs(Cfg &cfgToSet)
    {
        double *vals = cfgToSet.GetValues();

        for (unsigned int i=0;i < GetNrResidues();++i) {
	    vals[(i*3) + 0 ] = Algebra2D::DegreesToRadians(m_pose.phi(i+1));
	    vals[(i*3) + 1 ] = Algebra2D::DegreesToRadians(m_pose.psi(i+1));
	    vals[(i*3) + 2 ] = Algebra2D::DegreesToRadians(m_pose.omega(i+1));

	}
	cfgToSet.SetValues(vals);
    }

    std::vector<point>  MolecularStructureRosetta::GetAtomPositions(const Cfg &cfgToSet)
    {
	LoadPose(cfgToSet);

	std::vector<point> crds;
	std::vector<std::string> atomTypes = {"N","CA","C"};
	crds.reserve(m_pose.total_residue() * atomTypes.size());
	for (uint i=1; i<=m_pose.total_residue(); i++)
	{
	    const core::conformation::Residue & r = m_pose.residue(i);
	    for (auto j=0;j < atomTypes.size();++j)
	    {
	        auto aindex = r.atom_index(atomTypes[j]);
	        const  core::conformation::Atom &a = r.atom(aindex);
	        crds.push_back(point(a.xyz().x(),a.xyz().y(), a.xyz().z()));
	    }
	}
	return crds;
    }

}
