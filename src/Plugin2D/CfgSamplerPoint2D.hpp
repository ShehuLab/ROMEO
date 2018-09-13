#ifndef Antipatrea__CfgSamplerPoint2D_HPP_
#define Antipatrea__CfgSamplerPoint2D_HPP_

#include "Components/CfgSamplers/CfgSampler.hpp"
#include "Plugin2D/Scene2D.hpp"
#include "Utils/PseudoRandom.hpp"

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Sample a point uniformly at random inside the bounding box of the scene.
     *
     *@remarks
     *  - CfgSamplerPoint2D needs access to Scene2D.
     */
    class CfgSamplerPoint2D : public CfgSampler,
                              public Scene2DContainer
    {
    public:
        CfgSamplerPoint2D(void) : CfgSampler(),
                                  Scene2DContainer()
        {
        }
        
        virtual ~CfgSamplerPoint2D(void)
        {
        }
        
        virtual bool CheckSetup(void)
        {
            return
                CfgSampler::CheckSetup() &&
                GetScene2D() != NULL &&
                GetScene2D()->CheckSetup();
        }

        
        virtual void Info(const char prefix[]) const
        {
            CfgSampler::Info(prefix);
            Logger::m_out << prefix << " Scene2D = " << Name(GetScene2D()) << std::endl;
        }

        
        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Sample a point uniformly at random inside the bounding box of the scene.
         */
        virtual bool Sample(Cfg & cfg)
        {
            double *vals = cfg.GetValues();
            vals[0] = RandomUniformReal(GetScene2D()->GetBoundingBox()[0], GetScene2D()->GetBoundingBox()[2]);
            vals[1] = RandomUniformReal(GetScene2D()->GetBoundingBox()[1], GetScene2D()->GetBoundingBox()[3]);
            cfg.SetValues(vals);

            return true;
            
        }
    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to CfgSamplerPoint2D.
     */
    ClassContainer(CfgSamplerPoint2D, m_cfgSamplerPoint2D);
    
}

#endif
