// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#include "SwallowDepartmentResultModel.h"

namespace ExampleApp
{
    namespace Search
    {
        namespace Swallow
        {
            namespace SdkModel
            {
                SwallowDepartmentResultModel::SwallowDepartmentResultModel(const std::string& name,
                                                                           const std::string& imageUrl,
                                                                           const std::string& description,
                                                                           const std::vector<std::string>& desks)
                : m_name(name)
                , m_imageUrl(imageUrl)
                , m_description(description)
                , m_desks(desks)
                {
                }
                
                SwallowDepartmentResultModel::SwallowDepartmentResultModel()
                {
                    
                }
                
                SwallowDepartmentResultModel::~SwallowDepartmentResultModel()
                {
                    
                }
                
                const std::string& SwallowDepartmentResultModel::GetName() const
                {
                    return m_name;
                }
                
                const std::string& SwallowDepartmentResultModel::GetImageUrl() const
                {
                    return m_imageUrl;
                }
                
                const std::string& SwallowDepartmentResultModel::GetDescription() const
                {
                    return m_description;
                }
                
                const size_t SwallowDepartmentResultModel::GetNumberOfDesks() const
                {
                    return m_desks.size();
                }
                
                const std::string& SwallowDepartmentResultModel::GetDeskAt(int idx) const
                {
                    return m_desks.at(idx);
                }
                
                const std::vector<std::string>& SwallowDepartmentResultModel::GetAllDesks() const
                {
                    return m_desks;
                }
            }
        }
    }
}