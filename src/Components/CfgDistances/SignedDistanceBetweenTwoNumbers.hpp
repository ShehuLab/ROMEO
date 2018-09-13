#ifndef Antipatrea__SignedDistanceBetweenTwoNumbers_HPP_
#define Antipatrea__SignedDistanceBetweenTwoNumbers_HPP_

#include "Components/CfgDistances/SignedDistanceBetweenTwoValues.hpp"
#include "Utils/Algebra2D.hpp"

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Interprets the values as numbers and defines the signed distance as their difference.
     */
    class SignedDistanceBetweenTwoNumbers : public SignedDistanceBetweenTwoValues
    {
    public:
        SignedDistanceBetweenTwoNumbers(void) : SignedDistanceBetweenTwoValues()
        {
        }
        
        virtual ~SignedDistanceBetweenTwoNumbers(void)
        {
        }
        
        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Computes the signed distance between two numbers as their difference
         */
         virtual double SignedDistance(const double from,
                                      const double to)
        {
            return to - from;
        }
    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to SignedDistanceBetweenTwoNumbers.
     */        
    ClassContainer(SignedDistanceBetweenTwoNumbers, m_signedDistanceNumber);
    
}

#endif
