// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
#pragma once

#include "AdaptiveCards.Rendering.WinUI3.h"
#include "WholeItemsPanel.h"
#include <type_traits>

namespace AdaptiveCards::Rendering::WinUI3::XamlHelpers
{
    namespace rtrender = winrt::AdaptiveCards::Rendering::WinUI3;
    namespace rtom = winrt::AdaptiveCards::ObjectModel::WinUI3;
    namespace rtxaml = winrt::Windows::UI::Xaml;

    template<typename T> Microsoft::WRL::ComPtr<T> CreateABIClass(Microsoft::WRL::Wrappers::HStringReference className)
    {
        Microsoft::WRL::ComPtr<IInspectable> inspectableClass;
        THROW_IF_FAILED(RoActivateInstance(className.Get(), inspectableClass.ReleaseAndGetAddressOf()));

        Microsoft::WRL::ComPtr<T> result;
        THROW_IF_FAILED(inspectableClass.As<T>(&result));
        return result;
    }

    inline Microsoft::WRL::ComPtr<ABI::Windows::UI::Xaml::Media::IBrush> GetSolidColorBrush(_In_ ABI::Windows::UI::Color color)
    {
        Microsoft::WRL::ComPtr<ABI::Windows::UI::Xaml::Media::ISolidColorBrush> solidColorBrush =
            CreateABIClass<ABI::Windows::UI::Xaml::Media::ISolidColorBrush>(
                Microsoft::WRL::Wrappers::HStringReference(RuntimeClass_Windows_UI_Xaml_Media_SolidColorBrush));
        THROW_IF_FAILED(solidColorBrush->put_Color(color));
        Microsoft::WRL::ComPtr<ABI::Windows::UI::Xaml::Media::IBrush> solidColorBrushAsBrush;
        THROW_IF_FAILED(solidColorBrush.As(&solidColorBrushAsBrush));
        return solidColorBrushAsBrush;
    }

    inline rtxaml::Media::Brush GetSolidColorBrush(winrt::Windows::UI::Color const& color)
    {
        rtxaml::Media::SolidColorBrush solidColorBrush;
        solidColorBrush.Color(color);
        return solidColorBrush;
    }

    HRESULT SetStyleFromResourceDictionary(_In_ ABI::AdaptiveCards::Rendering::WinUI3::IAdaptiveRenderContext* renderContext,
                                           HSTRING resourceName,
                                           _In_ ABI::Windows::UI::Xaml::IFrameworkElement* frameworkElement) noexcept;

    HRESULT SetStyleFromResourceDictionary(_In_ ABI::AdaptiveCards::Rendering::WinUI3::IAdaptiveRenderContext* renderContext,
                                           const wchar_t* resourceName,
                                           _In_ ABI::Windows::UI::Xaml::IFrameworkElement* frameworkElement) noexcept;

    void SetStyleFromResourceDictionary(rtrender::AdaptiveRenderContext const& renderContext,
                                        winrt::hstring const& resourceName,
                                        rtxaml::FrameworkElement frameworkElement);

    rtxaml::UIElement CreateSeparator(rtrender::AdaptiveRenderContext const& renderContext,
                                      uint32_t spacing,
                                      uint32_t separatorThickness,
                                      winrt::Windows::UI::Color const& separatorColor,
                                      bool isHorizontal);

    Microsoft::WRL::ComPtr<ABI::Windows::UI::Xaml::IUIElement>
    CreateSeparator(_In_ ABI::AdaptiveCards::Rendering::WinUI3::IAdaptiveRenderContext* renderContext,
                    UINT spacing,
                    UINT separatorThickness,
                    ABI::Windows::UI::Color separatorColor,
                    bool isHorizontal = true);

    template<typename T>
    auto TryGetResourceFromResourceDictionaries(rtxaml::ResourceDictionary const& resourceDictionary, winrt::hstring const& resourceName)
    {
        T toReturn{nullptr};
        try
        {
            auto resourceKey = winrt::box_value(resourceName);
            toReturn = resourceDictionary.TryLookup(resourceKey).try_as<T>();
        }
        catch (...)
        {
        }
        return toReturn;
    }

