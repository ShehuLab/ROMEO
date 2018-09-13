#ifndef Antipatrea__DisjointSet_HPP_
#define Antipatrea__DisjointSet_HPP_

#include <vector>

namespace Antipatrea
{       
    class DisjointSet
    {
    public:
        DisjointSet(void) : m_nrComps(0)
        {
        }

        ~DisjointSet(void)
        {
        }

        struct Elem
        {
            int    m_rank;
            Elem  *m_parent;
        };


        int GetNrComponents(void) const
        {
            return m_nrComps;
        }

        Elem* Make(void);
        
        Elem* Find(Elem * elem);
        
        void Join(Elem * x, Elem * y);
        
        bool Same(Elem * x, Elem * y)
        {
            return Find(x) == Find(y);
        }

    protected:
        int  m_nrComps;
    };
};

#endif










