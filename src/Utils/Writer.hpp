#ifndef Antipatrea__Writer_HPP_
#define Antipatrea__Writer_HPP_

#include <ostream>

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Interface for writing to an output stream.
     */ 
    class Writer
    {
    public:
	Writer(void) 
	{
	}
	
	virtual ~Writer(void)
	{
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Open a file for writing, write the content, and then close the file.
	 *
	 *@remarks
	 * - The file is opened in ios_base::out mode by default.
	 */
	virtual void PrintToFile(const char fname[],
				 std::ios_base::openmode mode = std::ios_base::out) const;
		
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Write to the output stream.
	 */
	virtual std::ostream& Print(std::ostream & out) const = 0;
    };
    
}

#endif



