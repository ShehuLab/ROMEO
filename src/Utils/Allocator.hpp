#ifndef Antipatrea__Allocator_HPP_
#define Antipatrea__Allocator_HPP_

namespace Antipatrea
{	
   /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Facilitates creating, deleting, and copying an array of values.
     */
    class Allocator
    {
    public:
	Allocator(const int dim = 0) :  m_dim(dim)
	{
	}
	
	virtual ~Allocator(void)
	{
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set dimension.
	 *
	 *@remarks
	 * - The dimension should be set once at the beginning before any array is allocated.
	 */
	virtual void SetDim(const int dim)
	{
	    m_dim = dim;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get the dimension.
	 */
	virtual int  GetDim(void) const
	{
	    return m_dim;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Allocate memory for the array.
	 */
	virtual double* NewValues(void) const
	{
	    return new double[GetDim()];
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Delete the memory allocated to the array.
	 */
	virtual void DeleteValues(double values[]) const
	{
	    if(values)
		delete[] values;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Copy the values from <tt>src</tt> to <tt>dest</tt>.
	 */
	virtual void CopyValues(double dest[], const double src[]) const
	{
	    memcpy(dest, src, GetDim() * sizeof(double));
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Return a copy of the array.
	 */
	virtual double* CopyValues(const double values[]) const
	{
	    double *dest = NewValues();
	    CopyValues(dest, values);
	    return dest;	    
	}

    protected:
        /**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Array dimension.
	 */
	int m_dim;
	
    };
}

#endif
