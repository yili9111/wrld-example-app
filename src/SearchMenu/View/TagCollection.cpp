// Copyright WRLD Ltd (2018-), All Rights Reserved

#include "TagCollection.h"

namespace ExampleApp
{
	namespace SearchMenu
	{
		namespace View
		{
			TagCollection::TagCollection(ExampleAppMessaging::TMessageBus& messageBus)
			: m_messageBus(messageBus)
			, m_onTagSearchAddedHandler(this, &TagCollection::OnTagSearchAdded)
			, m_onTagSearchSwallowLoadedHandler(this, &TagCollection::OnTagSearchSwallowLoaded)
			{
				m_messageBus.SubscribeUi(m_onTagSearchAddedHandler);
				m_messageBus.SubscribeUi(m_onTagSearchSwallowLoadedHandler);
			}

			TagCollection::~TagCollection()
			{
				m_messageBus.UnsubscribeUi(m_onTagSearchSwallowLoadedHandler);
				m_messageBus.UnsubscribeUi(m_onTagSearchAddedHandler);
			}

			void TagCollection::OnTagSearchAdded(const TagSearch::TagSearchAddedMessage& message)
			{
				const TagSearch::View::TagSearchModel& tagSearchModel = message.Model();

				RememberTag(tagSearchModel.Name(), tagSearchModel.SearchTag(),
							tagSearchModel.Interior(),
							false, 0);
			}

			void TagCollection::OnTagSearchSwallowLoaded(const TagSearch::TagSearchSwallowLoadedMessage& message)
			{
				RememberTag(message.Key(), message.Tag(),
							message.ShouldTryInterior(),
							message.HasRadiusOverride(), message.RadiusOverride());
			}

			void TagCollection::RememberTag(const std::string& text, const std::string& tag,
											bool shouldTryInterior,
											bool hasRadiusOverride, float radiusOverride)
			{
				Eegeo_ASSERT(m_tagsByText.find(text) == m_tagsByText.end());
				Eegeo_ASSERT(m_tagsByTag .find(tag)  == m_tagsByTag .end());

				m_tagStorage.push_back(TagInfo(tag, text, shouldTryInterior, hasRadiusOverride, radiusOverride));

				int last = (int)m_tagStorage.size() - 1;

				m_tagsByText[text] = last;
				m_tagsByTag [tag]  = last;
			}

			bool TagCollection::HasTag(const std::string& text)
			{
				return m_tagsByText.find(text) != m_tagsByText.end();
			}

			const TagCollection::TagInfo& TagCollection::GetInfoByText(const std::string& text)
			{
				TTagTextMap::iterator it = m_tagsByText.find(text);
				Eegeo_ASSERT(it != m_tagsByText.end());

				return m_tagStorage[it->second];
			}

			const TagCollection::TagInfo& TagCollection::GetInfoByTag(const std::string& tag)
			{
				TTagTextMap::iterator it = m_tagsByTag.find(tag);
				Eegeo_ASSERT(it != m_tagsByTag.end());

				return m_tagStorage[it->second];
			}
		}
	}
}