#ifndef Antipatrea_SetupChain2D_HPP_
#define Antipatrea_SetupChain2D_HPP_

#include "Plugin2D/Chain2D.hpp"
#include "Plugin2D/Setup2D.hpp"
#include "Plugin2D/CfgAcceptorChain2D.hpp"
#include "Plugin2D/CfgProjectorChain2D.hpp"

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Setup the planning problem for a robot kinematic chain 
     *       operating in a 2D scene as well as the planner and its components.
     *
     *@remarks
     * - Setup creates instances of the planning components based on
     *   keywords provided by the user or default settings
     *   when the user does not provide any keywords.
     * - Only one instance per class is created.
     * - After creating the components, Setup sets the pointers
     *   so that components/planners that depend on, for example, CfgDistance
     *   will have the CfgDistance pointer set to the CfgDistance
     *   instance created in the first step.
     * - After setting the pointers, Setup iterates over all the instances
     *   that were created in step 1, and calls the function SetupFromParams
     *   in order to set the parameter values for each instance.
     */ 
    class SetupChain2D : public Setup2D
    {
    public:    
        SetupChain2D(void) : Setup2D()
        {
        }
                
        virtual ~SetupChain2D(void)
        {
        }

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Get a pointer to the kinematic chain.
           */
        virtual const Chain2D* GetChain2D(void) const
        {
            return dynamic_cast<const Chain2D*>(GetCfgForwardKinematics());
        }

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Get a pointer to the kinematic chain.
           */
        virtual Chain2D* GetChain2D(void)
        {
            return dynamic_cast<Chain2D*>(GetCfgForwardKinematics());
        }
        
        virtual void Prepare(Params & params);

    protected:
        virtual void NewCfgForwardKinematics(Params & params)
        {
            SetCfgForwardKinematics(new Chain2D());
            OnNewInstance(GetCfgForwardKinematics());
        }
        
        virtual void NewCfgAcceptor(Params & params)
        {
            SetCfgAcceptor(new CfgAcceptorChain2D());
            OnNewInstance(GetCfgAcceptor());
        }
        
        virtual void NewCfgProjector(Params & params)
        {
            SetCfgProjector(new CfgProjectorChain2D());
            OnNewInstance(GetCfgProjector());
        }
        
        virtual void SetupPointersCfgAcceptor(void)
        {
            Setup::SetupPointersCfgAcceptor();
            
            if(dynamic_cast<CfgAcceptorChain2D*>(GetCfgAcceptor()))
            {
                dynamic_cast<CfgAcceptorChain2D*>(GetCfgAcceptor())->SetScene2D(GetScene2D());
                dynamic_cast<CfgAcceptorChain2D*>(GetCfgAcceptor())->SetChain2D(GetChain2D());
            }
        }
        
        virtual void SetupPointersCfgProjector(void)
        {
            Setup::SetupPointersCfgProjector();
            
            if(dynamic_cast<CfgProjectorChain2D*>(GetCfgProjector()))
                  dynamic_cast<CfgProjectorChain2D*>(GetCfgProjector())->SetChain2D(GetChain2D());
        }
    };
    
}

#endif