    template<typename T>
    HRESULT TryGetResourceFromResourceDictionaries(_In_ ABI::Windows::UI::Xaml::IResourceDictionary* resourceDictionary,
                                                   HSTRING resourceName,
                                                   _COM_Outptr_result_maybenull_ T** style) noexcept
    {
        if (resourceDictionary == nullptr)
        {
            return E_INVALIDARG;
        }

        *style = nullptr;
        try
        {
            // Get a resource key for the requested style that we can use for ResourceDictionary Lookups
            ComPtr<IPropertyValueStatics> propertyValueStatics;
            THROW_IF_FAILED(GetActivationFactory(HStringReference(RuntimeClass_Windows_Foundation_PropertyValue).Get(),
                                                 &propertyValueStatics));
            ComPtr<IInspectable> resourceKey;
            THROW_IF_FAILED(propertyValueStatics->CreateString(resourceName, resourceKey.GetAddressOf()));

            // Search for the named resource
            ComPtr<IResourceDictionary> strongDictionary = resourceDictionary;
            ComPtr<IInspectable> dictionaryValue;
            ComPtr<IMap<IInspectable*, IInspectable*>> resourceDictionaryMap;

            boolean hasKey{};
            if (SUCCEEDED(strongDictionary.As(&resourceDictionaryMap)) &&
                SUCCEEDED(resourceDictionaryMap->HasKey(resourceKey.Get(), &hasKey)) && hasKey &&
                SUCCEEDED(resourceDictionaryMap->Lookup(resourceKey.Get(), dictionaryValue.GetAddressOf())))
            {
                ComPtr<T> resourceToReturn;
                if (SUCCEEDED(dictionaryValue.As(&resourceToReturn)))
                {
                    THROW_IF_FAILED(resourceToReturn.CopyTo(style));
                    return S_OK;
                }
            }
        }
        catch (...)
        {
        }
        return E_FAIL;
    }

    template<typename T>
    HRESULT TryGetResourceFromResourceDictionaries(_In_ ABI::Windows::UI::Xaml::IResourceDictionary* resourceDictionary,
                                                   const wchar_t* resourceName,
                                                   _COM_Outptr_result_maybenull_ T** style) noexcept
    {
        return TryGetResourceFromResourceDictionaries<T>(resourceDictionary, HStringReference(resourceName).Get(), style);
    }

    template<typename T>
    T TryGetResourceFromResourceDictionaries(rtxaml::ResourceDictionary const& resourceDictionary, const wchar_t* resourceName)
    {
        return TryGetResourceFromResourceDictionaries<T>(resourceDictionary, winrt::to_hstring(resourceName), style);
    }

    HRESULT SetSeparatorVisibility(_In_ ABI::Windows::UI::Xaml::Controls::IPanel* parentPanel);

    HRESULT HandleColumnWidth(_In_ ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveColumn* column,
                              boolean isVisible,
                              _In_ ABI::Windows::UI::Xaml::Controls::IColumnDefinition* columnDefinition);

    void HandleColumnWidth(winrt::AdaptiveCards::ObjectModel::WinUI3::AdaptiveColumn const& column,
                           bool isVisible,
                           winrt::Windows::UI::Xaml::Controls::ColumnDefinition const& columnDefinition);

    HRESULT HandleTableColumnWidth(_In_ ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveTableColumnDefinition* column,
                                   _In_ ABI::Windows::UI::Xaml::Controls::IColumnDefinition* columnDefinition);

