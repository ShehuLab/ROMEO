#ifndef Antipatrea__Object_HPP_
#define Antipatrea__Object_HPP_

#include "Utils/Definitions.hpp"
#include "Utils/Logger.hpp"
#include <typeinfo>
#include <ostream>

namespace Antipatrea
{	
    class Object
    {
    public:
	Object(void)
	{
	}
	
	virtual ~Object(void)
	{
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Return the class name associated with the object.
	 */
	static const char* Name(const Object * const obj)
	{
	    return obj ? typeid(*obj).name() : "null";
	}
	

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Print the paramater values and the names of the components
	 *       used by this class.
	 * 
	 *@remarks
	 * - This function is useful for debugging purposes to see
	 *   that the component has been setup as desired.
	 * - prefix is used to ensure proper alignment when printing (you can use "" for the prefix, for example).
	 */
	virtual void Info(const char prefix[]) const
	{
	    Logger::m_out << prefix << Name(this) << std::endl;
	}
    };
}

#endif
