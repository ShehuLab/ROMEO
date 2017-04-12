#ifndef Antipatrea__Cfg_HPP_
#define Antipatrea__Cfg_HPP_

#include <cstdlib>

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Configuration representation.
     * 
     *@remarks
     * - A configuration is represented by its values which could correspond to joint/dihedral angles.
     * - The energy value is also added as a field to the configuration so that once it is evaluated it is stored there.
     * - Other interpretations of values are possible -- it all depends on the motion-planning problem
     *   that is being solved. See Plugin2D/ for alternative examples. 
     * - By design, Cfg does not know how to allocate/copy itself -- in particular, it does not store its dimension.
     *   There is another class, CfgManager, which is responsible for allocating/copying configurations.
     *   For this reason, the constructor is made protected and the copy constructor/assignment operator are disabled.
     * - In this way, a planner/component can have access to CfgManager to create new configurations. 
     * - Why design it this way? Consider the alternative where Cfg allocates itself. Then, inside a planner/component 
     *   one could write <tt>Cfg *cfg = new Cfg()</tt> to create a new configuration. This is problematic however if someone
     *   extends the configuration class, e.g., <tt>class MyCfg : public Cfg</tt> and wants to use <tt>MyCfg</tt> instead of <tt>Cfg</tt> for
     *   planning purposes. By using CfgManager, such issue is avoided since one would write a class <tt>class MyCfgManager : public CfgManager</tt>,
     *   set the allocator for the planner/component to an instance of MyCfgManager, and then write inside the planner/component
     *   <tt>Cfg *cfg = GetCfgManager()->NewCfg() </tt>.
     */
    class Cfg
    {
    public:
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Special value to indicate that the energy of the configuration is undefined and needs to be computed.
	 *       It should be set to some value (by default its INFINITY) that does not correspond to an actual energy value.
	 */
	static double ENERGY_UNDEFINED;
	
	virtual ~Cfg(void)
	{
	    if(m_values)
		delete [] m_values;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get pointer to values.
	 *
	 *@remarks
	 * - Function returns just the pointer, not a deep copy.
	 * - The values are allocated by CfgManager.
	 */
	virtual const double* GetValues(void) const
	{
	    return m_values;
	}

	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get pointer to values.
	 *
	 *@remarks
	 * - Function returns just the pointer; not a deep copy.
	 * - The values are allocated by CfgManager.
	 */
	virtual double* GetValues(void)
	{  
	    return m_values;
	}

	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set pointer to values and mark the energy field as undefined.
	 *
	 *@remarks
	 * - Function just does pointer assignment; not  a deep copy.
	 * - The values are allocated by CfgManager.
	 * - The energy field is marked as undefined to indicate
	 *   that the energy should be computed (since the values changed).
	 */
	virtual void SetValues(double * const values)
	{
	    m_values = values;
	    SetEnergy(ENERGY_UNDEFINED);
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get the energy value associated with the configuration.
	 *
	 *@remarks
	 *  - The function just returns the energy value stored with the configuration.
	 *  - It does not perform an energy evaluation.
	 */
	virtual double GetEnergy(void) const
	{
	    return m_energy;
	}

	/**
         *@author Erion Plaku, Amarda Shehu
	 *@brief Set the energy value.
	 */
	virtual void SetEnergy(const double energy)
	{
	    m_energy = energy;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Return true iff the energy value is not the same as ENERGY_UNDEFINED.
	 */
	virtual bool IsEnergySet(void) const
	{
	    return GetEnergy() != ENERGY_UNDEFINED;
	}
	
    protected:
	/**
         *@author Erion Plaku, Amarda Shehu
	 *@brief Constructor is made protected since new configurations should be allocated only via CfgManager.
	 */
	Cfg(void) : m_values(NULL),
		    m_energy(ENERGY_UNDEFINED)
	{
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Copy constructor is disabled since copies should be performed only via CfgManager.
	 */
	Cfg(const Cfg&) = delete;

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Assignment operator is disabled since copies should be performed only via CfgManager.
	 */
	Cfg& operator=(const Cfg&) = delete; 

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Pointer to configuration values.
	 */
	double *m_values;

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Energy value.
	 */
	double  m_energy;
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief CfgManager performs allocate/copy functions for configurations.
	 */
	friend class CfgManager;
	
    };
}

#endif
