// Copyright WRLD Ltd (2018-), All Rights Reserved

#pragma once

#include "AndroidNativeState.h"
#include "ISearchProvider.h"
#include "CallbackCollection.h"

namespace ExampleApp
{
    namespace SearchProviders
    {
		class MyTestSearchProvider : public SearchMenu::View::ISearchProvider, private Eegeo::NonCopyable
		{
		private:
			typedef SearchMenu::View::ISearchProvider::TSearchResults TSearchResults;

			Eegeo::Helpers::CallbackCollection1<const std::string&> m_searchPerformedCallbacks;

			AndroidNativeState& m_nativeState;
			jclass m_javaClass;
			jobject m_javaInstance;
			jmethodID m_onSearchCompleted;

		public:
			MyTestSearchProvider(AndroidNativeState& nativeState);
			~MyTestSearchProvider();

			jobject GetJavaInstance();

			void InsertSearchPerformedCallback(Eegeo::Helpers::ICallback1<const std::string&>& callback);
			void RemoveSearchPerformedCallback(Eegeo::Helpers::ICallback1<const std::string&>& callback);

			void PerformSearch(const std::string& searchQuery);

			void OnSearchResponseReceived(const TSearchResults& searchResults);
		};
    }
}
