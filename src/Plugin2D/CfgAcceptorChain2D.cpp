#include "Plugin2D/CfgAcceptorChain2D.hpp"
#include "Utils/Geometry.hpp"

namespace Antipatrea
{
    bool CfgAcceptorChain2D::IsAcceptable(Cfg & cfg)
    {
        Chain2D   *chain = GetChain2D();
        Scene2D   *scene = GetScene2D();
        const int  n     = chain->GetNrLinks();
        double     p1[2];
        double     p2[2];
        
        
        chain->CopyJointValues(cfg.GetValues());
        chain->FK();

        //self-collision
        for(int i = 0; i < n; ++i)
            for(int j = i + 2; j < n; ++j)
                if(IntersectSegments2D(chain->GetLinkStartX(i), chain->GetLinkStartY(i),
                                       chain->GetLinkEndX(i), chain->GetLinkEndY(i),
                                       chain->GetLinkStartX(j), chain->GetLinkStartY(j),
                                       chain->GetLinkEndX(j), chain->GetLinkEndY(j)))
                    return false;
        
                                        
                
        //collision with the scene obstacles
        for(int i = 0; i < n; ++i)
        {
            p1[0] = chain->GetLinkStartX(i);
            p1[1] = chain->GetLinkStartY(i);
            
            p2[0] = chain->GetLinkEndX(i);
            p2[1] = chain->GetLinkEndY(i);

            if(scene->IsSegmentInCollision(p1, p2))
                return false;
        }

        
        
        return true;
        
    }
    
}
