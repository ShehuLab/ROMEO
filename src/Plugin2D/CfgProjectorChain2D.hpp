#ifndef Antipatrea__CfgProjectorChain2D_HPP_
#define Antipatrea__CfgProjectorChain2D_HPP_

#include "Components/CfgProjectors/CfgProjector.hpp"
#include "Plugin2D/Chain2D.hpp"

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief CfgProjectorChain2D computes the projection
     *       as the position of the end effector of the chain.
     *
     *@remarks
     *  - CfgAcceptorChain2D needs access to Chain2D.
     */
    class CfgProjectorChain2D : public CfgProjector,
				public Chain2DContainer
    {
    public:
	CfgProjectorChain2D(void) : CfgProjector(2),
				    Chain2DContainer()
	{
	}
	
	virtual ~CfgProjectorChain2D(void)
	{
	}
	
	virtual bool CheckSetup(void)
	{
	    return
		CfgProjector::CheckSetup() &&
		GetChain2D() != NULL &&
		GetChain2D()->CheckSetup();
	}

	
	virtual void Info(const char prefix[]) const
	{
	    CfgProjector::Info(prefix);
	    Logger::m_out << prefix << " Chain2D = " << Name(GetChain2D()) << std::endl;
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Compute the configuration projection as the position of the end effector.
	 *
	 *@remarks
	 * - The function assumes that <tt>proj</tt> has been properly allocated
	 *   (using CfgProjector::NewValues())
	 */
	virtual void Project(Cfg & cfg, double proj[]);
	
    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to CfgProjectorChain2D.
     */
    ClassContainer(CfgProjectorChain2D, m_cfgProjectorChain2D);
    
}

#endif
