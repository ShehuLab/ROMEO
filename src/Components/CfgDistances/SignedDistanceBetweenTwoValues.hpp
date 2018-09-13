#ifndef Antipatrea__SignedDistanceBetweenTwoValues_HPP_
#define Antipatrea__SignedDistanceBetweenTwoValues_HPP_

#include "Components/Component.hpp"

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Interface to define the signed distance between two values.
     */
    class SignedDistanceBetweenTwoValues : public Component
    {
    public:
        SignedDistanceBetweenTwoValues(void) : Component()
        {
        }
        
        virtual ~SignedDistanceBetweenTwoValues(void)
        {
        }
        
        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Computes the distance between two values.
         */
        virtual double SignedDistance(const double from,
                                      const double to) = 0;
    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to SignedDistanceBetweenTwoValues.
     */        
    ClassContainer(SignedDistanceBetweenTwoValues, m_signedDistanceValue);
    
}

#endif