    template<typename T>
    void AppendXamlElementToPanel(_In_ T* xamlElement,
                                  _In_ ABI::Windows::UI::Xaml::Controls::IPanel* panel,
                                  ABI::AdaptiveCards::ObjectModel::WinUI3::HeightType heightType =
                                      ABI::AdaptiveCards::ObjectModel::WinUI3::HeightType::Auto)
    {
        if (!xamlElement)
        {
            return;
        }

        Microsoft::WRL::ComPtr<T> localXamlElement(xamlElement);

        ComPtr<IUIElement> elementToAppend;
        THROW_IF_FAILED(localXamlElement.As(&elementToAppend));

        ComPtr<IVector<UIElement*>> panelChildren;
        THROW_IF_FAILED(panel->get_Children(panelChildren.ReleaseAndGetAddressOf()));

        THROW_IF_FAILED(panelChildren->Append(elementToAppend.Get()));

        if (heightType == ABI::AdaptiveCards::ObjectModel::WinUI3::HeightType::Stretch)
        {
            ComPtr<IPanel> spPanel(panel);
            ComPtr<IWholeItemsPanel> wholeItemsPanel;
            if (SUCCEEDED(spPanel.As(&wholeItemsPanel)))
            {
                ComPtr<WholeItemsPanel> wholeItemsPanelObj = PeekInnards<WholeItemsPanel>(wholeItemsPanel);
                wholeItemsPanelObj->AddElementToStretchablesList(elementToAppend.Get());
            }
        }
    }

    template<typename T>
    void AppendXamlElementToPanel(T const& xamlElement,
                                  rtxaml::Controls::Panel const& panel,
                                  rtom::HeightType heightType = rtom::HeightType::Auto)
    {
        if (!xamlElement)
        {
            return;
        }

        auto elementToAppend = xamlElement.as<rtxaml::UIElement>();
        panel.Children().Append(elementToAppend);

        if (heightType == rtom::HeightType::Stretch)
        {
            if (auto wholeItemsPanel = PeekInnards<WholeItemsPanel>(panel))
            {
                wholeItemsPanel->AddElementToStretchablesList(elementToAppend);
            }
        }
    }

    template<typename T> void SetToggleValue(_In_ T* item, boolean isChecked)
    {
        ComPtr<IPropertyValueStatics> propertyValueStatics;
        ABI::Windows::Foundation::GetActivationFactory(HStringReference(RuntimeClass_Windows_Foundation_PropertyValue).Get(),
                                                       &propertyValueStatics);

        ComPtr<IPropertyValue> propertyValue;
        propertyValueStatics->CreateBoolean(isChecked, &propertyValue);

        ComPtr<ABI::Windows::Foundation::IReference<bool>> boolProperty;
        propertyValue.As(&boolProperty);

        ComPtr<T> localItem(item);
        ComPtr<IToggleButton> toggleButton;
        THROW_IF_FAILED(localItem.As(&toggleButton));
        THROW_IF_FAILED(toggleButton->put_IsChecked(boolProperty.Get()));
    }

    template<typename T> void GetToggleValue(_In_ T* item, _Out_ boolean* isChecked)
    {
        ComPtr<T> localItem(item);
        ComPtr<IToggleButton> toggleButton;
        THROW_IF_FAILED(localItem.As(&toggleButton));

        ComPtr<IReference<bool>> isCheckedReference;
        THROW_IF_FAILED(toggleButton->get_IsChecked(&isCheckedReference));

        if (isCheckedReference != nullptr)
        {
            THROW_IF_FAILED(isCheckedReference->get_Value(isChecked));
        }
        else
        {
            *isChecked = false;
        }
    }

    template<typename T> bool GetToggleValue(T item)
    {
        /* ComPtr<T> localItem(item);
         ComPtr<IToggleButton> toggleButton;
         THROW_IF_FAILED(localItem.As(&toggleButton));*/
        static_assert(std::is_base_of<winrt::Windows::UI::Xaml::Controls::ToggleButton>, T > ::value, "T must inherit from ToggleButton");
        auto toggleButton = item.as<winrt::Windows::UI::Xaml::Controls::ToggleButton>(); // TODO: I don't think we need this cast, all toggleButton have isChecked() exposed, right?

        auto isCheckedRef = toggleButton.IsChecked();

        if (isCheckedRef != nullptr)
        {
            return isCheckedRef.Value();
        }
        else
        {
            return false;
        }

        /*ComPtr<IReference<bool>> isCheckedReference;
        THROW_IF_FAILED(toggleButton->get_IsChecked(&isCheckedReference));*/
    }

