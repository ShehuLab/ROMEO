#ifndef Antipatrea__Reader_HPP_
#define Antipatrea__Reader_HPP_

#include <istream>

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Interface for reading from an input stream.
     */ 
    class Reader
    {
    public:
	Reader(void) 
	{
	}
	
	virtual ~Reader(void)
	{
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Read a double value from the input stream.
	 *
	 *@remarks
	 * - ReadDouble will read not only double values but also inf,
	 *   which will be assigned the value INFINITY.
  	 */ 
	static std::istream& ReadDouble(std::istream & in, double & val);

	

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Open a file for reading, read the content, and then close the file.
	 *
	 *@remarks
	 * - The file is opened in ios_base::in mode by default.
	 * - The function Read is used to read the content from the file.
	 */
	virtual void ReadFromFile(const char fname[],
				  std::ios_base::openmode mode = std::ios_base::in);	
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Read the content from the input stream.
	 */
	virtual std::istream& Read(std::istream & in) = 0;
	
    };
    
}

#endif



