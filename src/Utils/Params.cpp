#include "Utils/Params.hpp"
#include "Utils/Logger.hpp"
#include <iomanip>

namespace Antipatrea
{
    
    Params* Params::m_singleton = new Params();

    Params::~Params(void)
    {
	m_map.clear();
    }

    Params::Data* Params::GetData(const char id[])
    {
	auto iter = m_map.find(id);
	if(iter == m_map.end())
	    return NULL;
	return iter->second;
    }
    
    
    const char* Params::GetValue(const char id[], const char notFound[]) 
    {
	auto data = GetData(id);
	if(data == NULL || data->m_values.size() == 0)
	    return notFound;
	return data->m_values[0]->c_str();
    }

    void Params::SetParams(const char id[], Params * const params)
    {
	auto iter = m_map.find(id);
	if(iter != m_map.end())
	{
	    Data *data = iter->second;
	    if(data->m_params)
		delete data->m_params;
	    data->m_params = params;
	}
	else
	{
	    Data *data = new Data();
	    data->m_params = params;
	    m_map.insert(std::make_pair(id, data));
	}
	
    }
    
    void Params::SetValue(const char id[], const char val[])
    {
	auto iter = m_map.find(id);
	if(iter != m_map.end())
	{
	    Data *data = iter->second;
	    if(data->m_values.size() == 0)
		data->m_values.push_back(new std::string(val));
	    else
		*(data->m_values[0]) = val;
	}
	else
	{
	    Data *data = new Data();
	    data->m_values.push_back(new std::string(val));
	    m_map.insert(std::make_pair(id, data));
	}
    }

    void Params::AddValue(const char id[], const char val[])
    {
	auto iter = m_map.find(id);
	if(iter != m_map.end())
	    iter->second->m_values.push_back(new std::string(val));
	else
	{
	    Data *data = new Data();
	    
	    data->m_values.push_back(new std::string(val));
	    m_map.insert(std::make_pair(id, data));
	}
	
    }
    
    std::ostream& Params::Print(std::ostream & out) const
    {
	static const Params *original = this;
	
	for(auto &iter : m_map)
	{
	    auto data = iter.second;
	    
	    out << " " << iter.first.c_str() << " ";
	    if(data->m_values.size() > 0)
	    {
		out << "[ ";
		for(auto & val : data->m_values)
		    out << *val << " ";
		out << "] ";
	    }
	    if(data->m_params != NULL)
	    {
		out << "{";
		data->m_params->Print(out);
		out << " } ";
	    }
	    if(original == this)
		out << std::endl;
	}
	
	return out;
    }

    std::istream& Params::Read(std::istream &in)
    {
	std::string id;
	std::string val;
	std::string comment;
	
	while(in >> id)
	{
	     if(id[0] == '#') //comment
		 std::getline(in, comment);
	     else if(id.back() == '}')
		 return in;
	     else
	     {
		 in >> val;
		 if(in.good() == false)
		     return in;
		 if(StrSameContent(id.c_str(), "ParamsExtraFile"))
		 {
		     Logger::m_out << "...using ParamsExtraFile " << val << std::endl;
		     ReadFromFile(val.c_str());
		 }
		 else if(val.length() > 0 && val[0] == '[')
		 {
		     if(val.length() > 1)
			 AddValue(id.c_str(), &(val.c_str()[1]));
		     
		     while((in >> val))
		     {
			 if(val.back() == ']')
			 {
			     val.pop_back();
			     if(!val.empty())
				 AddValue(id.c_str(), val.c_str());
			     break;
			 }
			 else			 
			     AddValue(id.c_str(), val.c_str());
		     }
		 }
		 else if(val.length() > 0 && val[0] == '{')
		 {
		     Params *params = new Params();
		     params->Read(in);
		     SetParams(id.c_str(), params);
		 }
		 else
		     SetValue(id.c_str(), val.c_str());
	     }
	}
	return in;
	
    }
    
}