    template<typename T> void SetContent(T const& item, winrt::hstring const& contentString)
    {
        // TODO: Do I need this here? should it be simply ContentControl? that should be enough, right?
        static_assert(std::is_base_of<winrt::Windows::UI::Xaml::Controls::ToggleButton>, T > ::value, "T must inherit from ContenControl");
        SetContent(item, contentString, false);
    }

    template<typename T> void SetContent(T const& item, winrt::hstring const& contentString, bool wrap)
    {
        static_assert(std::is_base_of<winrt::Windows::UI::Xaml::Controls::ToggleButton>, T > ::value, "T must inherit from ContenControl");
        rtxaml::Controls::TextBlock content{};
        content.Text(contentString);

        if (wrap)
        {
            content.TextWrapping(rtxaml::TextWrapping::WrapWholeWords) :
        }
        item.Content(content);
    }

    // template<typename T> void SetContent(_In_ T* item, _In_ HSTRING contentString, boolean wrap)
    //{
    //    ComPtr<T> localItem(item);
    //    ComPtr<IContentControl> contentControl;
    //    THROW_IF_FAILED(localItem.As(&contentControl));

    //    ComPtr<ITextBlock> content =
    //        XamlHelpers::CreateABIClass<ITextBlock>(HStringReference(RuntimeClass_Windows_UI_Xaml_Controls_TextBlock));
    //    THROW_IF_FAILED(content->put_Text(contentString));

    //    if (wrap)
    //    {
    //        THROW_IF_FAILED(content->put_TextWrapping(TextWrapping::TextWrapping_WrapWholeWords));
    //    }
    //    THROW_IF_FAILED(contentControl->put_Content(content.Get()));
    //}

    // template<typename T> void SetContent(T* item, HSTRING contentString) { SetContent(item, contentString, false); }

    template<typename T>
    void AddRow(_In_ T* item, _In_ ABI::Windows::UI::Xaml::Controls::IGrid* grid, ABI::Windows::UI::Xaml::GridLength rowHeight)
    {
        ComPtr<ABI::Windows::UI::Xaml::Controls::IGrid> localGrid(grid);

        ComPtr<IVector<RowDefinition*>> rowDefinitions;
        THROW_IF_FAILED(localGrid->get_RowDefinitions(&rowDefinitions));

        unsigned int rowIndex;
        THROW_IF_FAILED(rowDefinitions->get_Size(&rowIndex));
        ComPtr<IGridStatics> gridStatics;
        THROW_IF_FAILED(GetActivationFactory(HStringReference(RuntimeClass_Windows_UI_Xaml_Controls_Grid).Get(), &gridStatics));
        Microsoft::WRL::ComPtr<T> localItem(item);
        ComPtr<IFrameworkElement> localItemAsFrameworkElement;
        THROW_IF_FAILED(localItem.As(&localItemAsFrameworkElement));
        gridStatics->SetRow(localItemAsFrameworkElement.Get(), rowIndex);

        ComPtr<IRowDefinition> rowDefinition =
            XamlHelpers::CreateABIClass<IRowDefinition>(HStringReference(RuntimeClass_Windows_UI_Xaml_Controls_RowDefinition));
        THROW_IF_FAILED(rowDefinition->put_Height(rowHeight));
        THROW_IF_FAILED(rowDefinitions->Append(rowDefinition.Get()));

        ComPtr<ABI::Windows::UI::Xaml::Controls::IPanel> localPanel;
        THROW_IF_FAILED(localGrid.As(&localPanel));
        XamlHelpers::AppendXamlElementToPanel(item, localPanel.Get());
    }

