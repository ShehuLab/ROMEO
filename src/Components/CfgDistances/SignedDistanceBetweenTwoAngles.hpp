#ifndef Antipatrea__SignedDistanceBetweenTwoAngles_HPP_
#define Antipatrea__SignedDistanceBetweenTwoAngles_HPP_

#include "Components/CfgDistances/SignedDistanceBetweenTwoValues.hpp"
#include "Utils/Algebra2D.hpp"

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Interprets the values as angles and defines the signed distance as the signed length of the shortest arc.
     */
    class SignedDistanceBetweenTwoAngles : public SignedDistanceBetweenTwoValues
    {
    public:
        SignedDistanceBetweenTwoAngles(void) : SignedDistanceBetweenTwoValues()
        {
        }
        
        virtual ~SignedDistanceBetweenTwoAngles(void)
        {
        }
        
        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Computes the signed distance between two angles as the signed length of the shortest arc.
         *
         *@remarks
         *  - Angles are assumed to be in radians.
         *  - Distance is positive only when the shortest arc connecting <tt>from</tt> to <tt>to</tt> is ccw.
         */
         virtual double SignedDistance(const double from,
                                      const double to)
        {
            return Algebra2D::AngleSignedDistance(from, to);
        }
    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to SignedDistanceBetweenTwoAngles.
     */        
    ClassContainer(SignedDistanceBetweenTwoAngles, m_signedDistanceAngle);
    
}

#endif
