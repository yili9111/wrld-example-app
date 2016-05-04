// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#include "SwallowSearchService.h"
#include "SearchQuery.h"
#include "SwallowSearchConstants.h"
#include "SwallowPoiDbServiceProvider.h"
#include "SwallowPoiDbCombinedService.h"
#include <sstream>
#include "SwallowSearchConstants.h"
#include "SwallowSearchParser.h"

namespace ExampleApp
{
    namespace Search
    {
        namespace Swallow
        {
            namespace SdkModel
            {
                SwallowSearchService::SwallowSearchService(const std::vector<std::string>& availableCategories,
                                                           SwallowPoiDb::SwallowPoiDbServiceProvider& swallowPoiDbServiceProvider)
                : Search::SdkModel::SearchServiceBase(availableCategories)
                , m_swallowPoiDbServiceProvider(swallowPoiDbServiceProvider)
                {
                    
                }
                
                void SwallowSearchService::CancelInFlightQueries()
                {
                    
                }
                
                void SwallowSearchService::PerformLocationQuerySearch(const Search::SdkModel::SearchQuery& query)
                {
                    SwallowPoiDb::SwallowPoiDbCombinedService* swallowPoiDbService;
                    
                    ExecuteQueryPerformedCallbacks(query);
                    
                    if(m_swallowPoiDbServiceProvider.TryGetSwallowPoiDbService(swallowPoiDbService))
                    {
                        if(query.IsCategory())
                        {
                            PerformCategorySearch(query, *swallowPoiDbService);
                        }
                        else
                        {
                            PerformFullTextSearch(query, *swallowPoiDbService);
                        }
                    }
                    else
                    {
                        ExecutQueryResponseReceivedCallbacks(query, std::vector<Search::SdkModel::SearchResultModel>(), false);
                    }
                }
                    
                void SwallowSearchService::PerformIdentitySearch(const Search::SdkModel::SearchResultModel& outdatedSearchResult,
                                                                 Eegeo::Helpers::ICallback1<const Search::SdkModel::IdentitySearchCallbackData&>& callback)
                {
                    // Not implemented.
                    Search::SdkModel::IdentitySearchCallbackData result = Search::SdkModel::IdentitySearchCallbackData::CreateFailed(outdatedSearchResult);
                    callback(result);
                }
                
                void SwallowSearchService::PerformFullTextSearch(const Search::SdkModel::SearchQuery& query, SwallowPoiDb::SwallowPoiDbCombinedService& swallowPoiDbService)
                {
                    std::vector<Search::SdkModel::SearchResultModel> results;
                    
                    swallowPoiDbService.FullTextSearch(query.Query(), results);
                    
                    ExecuteQueryPerformedCallbacks(query);
                    ExecutQueryResponseReceivedCallbacks(query, results, true);
                }
                
                void SwallowSearchService::PerformCategorySearch(const Search::SdkModel::SearchQuery& query, SwallowPoiDb::SwallowPoiDbCombinedService& swallowPoiDbService)
                {
                    std::vector<Search::SdkModel::SearchResultModel> results;
                    
                    swallowPoiDbService.CategorySearch(query.Query(), results);
                    
                    ExecuteQueryPerformedCallbacks(query);
                    ExecutQueryResponseReceivedCallbacks(query, results, true);
                }
            }
        }
    }
}