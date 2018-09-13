#ifndef Antipatrea__Flags_HPP_
#define Antipatrea__Flags_HPP_

namespace Antipatrea
{
    typedef unsigned int Flags;
   
    static inline
    bool HasFlag(const Flags f, const Flags b)
    {
        return f & b;
    }
          
    static inline
    Flags AddFlag(const Flags f, const Flags b)
    {
        return f | b;
    }
        

    static inline
    Flags RemoveFlag(const Flags f, const Flags b)
    {
        return f & (~b);
    }
    
    static inline
    Flags RemoveAndAddFlag(const Flags f, const Flags rem, const Flags add)
    {
        return (f & (~rem)) | add;        
    }
    

    static inline
    Flags FlipFlag(const Flags f, const Flags b)
    {
        return f ^ b;
    }
}

#endif
    
    
    
    







