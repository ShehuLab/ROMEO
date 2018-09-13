#include "Plugin2D/Chain2D.hpp"
#include "Setup/Defaults.hpp"
#include "Utils/GDraw.hpp"
#include <cmath>

namespace Antipatrea
{
    Chain2D::Chain2D(void) : CfgForwardKinematics()
    {
        m_positions.push_back(0.0);
        m_positions.push_back(0.0);
    }
    
    Chain2D::~Chain2D(void)
    {
    }

    void Chain2D::Info(const char prefix[]) const
    {
        const int n = GetNrLinks();
        
        CfgForwardKinematics::Info(prefix);
        Logger::m_out << prefix << " NrLinks     = " << n << std::endl           
                      << prefix << " LinkLengths = ";
        for(int i = 0; i < n; ++i)
            Logger::m_out << GetLinkLength(i) << " ";
        Logger::m_out << std::endl;
    }
    
    void Chain2D::SetupFromParams(Params & params)
    {
        CfgForwardKinematics::SetupFromParams(params);

        auto data = params.GetData(Constants::KW_Chain2D);
        if(data && data->m_params)
        {
            const int    n = data->m_params->GetValueAsInt(Constants::KW_NrLinks, Constants::VAL_Chain2D_NrLinks);
            const double d = data->m_params->GetValueAsDouble(Constants::KW_LinkLength, Constants::VAL_Chain2D_LinkLength);
            
            RemoveAllLinks();
            AddLinks(n, d);
        }
    }
    
    void Chain2D::RemoveAllLinks(void)
    {
        m_joints.clear();
        m_lengths.clear();
        m_positions.clear();
        m_positions.push_back(0.0);
        m_positions.push_back(0.0);
    }
    
    
    void Chain2D::AddLinks(const int n, const double length)
    {
        for(int i = 0; i < n; ++i)
            AddLink(length);
    }
    
    void Chain2D::AddLink(const double length)
    {
        m_joints.push_back(0);
        m_lengths.push_back(length);
        m_positions.resize(m_positions.size() + 2);

        OnValuesChanged();
    }
    
    void Chain2D::MatrixMultMatrix(const double M1[6], const double M2[6], double M[6])
    {
        double Mresult[6];
        
        Mresult[0] = M1[0] * M2[0] + M1[1] * M2[3];
        Mresult[1] = M1[0] * M2[1] + M1[1] * M2[4];
        Mresult[2] = M1[0] * M2[2] + M1[1] * M2[5] + M1[2];
        
        Mresult[3] = M1[3] * M2[0] + M1[4] * M2[3];
        Mresult[4] = M1[3] * M2[1] + M1[4] * M2[4];
        Mresult[5] = M1[3] * M2[2] + M1[4] * M2[5] + M1[5];
        
        M[0] = Mresult[0];
        M[1] = Mresult[1];
        M[2] = Mresult[2];
        M[3] = Mresult[3];
        M[4] = Mresult[4];
        M[5] = Mresult[5];
    }
    
    void Chain2D::DoFK(void)
    {
        const int n = GetNrLinks();
        
        double M[6];
        double Mall[6];
        double p[2] = {0, 0};
        double pnew[2];
        
        
        Mall[0] = Mall[4] = 1;
        Mall[1] = Mall[2] = Mall[3] = Mall[5] = 0;
        
        for(int i = 0; i < n; ++i)
        {
            const double ctheta = cos(GetLinkTheta(i));
            const double stheta = sin(GetLinkTheta(i));
            
            M[0] = ctheta;  M[1] = -stheta; M[2] = GetLinkLength(i) * ctheta;
            M[3] = stheta;  M[4] =  ctheta; M[5] = GetLinkLength(i) * stheta;
            
            MatrixMultMatrix(Mall, M, Mall);
            
            m_positions[2 * i + 2] = Mall[2];
            m_positions[2 * i + 3] = Mall[5];
        }
    }

    void Chain2D::Draw(void)
    {
        const int n = GetNrLinks();
        for(int i = 0; i < n; ++i)
            GDrawCircle2D(GetLinkStartX(i), GetLinkStartY(i), 0.1);
        for(int i = 0; i < n; ++i)
            GDrawSegment2D(GetLinkStartX(i), GetLinkStartY(i),
                           GetLinkEndX(i), GetLinkEndY(i));
        
    }
    
}
