/*
 * Feltr4D.hpp
 *
 *  Created on: Sep 14, 2017
 *      Author: kmolloy
 */


#ifndef Antipatrea_Feltr4D_HPP_
#define Antipatrea_Feltr4D_HPP_

#include "Components/CfgForwardKinematics/CfgForwardKinematics.hpp"
#include "Utils/Misc.hpp"
#include <vector>

namespace Antipatrea
{
    /**
     *@author Kevin Molloy, Erion Plaku, Amarda Shehu
     *@brief Project a protein into the 4D Feltr low-dimensional embedding
     *
     */
    class Feltr4D : public CfgForwardKinematics
    {
    public:
	Feltr4D(void);

	virtual ~Feltr4D(void);


	virtual void Info(const char prefix[]) const;

	/**
	 *@author Kevin Molloy, Erion Plaku, Amarda Shehu
	 *@brief Set the parameter values of the component from the given parameters.
	 *
	 *@remarks
	 * - Sets the number of links and the link length (keywords Constants::KW_Chain2D_NrLinks, Constants::KW_Chain2D_LinkLength).
	 * - It uses the parameter group associated with the keyword Constants::KW_Chain2D.
	 * - The parameter values can be specified in a text file as, for example,
	 *     <center><tt>Chain2D { NrLinks 8 LinkLength 1.0 }</tt></center>
	 */
	virtual void SetupFromParams(Params & params);

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get the number of joints in the chain.
	 */
	virtual int GetNrJoints(void) const
	{
	    return m_joints.size();
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get the number of links in the chain.
	 */
	virtual int GetNrLinks(void) const
	{
	    return GetNrJoints();
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get the x-coordinate of the start position of the <tt>i</tt>th link.
	 *
	 *@remarks
	 * - Function does not check if <tt>i</tt> is in the
	 *   range <tt>0</tt> to <tt>GetNrLinks() - 1</tt>.
	 */
	virtual double GetLinkStartX(const int i) const
	{
	    return m_positions[2 * i];
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get the y-coordinate of the start position of the <tt>i</tt>th link.
	 *
	 *@remarks
	 * - Function does not check if <tt>i</tt> is in the
	 *   range <tt>0</tt> to <tt>GetNrLinks() - 1</tt>.
	 */
	virtual double GetLinkStartY(const int i) const
	{
	    return m_positions[2 * i + 1];
	}


	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get the x-coordinate of the end position of the <tt>i</tt>th link.
	 *
	 *@remarks
	 * - Function does not check if <tt>i</tt> is in the
	 *   range <tt>0</tt> to <tt>GetNrLinks() - 1</tt>.
	 */
	virtual double GetLinkEndX(const int i) const
	{
	    return GetLinkStartX(i + 1);
	}


	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get the y-coordinate of the end position of the <tt>i</tt>th link.
	 *
	 *@remarks
	 * - Function does not check if <tt>i</tt> is in the
	 *   range <tt>0</tt> to <tt>GetNrLinks() - 1</tt>.
	 */
	virtual double GetLinkEndY(const int i) const
	{
	    return GetLinkStartY(i + 1);
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get the angle (in radians) associated with the <tt>i</tt>th link.
	 *
	 *@remarks
	 * - Function does not check if <tt>i</tt> is in the
	 *   range <tt>0</tt> to <tt>GetNrLinks() - 1</tt>.
	 */
	virtual double GetLinkTheta(const int i) const
	{
	    return m_joints[i];
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get the length of the <tt>i</tt>th link.
	 *
	 *@remarks
	 * - Function does not check if <tt>i</tt> is in the
	 *   range <tt>0</tt> to <tt>GetNrLinks() - 1</tt>.
	 */
	virtual double GetLinkLength(const int i) const
	{
	    return m_lengths[i];
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Remove all links from the chain.
	 */
	virtual void RemoveAllLinks(void);

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Add one link with length <tt>length</tt>.
	 */
	virtual void AddLink(const double length);

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Add <tt>n</tt> links, each having length <tt>length</tt>.
	 */
	virtual void AddLinks(const int n, const double length);

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Draw the chain in the current configuration.
	 */
	virtual void Draw(void);

    protected:
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Implement the forward kinematics.
	 *
	 *@remarks
	 * - This function always computes the forward kinematics, ignoring flag values.
	 */
	virtual void DoFK(void);

	virtual void MatrixMultMatrix(const double M1[6], const double M2[6], double M[6]);


	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Lengths for each link in the chain.
	 */
	std::vector<double> m_lengths;

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Positions for each link in the chain.
	 *
	 *@remarks
	 *  - The start position of the first link is <tt>(0, 0)</tt>.
	 */

	std::vector<double> m_positions;
    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to Chain2D.
     */
    ClassContainer(Chain2D, m_chain2D);

}

#endif



#endif /* Antipatrea_Feltr4D_HPP_ */
