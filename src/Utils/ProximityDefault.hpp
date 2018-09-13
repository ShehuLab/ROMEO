#ifndef Antipatrea__ProximityDefault_HPP_
#define Antipatrea__ProximityDefault_HPP_

#include "Utils/ProximityGNAT.hpp"

namespace Antipatrea
{
    template <typename Key, typename DistFnData>
    class ProximityDefault : public ProximityGNAT<Key, DistFnData>
    { 
    public:
        ProximityDefault(void) : ProximityGNAT<Key, DistFnData>()
        {
        }
        
        
        virtual ~ProximityDefault(void)
        {
        }
    };
}

#endif








