#ifndef Antipatrea__Params_HPP_
#define Antipatrea__Params_HPP_

#include "Utils/Reader.hpp"
#include "Utils/Writer.hpp"
#include "Utils/Misc.hpp"
#include <cstdlib>
#include <string>
#include <climits>
#include <cmath>
#include <unordered_map>

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Class to facilitate setting of parameter values.
     *
     *@remarks
     * - A parameter is defined by a keyword and the data associated with that keyword.
     * - The data could be a single value, a list of values, or a group of parameters (pointer to an instance of Params).
     * - The data is retrieved by using the keyword as the index.
     * - After retrieving the data, the calling function can enumerate the list of values or access the group of parameters.
     * - The values can be converted to strings, integers, doubles, or booleans.
     * - The parameters can also be read from an input stream and written to an output stream.
     *   The format is as follows:
     *    - <Entries> := <Entry> | <Entry><Whitespace><Entries>
     *    - <Entry>   := <Comment> | <ParamName><Data>
     *    - <Comment> := any line where the first character is #
     *    - <ParamName> := string (no spaces or other whitespaces)
     *    - <Data> := <SingleValue> | <ListOfValues> | <Group>
     *    - <SingleValue> := string 
     *    - <ListOfValues> := [<Whitespace><Values><Whitespace>]
     *    - <Values> := <SingleValue> | <SingleValue> <Whitespace> <Values>
     *    - <Group>  := {<Whitespace><Entries><Whitespace>}
     *    - <Whitespace> := any white space character, e.g, empty space, tab, newline.
     *    - An example is provided below:<br> 
          @verbatim
          NrPoints 10 
          Dims [32 64]
          PRM { NrNeighbors 10 Grid { Dims [10 20] Min [-10 10] Max [-10 10] } }   
          @endverbatim
     */
    class Params : public Reader,
                   public Writer
    {
    public:
        Params(void) : Reader(),
                       Writer()
        {
        }
        
        virtual ~Params(void);
        
        struct Data
        {
            Data(void) : m_params(NULL)
            {
            }

            virtual ~Data(void)
            {
                if(m_params)
                    delete m_params;
                DeleteItems<std::string*>(m_values);
            }

            std::vector<std::string*> m_values;
            Params                   *m_params;
        };

        virtual Data* GetData(const char id[]);
        
        virtual const char* GetValue(const char id[], const char notFound[] = NULL);

        virtual double GetValueAsDouble(const char id[], const double notFound = INFINITY)
        {
            const char *str = GetValue(id);
            if(str == NULL)
                return notFound;
            return StrToDouble(str);
        }

        virtual int GetValueAsInt(const char id[], const int notFound = INT_MAX)
        {
            const char *str = GetValue(id);
            if(str == NULL)
                return notFound;
            return StrToInt(str);
        }

        virtual bool GetValueAsBool(const char id[], const bool notFound = false)
        {
            const char *str = GetValue(id);
            if(str == NULL)
                return notFound;
            return StrToBool(str);
        }
                
        virtual void SetValue(const char id[], const char val[]);

        virtual void AddValue(const char id[], const char val[]);
        
        virtual void SetParams(const char id[], Params * const params);
        
        virtual std::istream& Read(std::istream & in);

        virtual std::ostream& Print(std::ostream & out) const;
        
        static Params* GetSingleton(void)
        {
            return m_singleton;
        }
        
    protected:
                    
        static Params *m_singleton;

        std::unordered_map<std::string, Data*> m_map;
    };
    
}

#endif