    HRESULT HandleStylingAndPadding(_In_ ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveContainerBase* adaptiveContainer,
                                    _In_ ABI::Windows::UI::Xaml::Controls::IBorder* containerBorder,
                                    _In_ ABI::AdaptiveCards::Rendering::WinUI3::IAdaptiveRenderContext* renderContext,
                                    _In_ ABI::AdaptiveCards::Rendering::WinUI3::IAdaptiveRenderArgs* renderArgs,
                                    _Out_ ABI::AdaptiveCards::ObjectModel::WinUI3::ContainerStyle* containerStyle);

    bool SupportsInteractivity(_In_ ABI::AdaptiveCards::Rendering::WinUI3::IAdaptiveHostConfig* hostConfig);

    bool SupportsInteractivity(rtrender::AdaptiveHostConfig const& hostConfig);

    template<typename T>
    void SetVerticalContentAlignmentToChildren(_In_ T* container, _In_ ABI::AdaptiveCards::ObjectModel::WinUI3::VerticalContentAlignment verticalContentAlignment)
    {
        ComPtr<T> localContainer(container);
        ComPtr<IWholeItemsPanel> containerAsPanel;
        THROW_IF_FAILED(localContainer.As(&containerAsPanel));

        ComPtr<WholeItemsPanel> panel = PeekInnards<WholeItemsPanel>(containerAsPanel);
        panel->SetVerticalContentAlignment(verticalContentAlignment);
    }

    template<typename T>
    void SetVerticalContentAlignmentToChildren(T container, rtom::VerticalContentAlignment verticalContentAlignment)
    {
        winrt::com_ptr<T> localContainer = winrt::make_self<T>(container);

        if (const winrt::com_ptr<rtrender::implementation::WholeItemsPanel> containerAsPanel =
                localContainer.try_as<rtrender::implementation::WholeItemsPanel>())
        {
            auto panel = *containerAsPanel;
            panel.VerticalAlignment(verticalContentAlignment);
        }
    }

    HRESULT RenderInputLabel(_In_ ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveInputElement* adaptiveInputElement,
                             _In_ ABI::AdaptiveCards::Rendering::WinUI3::IAdaptiveRenderContext* renderContext,
                             _In_ ABI::AdaptiveCards::Rendering::WinUI3::IAdaptiveRenderArgs* renderArgs,
                             _COM_Outptr_ ABI::Windows::UI::Xaml::IUIElement** labelControl);

    HRESULT RenderInputErrorMessage(ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveInputElement* adaptiveInputElement,
                                    ABI::AdaptiveCards::Rendering::WinUI3::IAdaptiveRenderContext* renderContext,
                                    ABI::Windows::UI::Xaml::IUIElement** errorMessageControl);

    HRESULT CreateValidationBorder(ABI::Windows::UI::Xaml::IUIElement* childElement,
                                   ABI::AdaptiveCards::Rendering::WinUI3::IAdaptiveRenderContext* renderContext,
                                   ABI::Windows::UI::Xaml::Controls::IBorder** elementWithBorder);

    winrt::Windows::UI::Xaml::Controls::Border
    CreateValidationBorder(winrt::Windows::UI::Xaml::UIElement const& childElement,
                           winrt::AdaptiveCards::Rendering::WinUI3::AdaptiveRenderContext const& renderContext);

    HRESULT HandleLabelAndErrorMessage(_In_ ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveInputElement* adaptiveInput,
                                       _In_ ABI::AdaptiveCards::Rendering::WinUI3::IAdaptiveRenderContext* renderContext,
                                       _In_ ABI::AdaptiveCards::Rendering::WinUI3::IAdaptiveRenderArgs* renderArgs,
                                       _Out_ ABI::Windows::UI::Xaml::IUIElement** inputLayout);

    HRESULT HandleInputLayoutAndValidation(ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveInputElement* adaptiveInput,
                                           ABI::Windows::UI::Xaml::IUIElement* inputUIElement,
                                           boolean hasTypeSpecificValidation,
                                           ABI::AdaptiveCards::Rendering::WinUI3::IAdaptiveRenderContext* renderContext,
                                           ABI::Windows::UI::Xaml::IUIElement** inputLayout,
                                           ABI::Windows::UI::Xaml::Controls::IBorder** validationBorderOut);

