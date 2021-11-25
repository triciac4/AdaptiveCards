// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
#include "pch.h"

#include "AdaptiveShowCardActionRenderer.h"
#include "AdaptiveShowCardActionRenderer.g.cpp"
#include "ActionHelpers.h"

namespace winrt::AdaptiveCards::Rendering::WinUI3::implementation
{
    rtxaml::UIElement AdaptiveShowCardActionRenderer::Render(rtom::IAdaptiveActionElement const& action,
                                                             rtrender::AdaptiveRenderContext const& renderContext,
                                                             rtrender::AdaptiveRenderArgs const& renderArgs)
    {
        try
        {
            return ::AdaptiveCards::Rendering::WinUI3::ActionHelpers::BuildAction(action, renderContext, renderArgs, false);
        }
        catch (winrt::hresult_error const& ex)
        {
            // TODO: what do we do here?
            return nullptr;
        }
    }
    rtxaml::UIElement AdaptiveShowCardActionRenderer::BuildShowCard(rtom::AdaptiveCard const& showCard,
                                                                    rtrender::AdaptiveRenderContext const& renderContext,
                                                                    rtrender::AdaptiveRenderArgs const& renderArgs,
                                                                    bool isBottomActionBar)
    {
        try
        {
            auto hostConfig = renderContext.HostConfig();
            auto actionsConfig = hostConfig.Actions();
            auto showCardActionConfig = actionsConfig.ShowCard();
            auto showCardConfigStyle = showCardActionConfig.Style();

            bool wasInShowCard = renderArgs.IsInShowCard();
            renderArgs.IsInShowCard(true);
            renderContext.LinkCardToParent(showCard, renderArgs);

            auto localUiShowCard =
                ::AdaptiveCards::Rendering::WinUI3::XamlBuilder::BuildXamlTreeFromAdaptiveCard(showCard, renderContext, nullptr, showCardConfigStyle);
            renderArgs.IsInShowCard(wasInShowCard);

            // Set the padding
            auto spacingConfig = hostConfig.Spacing();
            uint32_t padding = spacingConfig.Padding();

            rtrender::ActionMode showCardActionmode = showCardActionConfig.ActionMode();

            // Set the top margin
            uint32_t inlineTopMargin = showCardActionConfig.InlineTopMargin();

            const double negativePadding = -(static_cast<double>(padding));
            const double sideMargin = negativePadding;
            const double topMargin = isBottomActionBar ? inlineTopMargin + padding : inlineTopMargin;
            const double bottomMargin = negativePadding;

            localUiShowCard.Margin({sideMargin, topMargin, sideMargin, bottomMargin});

            // Set the visibility as Collapsed until the action is triggered
            localUiShowCard.Visibility(rtxaml::Visibility::Collapsed);

            return localUiShowCard;
        }
        catch (winrt::hresult_error const& ex)
        {
            // TODO: what do we do here?
            return nullptr;
        }
    }
}