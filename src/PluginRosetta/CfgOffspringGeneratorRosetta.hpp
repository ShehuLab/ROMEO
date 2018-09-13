#ifndef Antipatrea__CfgOffspringGeneratorRosetta_HPP_
#define Antipatrea__CfgOffspringGeneratorRosetta_HPP_

#include "Components/CfgOffspringGenerators/CfgOffspringGenerator.hpp"
#include "PluginRosetta/MolecularStructureRosetta.hpp"
#include "Components/CfgDistances/CfgDistance.hpp"
#include "Components/CfgAcceptors/CfgAcceptorBasedOnDistance.hpp"
#include "PluginRosetta/CfgDistanceAtomRMSD.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>

#include "Utils/Selector.hpp"

namespace Antipatrea
{
    /**
      *@author Kevin Molloy, Erion Plaku, Amarda Shehu
      *@brief Store a single molecular fragment.  A Fragment is a
      *       contiguous piece exised from an imaged protein (NMR or
      *       X-ray usually) where the represented consists of the
      *       PHI, PSI, and OMEGA backbone dihedral angles (an ideal
      *       geometry is assumed).  An example use of a fragment would
      *       be to randomly perturb a known good (energetically feasible)
      *       protein by selecting a fragment from a library/db and
      *       substituting in the backbone angles and adjusting the
      *       protein (forward kinematics).
      *
      */
    class Fragment
    {
    public:
        Fragment(std::string pdbID,
                 unsigned int pdbAAOffset,
                 std::vector<double> phiAngles,
                 std::vector<double> psiAngles,
                 std::vector<double> omegaAngles)
        {
                m_pdbID = pdbID;
                m_pdbAAOffset = pdbAAOffset;
                m_phiAngles = phiAngles;
                m_psiAngles = psiAngles;
                m_omegaAngles = omegaAngles;
        }

        /**
         *@author Kevin Molloy, Erion Plaku, Amarda Shehu
         *@brief Return the entire set of fragment angles
         *
         */

        void GetAngles(std::vector<double> &phiAngles,
                           std::vector<double> &psiAngles,
                           std::vector<double> &omegaAngles);
        /**
         *@author Kevin Molloy, Erion Plaku, Amarda Shehu
         *@brief Return a specific PHI angle
         *
         */
        virtual double GetPhi(unsigned int offset)
        {
            return(m_phiAngles[offset]);
        }

        /**
         *@author Kevin Molloy, Erion Plaku, Amarda Shehu
         *@brief Return a specific PSI angle
         *
         */
        virtual double GetPsi(unsigned int offset)
        {
            return(m_psiAngles[offset]);
        }

        /**
         *@author Kevin Molloy, Erion Plaku, Amarda Shehu
         *@brief Return a specific OMEGA angle
         *
         */
        virtual double GetOmega(unsigned int offset)
        {
            return(m_omegaAngles[offset]);
        }

        /**
         *@author Kevin Molloy, Erion Plaku, Amarda Shehu
         *@brief Return a the number of positions in this fragment
         *
         */
        unsigned int GetLength()
        {
            return m_phiAngles.size();
        };

        void PrintFragment()
        {
            std::cout << "Printing fragment:";
            for (unsigned int i(0);i<m_phiAngles.size();++i)
            {
                std::cout << "phi" << i << ":" << m_phiAngles[i] << ":"
                          << "psi" << i << ":" << m_psiAngles[i] << ":"
                          << "omg" << i << ":" << m_omegaAngles[i] << ":";
            }
            std::cout << "\n";
        }

    protected:
        /**
         *@author Kevin Molloy, Erion Plaku, Amarda Shehu
         *@brief PDB from which the fragment was extracted.
         */
        std::string m_pdbID;

        /**
         *@author Kevin Molloy, Erion Plaku, Amarda Shehu
         *@brief Offset within the PDB that the fragment was extracted
         */
        unsigned int m_pdbAAOffset;

