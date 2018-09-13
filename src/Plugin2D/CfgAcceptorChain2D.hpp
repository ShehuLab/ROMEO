#ifndef Antipatrea__CfgAcceptorChain2D_HPP_
#define Antipatrea__CfgAcceptorChain2D_HPP_

#include "Components/CfgAcceptors/CfgAcceptor.hpp"
#include "Plugin2D/Chain2D.hpp"
#include "Plugin2D/Scene2D.hpp"

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief CfgAcceptorChain2D accepts a configuration iff the 
     *       chain avoids self collisions and collisions with the 
     *       obstacles in the scene.
     *
     *@remarks
     *  - The chain is considered to be in self-collision iff
     *    nonconsecutive links of the chain intersect with each other.
     *  - CfgAcceptorChain2D needs access to Scene2D and Chain2D.
     */
    class CfgAcceptorChain2D : public CfgAcceptor,
                               public Scene2DContainer,
                               public Chain2DContainer
    {
    public:
        CfgAcceptorChain2D(void) : CfgAcceptor(),
                                   Scene2DContainer(),
                                   Chain2DContainer()
        {
        }
        
        virtual ~CfgAcceptorChain2D(void)
        {
        }
        
        virtual bool CheckSetup(void)
        {
            return
                CfgAcceptor::CheckSetup() &&
                GetScene2D() != NULL &&
                GetChain2D() != NULL &&
                GetScene2D()->CheckSetup() &&
                GetChain2D()->CheckSetup();
        }

        virtual void Info(const char prefix[]) const
        {
            CfgAcceptor::Info(prefix);
            Logger::m_out << prefix << " Scene2D = " << Name(GetScene2D()) << std::endl
                          << prefix << " Chain2D = " << Name(GetChain2D()) << std::endl;
        }
        

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Return true iff the chain avoids self collisions and
         *       collisions with the obstacles in the scene.
         */        
        virtual bool IsAcceptable(Cfg & cfg);
        
    };
    
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to CfgAcceptorChain2D.
     */
  ClassContainer(CfgAcceptorChain2D, m_cfgAcceptorChain2D);
    
}

#endif
