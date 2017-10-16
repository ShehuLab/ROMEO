#ifndef Antipatrea__CfgProjectorPoint2D_HPP_
#define Antipatrea__CfgProjectorPoint2D_HPP_

#include "Components/CfgProjectors/CfgProjector.hpp"

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief CfgProjectorPoint2D computes the projection
     *       as the position of the point robot.
     */
    class CfgProjectorPoint2D : public CfgProjector
    {
    public:
	CfgProjectorPoint2D(void) : CfgProjector(2)
	{
	}
	
	virtual ~CfgProjectorPoint2D(void)
	{
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Compute the configuration projection as the position of the point robot.
	 *
	 *@remarks
	 * - The function assumes that <tt>proj</tt> has been properly allocated
	 *   (using CfgProjector::NewValues())
	 */
	virtual void Project(const Cfg & cfg, double proj[])
	{
	    proj[0] = cfg.GetValues()[0];
	    proj[1] = cfg.GetValues()[1];
	}
	
	
    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to CfgProjectorPoint2D.
     */
    ClassContainer(CfgProjectorPoint2D, m_cfgProjectorPoint2D);
    
}

#endif