        /**
         *@author Kevin Molloy, Erion Plaku, Amarda Shehu
         *@brief PHI backbone dihedral angles for the fragment
         */

        std::vector<double> m_phiAngles;

        /**
         *@author Kevin Molloy, Erion Plaku, Amarda Shehu
         *@brief PSI backbone dihedral angles for the fragment
         */
        std::vector<double> m_psiAngles;

        /**
         *@author Kevin Molloy, Erion Plaku, Amarda Shehu
         *@brief OMEGA backbone dihedral angles for the fragment
         */
        std::vector<double> m_omegaAngles;

   };

   /**
     *@author Kevin Molloy, Erion Plaku, Amarda Shehu
     *@brief Store a set of Rosetta fragments (all of the same
     *       length and for the same position within the protein).
     */

    class RosettaFragment: public Fragment
    {
    public:
        RosettaFragment(std::string pdbID,
                        unsigned int pdbAAOffset,
                        std::vector<double> phiAngles,
                        std::vector<double> psiAngles,
                        std::vector<double> omegaAngles)
                :Fragment(pdbID,pdbAAOffset,phiAngles,psiAngles,omegaAngles)
        {

        }
    };

   /**
    *@author Kevin Molloy, Erion Plaku, Amarda Shehu
    *@brief DB of Rosetta fragments for a set of a protein
    */

    class FragmentDB
    {
    public:
        /**
          *@author Kevin Molloy, Erion Plaku, Amarda Shehu
          *@brief Insert a new fragment
        */
        void Insert(unsigned int pos,
                    Fragment &f)
        {
            if (m_fragments.size() >= pos)
                m_fragments.resize(pos+1);

            m_fragments[pos].push_back(f);

            /*
             * std::cout << "Insert fragment for pos:" << pos <<":fragment:"
                      << m_fragments[pos].size() << ":";
               f.PrintFragment();
             */

        }

        /**
          *@author Kevin Molloy, Erion Plaku, Amarda Shehu
          *@brief Number of positions  (protein positons) in the DB
        */
        unsigned int NumberOfPositions() const
        {
            return (m_fragments.size());
        }

        /**
          *@author Kevin Molloy, Erion Plaku, Amarda Shehu
          *@brief Number of fragments for a given position
        */
        unsigned int NumberOfSamples(unsigned int pos)
        {
            if (pos < m_fragments.size())
                return (m_fragments[pos].size());
        }
        /**
          *@author Kevin Molloy, Erion Plaku, Amarda Shehu
          *@brief Return the PHI angle for a specific position, fragment,
          *       and offset within the fragment
        */

        double GetFragmentPhi(unsigned int pos,
                              unsigned int fragmentNumber,
                              unsigned int fragmentOffset)
        {
            return (m_fragments[pos][fragmentNumber].GetPhi(fragmentOffset));
        }

        double GetFragmentPsi(unsigned int pos,
                              unsigned int fragmentNumber,
                              unsigned int fragmentOffset)
        {
            return (m_fragments[pos][fragmentNumber].GetPsi(fragmentOffset));
        }

        double GetFragmentOmega(unsigned int pos,
                                unsigned int fragmentNumber,
                                unsigned int fragmentOffset)
        {
            return (m_fragments[pos][fragmentNumber].GetOmega(fragmentOffset));
        }

    protected:
        /**
          *@author Kevin Molloy, Erion Plaku, Amarda Shehu
          *@brief Number of positions/amino acids within a single fragment
        */
        unsigned int m_fragmentSize;

        // first dim is (amino acid) position within the protein (0..n-1)
        // second dimension is an array of fragments for that position
        std::vector<std::vector <Fragment> >  m_fragments;
   };