    void HandleInputLayoutAndValidation(winrt::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveInputElement const& adaptiveInput,
                                        winrt::Windows::UI::Xaml::UIElement const& inputUIElement,
                                        bool hasTypeSpecificValidation,
                                        winrt::AdaptiveCards::Rendering::WinUI3::AdaptiveRenderContext const& renderContext,
                                        winrt::Windows::UI::Xaml::UIElement& inputLayout,
                                        winrt::Windows::UI::Xaml::Controls::Border& validationBorderOut);

    template<typename TXamlControl>
    HRESULT SetXamlHeaderFromLabel(_In_ ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveInputElement* adaptiveInputElement,
                                   _In_ ABI::AdaptiveCards::Rendering::WinUI3::IAdaptiveRenderContext* renderContext,
                                   _In_ ABI::AdaptiveCards::Rendering::WinUI3::IAdaptiveRenderArgs* renderArgs,
                                   _In_ TXamlControl* xamlControl)
    {
        ComPtr<IUIElement> labelControl;
        RETURN_IF_FAILED(RenderInputLabel(adaptiveInputElement, renderContext, renderArgs, &labelControl));

        if (labelControl != nullptr)
        {
            ComPtr<IInspectable> labelControlAsInspectable;
            RETURN_IF_FAILED(labelControl.As(&labelControlAsInspectable));
            RETURN_IF_FAILED(xamlControl->put_Header(labelControlAsInspectable.Get()));
        }

        return S_OK;
    }

    HRESULT AddHandledTappedEvent(_In_ ABI::Windows::UI::Xaml::IUIElement* uiElement);
    void AddHandledTappedEvent(winrt::Windows::UI::Xaml::UIElement const& uiElement);

    void ApplyBackgroundToRoot(_In_ ABI::Windows::UI::Xaml::Controls::IPanel* rootPanel,
                               _In_ ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveBackgroundImage* backgroundImage,
                               _In_ ABI::AdaptiveCards::Rendering::WinUI3::IAdaptiveRenderContext* renderContext,
                               _In_ ABI::AdaptiveCards::Rendering::WinUI3::IAdaptiveRenderArgs* renderArgs);

    void ApplyBackgroundToRoot(rtxaml::Controls::Panel const& rootPanel,
                               rtom::AdaptiveBackgroundImage const& backgroundImage,
                               rtrender::AdaptiveRenderContext const& renderContext,
                               rtrender::AdaptiveRenderArgs const& renderArgs);

    HRESULT AddRenderedControl(_In_ ABI::Windows::UI::Xaml::IUIElement* newControl,
                               _In_ ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement* element,
                               _In_ ABI::Windows::UI::Xaml::Controls::IPanel* parentPanel,
                               _In_ ABI::Windows::UI::Xaml::IUIElement* separator,
                               _In_ ABI::Windows::UI::Xaml::Controls::IColumnDefinition* columnDefinition,
                               std::function<void(ABI::Windows::UI::Xaml::IUIElement* child)> childCreatedCallback);

    HRESULT RenderFallback(_In_ ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement* currentElement,
                           _In_ ABI::AdaptiveCards::Rendering::WinUI3::IAdaptiveRenderContext* renderContext,
                           _In_ ABI::AdaptiveCards::Rendering::WinUI3::IAdaptiveRenderArgs* renderArgs,
                           _COM_Outptr_ ABI::Windows::UI::Xaml::IUIElement** result,
                           _COM_Outptr_ ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement** renderedElement);

    void RenderFallback(rtom::IAdaptiveCardElement const& currentElement,
                        rtrender::AdaptiveRenderContext const& renderContext,
                        rtrender::AdaptiveRenderArgs const& renderArgs,
                        winrt::com_ptr<rtxaml::UIElement> result,
                        winrt::com_ptr<rtom::IAdaptiveCardElement> renderedElement);

