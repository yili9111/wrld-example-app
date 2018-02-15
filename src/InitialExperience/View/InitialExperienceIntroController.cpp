// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#include "InitialExperienceIntroController.h"
#include "IInitialExperienceIntroView.h"
#include "InitialExperienceIntroDismissedMessage.h"
#include "ICameraTransitionController.h"
#include "CameraTransitionControllerChangedMessage.h"
#include "ICompassModule.h"
#include "ICompassModel.h"

namespace ExampleApp
{
    namespace InitialExperience
    {
        namespace
        {
            const int maxTutorialViews = 2;
        }
        
        namespace View
        {
            InitialExperienceIntroController::InitialExperienceIntroController(IInitialExperienceIntroView& view,
                                                                               ExampleAppMessaging::TMessageBus& messageBus,
                                                                               bool isInKioskMode,
                                                                               CameraTransitions::SdkModel::ICameraTransitionController& cameraTransitionController,
                                                                               const Compass::SdkModel::ICompassModule& compassModule)
            : m_view(view)
            , m_messageBus(messageBus)
            , m_showIntroMessageHandler(this, &InitialExperienceIntroController::OnShowIntro)
            , m_viewDismissed(this, &InitialExperienceIntroController::OnViewDismissed)
            , m_replayExitIUX(isInKioskMode ? true : false)
            , m_exitIUXViewedCount(isInKioskMode ? 0 : maxTutorialViews)
            , m_currAppMode(AppModes::SdkModel::WorldMode)
            , m_shouldShowExitIUX(false)
            , m_appModeChangedHandler(this, &InitialExperienceIntroController::OnAppModeChangedMessage)
            , m_cameraTransitionController(cameraTransitionController)
            , m_transitionCompleteHandler(this, &InitialExperienceIntroController::OnTransitionCompleteHandler)
            , m_compassModule(compassModule)
            {
                m_view.InsertDismissedCallback(m_viewDismissed);
                m_messageBus.SubscribeUi(m_showIntroMessageHandler);
                m_messageBus.SubscribeUi(m_appModeChangedHandler);
                m_messageBus.SubscribeUi(m_transitionCompleteHandler);
            }
            
            InitialExperienceIntroController::~InitialExperienceIntroController()
            {
                m_messageBus.UnsubscribeUi(m_transitionCompleteHandler);
                m_messageBus.UnsubscribeUi(m_appModeChangedHandler);
                m_messageBus.UnsubscribeUi(m_showIntroMessageHandler);
                m_view.RemoveDismissedCallback(m_viewDismissed);
            }
            
            void InitialExperienceIntroController::OnShowIntro(const ShowInitialExperienceIntroMessage& message)
            {
                m_view.Show();
            }
            
            void InitialExperienceIntroController::OnViewDismissed()
            {
                m_messageBus.Publish(InitialExperienceIntroDismissedMessage());
            }

            void InitialExperienceIntroController::OnAppModeChangedMessage(const AppModes::AppModeChangedMessage& message)
            {
                    m_shouldShowExitIUX = false;

                    AppModes::SdkModel::AppMode newAppMode = message.GetAppMode();
                
                    if(newAppMode == AppModes::SdkModel::AttractMode)
                    {
                        m_view.DismissExitIUX();
                        ReplayExitIUX(true);
                        m_exitIUXViewedCount = 0;
                    }
                    else if(newAppMode == AppModes::SdkModel::WorldMode && m_currAppMode == AppModes::SdkModel::InteriorMode && m_compassModule.GetCompassModel().GetGpsMode() == Compass::GpsMode::Values::GpsDisabled)
                    {
                        if (m_cameraTransitionController.IsTransitioning())
                        {
                            m_shouldShowExitIUX = true;
                        }
                        else
                        {
                            ShowExitIUX();
                        }
                    }

                    m_currAppMode = newAppMode;
            }

            void InitialExperienceIntroController::OnTransitionCompleteHandler(const CameraTransitions::CameraTransitionControllerChangedMessage& message)
            {
                if (!m_cameraTransitionController.IsTransitioning() && m_shouldShowExitIUX)
                {
                    m_shouldShowExitIUX = false;
                    ShowExitIUX();
                }
            }

            void InitialExperienceIntroController::ShowExitIUX()
            {
                if(m_replayExitIUX || m_exitIUXViewedCount < maxTutorialViews)
                {
                    m_view.ShowExitIUX();
                    ReplayExitIUX(false);
                    ++m_exitIUXViewedCount;
                }
            }

            void InitialExperienceIntroController::ReplayExitIUX(const bool enableTutorials)
            {
                m_replayExitIUX = enableTutorials;
                m_replayExitIUXCallbacks.ExecuteCallbacks(m_replayExitIUX);
            }

            void InitialExperienceIntroController::InsertReplayExitIUXChangedCallback(Eegeo::Helpers::ICallback1<bool>& callback)
            {
                m_replayExitIUXCallbacks.AddCallback(callback);
            }

            void InitialExperienceIntroController::RemoveReplayExitIUXChangedCallback(Eegeo::Helpers::ICallback1<bool>& callback)
            {
                m_replayExitIUXCallbacks.RemoveCallback(callback);
            }
        }
    }
}