    /**
     *@author Kevin Molloy, Erion Plaku, Amarda Shehu
     *@brief Use Rosetta/library to generate an offspring near the parent configuration.
     *
     *
     */
    class CfgOffspringGeneratorRosetta : public CfgOffspringGenerator,
                                         public MolecularStructureRosettaContainer,
                                         public CfgDistanceContainer
    {
    protected:
        /**
          *@author Kevin Molloy, Erion Plaku, Amarda Shehu
          *@brief A map of fragment databases organized by the fragment length.
          *
        */
        std::unordered_map<unsigned int,FragmentDB> m_fragmentMap;

        /**
          *@author Kevin Molloy, Erion Plaku, Amarda Shehu
          *@brief Hold a vector of lengths
        */

        std::vector<double> m_fragmentProbs;

        std::vector<std::string> m_fragmentFileNames;

         /**
          *@author Kevin Molloy, Erion Plaku, Amarda Shehu
          *@brief Read in a Rosetta fragment file.  These files
          *       can be generated from the website
          *       http://robetta.bakerlab.org.  The files are organized by
          *       having the position and neighbors(fragments) header, followed
          *       by the number of neighbors/fragments.
          *
          */
        unsigned int LoadFragmentLibrary(std::string fragmentFile,
         		                         double fragmentProbability);

        /**
           *@author Kevin Molloy, Erion Plaku, Amarda Shehu
           *@brief  Number of offspring to evaluate per call. The offspring
                    that is the closest to the passed to GenerateOffspringCfg
                    will be accepted.
           */
        int m_offspringToGenerate;

    public:
        CfgOffspringGeneratorRosetta(void) : CfgOffspringGenerator(),
                                             MolecularStructureRosettaContainer(),
                                             CfgDistanceContainer()
        {
            m_offspringToGenerate=1;
        }

        virtual ~CfgOffspringGeneratorRosetta(void)
        {
        }

        /**
         *@author Kevin Molloy Erion Plaku, Amarda Shehu
         *@brief Get the pointer to the target configuration.
         */
        virtual const Cfg* GetTargetCfg(void) const
        {
            return m_cfgTarget;
        }

        /**
         * @author Kevin Molloy, Erion Plaku, Amarda Shehu
         * @brief select a fragment length to use based on weights
         */
        Selector<int> SelectFragmentLength()
		{

		}
        /**
         *@author Kevin Molloy Erion Plaku, Amarda Shehu
         *@brief Set the pointer to the target configuration.
         */
        virtual void SetTargetCfg(const Cfg * const cfgTarget)
        {
            m_cfgTarget = cfgTarget;
        }

        virtual void PrintSummary() const
        {
            for (std::unordered_map<unsigned int,FragmentDB>::const_iterator
                 it = m_fragmentMap.cbegin(); it !=m_fragmentMap.cend(); ++it)
            {
                std::cout << it->first << " has data for "
                          << it->second.NumberOfPositions() << std::endl;
            }
        }

        virtual bool CheckSetup(void) const
        {
            return
                CfgOffspringGenerator::CheckSetup() &&
                GetMolecularStructureRosetta() != NULL &&
                GetMolecularStructureRosetta()->CheckSetup();
        }

        virtual void Info(const char prefix[]) const
        {
            CfgOffspringGenerator::Info(prefix);
            Logger::m_out << prefix << " MolecularStructureRosetta = " << Name(GetMolecularStructureRosetta())
                          << " m_offspringToGenerate = " << m_offspringToGenerate << std::endl;
        }


        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Set the parameter values of the component from the given parameters.
         */
        virtual void SetupFromParams(Params & params);

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Use Rosetta to generate the offspring configuration near the parent configuration.
         *
         *@remarks
         * - Use GetStep() as a measure of how far/different the offspring should be from its parent.
         */
        virtual void GenerateOffspringCfg(Cfg & cfg);

    protected:
        double GenerateAnOffspringCfg(Cfg & cfg);

        /**
          *@author Kevin Molloy, Erion Plaku, Amarda Shehu
          *@brief Pointer to the target configuration.
          */
         const Cfg *m_cfgTarget;

         Selector<int> m_selector;

    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to CfgOffspringGeneratorRosetta.
     */
    ClassContainer(CfgOffspringGeneratorRosetta, m_cfgOffspringGeneratorRosetta);
}

#endif