    void GetSeparationConfigForElement(_In_ ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement* element,
                                       _In_ ABI::AdaptiveCards::Rendering::WinUI3::IAdaptiveHostConfig* hostConfig,
                                       _Out_ UINT* spacing,
                                       _Out_ UINT* separatorThickness,
                                       _Out_ ABI::Windows::UI::Color* separatorColor,
                                       _Out_ bool* needsSeparator);

    inline HRESULT WarnFallbackString(_In_ ABI::AdaptiveCards::Rendering::WinUI3::IAdaptiveRenderContext* renderContext,
                                      const std::string& warning)
    {
        HString warningMsg;
        RETURN_IF_FAILED(UTF8ToHString(warning, warningMsg.GetAddressOf()));

        RETURN_IF_FAILED(renderContext->AddWarning(ABI::AdaptiveCards::ObjectModel::WinUI3::WarningStatusCode::PerformingFallback,
                                                   warningMsg.Get()));
        return S_OK;
    }

    inline void WarnFallbackString(rtrender::AdaptiveRenderContext const& renderContext, winrt::hstring const& warning)
    {
        renderContext.AddWarning(rtom::WarningStatusCode::PerformingFallback, warning);
    }

    inline HRESULT WarnForFallbackContentElement(_In_ ABI::AdaptiveCards::Rendering::WinUI3::IAdaptiveRenderContext* renderContext,
                                                 _In_ HSTRING parentElementType,
                                                 _In_ HSTRING fallbackElementType)
    try
    {
        std::string warning = "Performing fallback for element of type \"";
        warning.append(HStringToUTF8(parentElementType));
        warning.append("\" (fallback element type \"");
        warning.append(HStringToUTF8(fallbackElementType));
        warning.append("\")");

        return WarnFallbackString(renderContext, warning);
    }
    CATCH_RETURN();

    inline HRESULT WarnForFallbackDrop(_In_ ABI::AdaptiveCards::Rendering::WinUI3::IAdaptiveRenderContext* renderContext,
                                       _In_ HSTRING elementType)
    try
    {
        std::string warning = "Dropping element of type \"";
        warning.append(HStringToUTF8(elementType));
        warning.append("\" for fallback");

        return WarnFallbackString(renderContext, warning);
    }
    CATCH_RETURN();

    inline void WarnForFallbackContentElement(rtrender::AdaptiveRenderContext const& renderContext,
                                              winrt::hstring const& parentElementType,
                                              winrt::hstring const& fallbackElementType)
    {
        auto warning = L"Performing fallback for element of type \"" + parentElementType +
            L"\" (fallback element type \"" + fallbackElementType + L"\")";
        WarnFallbackString(renderContext, warning);
    }

    inline void WarnForFallbackDrop(rtrender::AdaptiveRenderContext const& renderContext, winrt::hstring const& elementType)
    {
        WarnFallbackString(renderContext, L"Dropping element of type \"" + elementType + L"\" for fallback");
    }

    void AddSeparatorIfNeeded(int& currentElement,
                              _In_ ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement* element,
                              _In_ ABI::AdaptiveCards::Rendering::WinUI3::IAdaptiveHostConfig* hostConfig,
                              _In_ ABI::AdaptiveCards::Rendering::WinUI3::IAdaptiveRenderContext* renderContext,
                              _In_ ABI::Windows::UI::Xaml::Controls::IPanel* parentPanel,
                              _Outptr_ ABI::Windows::UI::Xaml::IUIElement** addedSeparator);

    HRESULT SetAutoImageSize(_In_ ABI::Windows::UI::Xaml::IFrameworkElement* imageControl,
                             _In_ IInspectable* parentElement,
                             _In_ ABI::Windows::UI::Xaml::Media::Imaging::IBitmapSource* imageSource,
                             bool setVisible);

    HRESULT ApplyMarginToXamlElement(_In_ ABI::AdaptiveCards::Rendering::WinUI3::IAdaptiveHostConfig* hostConfig,
                                     _In_ ABI::Windows::UI::Xaml::IFrameworkElement* element) noexcept;
}
