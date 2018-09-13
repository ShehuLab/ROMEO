#ifndef Antipatrea__CfgAcceptorPoint2D_HPP_
#define Antipatrea__CfgAcceptorPoint2D_HPP_

#include "Components/CfgAcceptors/CfgAcceptor.hpp"
#include "Plugin2D/Scene2D.hpp"

namespace Antipatrea
{        
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief CfgAcceptorPoints2D accepts a configuration iff the 
     *       point robot avoids collisions with the 
     *       obstacles in the scene.
     *
     *@remarks
     *  - CfgAcceptorPoint2D needs access to Scene2D.
     */
    class CfgAcceptorPoint2D : public CfgAcceptor,
                               public Scene2DContainer
    {
    public:
        CfgAcceptorPoint2D(void) : CfgAcceptor(),
                                   Scene2DContainer()
        {
        }
        
        virtual ~CfgAcceptorPoint2D(void)
        {
        }
        
        virtual bool CheckSetup(void)
        {
            return
                CfgAcceptor::CheckSetup() &&
                GetScene2D() != NULL &&
                GetScene2D()->CheckSetup();
        }

        virtual void Info(const char prefix[]) const
        {
            CfgAcceptor::Info(prefix);
            Logger::m_out << prefix << " Scene2D = " << Name(GetScene2D()) << std::endl;
        }
                
        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Return true iff the point robot avoids
         *       collisions with the obstacles in the scene.
         */        
        virtual bool IsAcceptable(Cfg & cfg)
        {
            return GetScene2D()->IsPointInCollision(cfg.GetValues()) == false;
        }
    };

       /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to CfgAcceptorPoint2D.
     */
    ClassContainer(CfgAcceptorPoint2D, m_cfgAcceptorPoint2D);
    
}

#endif
