// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
#include "pch.h"

#include "AdaptiveActionParserRegistration.h"
#include "AdaptiveActionSet.h"
#include "AdaptiveColumn.h"
#include "AdaptiveColumnSet.h"
#include "AdaptiveContainer.h"
#include "AdaptiveChoiceInput.h"
#include "AdaptiveChoiceSetInput.h"
#include "AdaptiveDateInput.h"
#include "AdaptiveElementParserRegistration.h"
#include "AdaptiveExecuteAction.h"
#include "AdaptiveFact.h"
#include "AdaptiveFactSet.h"
#include "AdaptiveImage.h"
#include "AdaptiveImageSet.h"
#include "AdaptiveMedia.h"
#include "AdaptiveMediaSource.h"
#include "AdaptiveNumberInput.h"
#include "AdaptiveOpenUrlAction.h"
#include "AdaptiveRequirement.h"
#include "AdaptiveRichTextBlock.h"
#include "AdaptiveShowCardAction.h"
#include "AdaptiveSubmitAction.h"
#include "AdaptiveTable.h"
#include "AdaptiveTextBlock.h"
#include "AdaptiveTextInput.h"
#include "AdaptiveTextRun.h"
#include "AdaptiveTimeInput.h"
#include "AdaptiveToggleInput.h"
#include "AdaptiveToggleVisibilityTarget.h"
#include "AdaptiveToggleVisibilityAction.h"
#include "AdaptiveUnsupportedAction.h"
#include "AdaptiveUnsupportedElement.h"
#include "AdaptiveWarning.h"
#include "CustomActionWrapper.h"
#include "CustomElementWrapper.h"
#include "winrt/Windows.Foundation.Collections.h"

using namespace AdaptiveCards;
using namespace Microsoft::WRL;
using namespace ABI::Windows::Data::Json;
using namespace Microsoft::WRL;
using namespace AdaptiveCards::ObjectModel::WinUI3;
using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::Foundation::Collections;

std::string WStringToString(std::wstring_view in)
{
    const int length_in = static_cast<int>(in.length());

    if (length_in > 0)
    {
        const int length_out = ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, in.data(), length_in, NULL, 0, NULL, NULL);

        if (length_out > 0)
        {
            std::string out(length_out, '\0');

            const int length_written =
                ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, in.data(), length_in, out.data(), length_out, NULL, NULL);

            if (length_written == length_out)
            {
                return out;
            }
        }

        throw bad_string_conversion();
    }

    return {};
}

std::wstring StringToWString(std::string_view in)
{
    const int length_in = static_cast<int>(in.length());

    if (length_in > 0)
    {
        const int length_out = ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, in.data(), length_in, NULL, 0);

        if (length_out > 0)
        {
            std::wstring out(length_out, L'\0');

            const int length_written =
                ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, in.data(), length_in, out.data(), length_out);

            if (length_written == length_out)
            {
                return out;
            }
        }

        throw bad_string_conversion();
    }

    return {};
}

HRESULT UTF8ToHString(std::string_view in, _Outptr_ HSTRING* out) noexcept
try
{
    if (out == nullptr)
    {
        return E_INVALIDARG;
    }
    else
    {
        std::wstring wide = StringToWString(in);
        return WindowsCreateString(wide.c_str(), static_cast<UINT32>(wide.length()), out);
    }
}
CATCH_RETURN();

winrt::hstring UTF8ToHString(std::string_view in)
{
    return winrt::hstring{StringToWString(in)};
}

HRESULT HStringToUTF8(HSTRING in, std::string& out) noexcept
try
{
    UINT32 length = 0U;
    const auto* ptr_wide = WindowsGetStringRawBuffer(in, &length);
    out = WStringToString(std::wstring_view(ptr_wide, length));

    return S_OK;
}
CATCH_RETURN();

std::string HStringToUTF8(HSTRING in)
{
    std::string typeAsKey;
    if (SUCCEEDED(HStringToUTF8(in, typeAsKey)))
    {
        return typeAsKey;
    }

    return {};
}

std::string HStringToUTF8(winrt::hstring const& in)
{
    return WStringToString(static_cast<std::wstring_view>(in));
}

template<typename TSharedBaseType, typename TAdaptiveBaseType, typename TAdaptiveType>
std::shared_ptr<AdaptiveCards::BaseCardElement> GetSharedModel(_In_ TAdaptiveBaseType* item)
{
    if (auto adaptiveElement = PeekInnards<TAdaptiveType>(item))
    {
        return adaptiveElement->GetSharedModel();
    }

    return nullptr;
}

HRESULT GenerateSharedElement(_In_ ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement* item,
                              std::shared_ptr<AdaptiveCards::BaseCardElement>& baseCardElement)
{
    ABI::AdaptiveCards::ObjectModel::WinUI3::ElementType elementType;
    RETURN_IF_FAILED(item->get_ElementType(&elementType));

    switch (elementType)
    {
    case ABI::AdaptiveCards::ObjectModel::WinUI3::ElementType::ActionSet:
        baseCardElement =
            GetSharedModel<AdaptiveCards::BaseCardElement, ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement, AdaptiveCards::ObjectModel::WinUI3::AdaptiveActionSet>(
                item);
        break;
    case ABI::AdaptiveCards::ObjectModel::WinUI3::ElementType::ChoiceSetInput:
        baseCardElement =
            GetSharedModel<AdaptiveCards::BaseCardElement, ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement, AdaptiveCards::ObjectModel::WinUI3::AdaptiveChoiceSetInput>(
                item);
        break;
    case ABI::AdaptiveCards::ObjectModel::WinUI3::ElementType::Column:
        baseCardElement =
            GetSharedModel<AdaptiveCards::BaseCardElement, ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement, AdaptiveCards::ObjectModel::WinUI3::AdaptiveColumn>(
                item);
        break;
    case ABI::AdaptiveCards::ObjectModel::WinUI3::ElementType::ColumnSet:
        baseCardElement =
            GetSharedModel<AdaptiveCards::BaseCardElement, ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement, AdaptiveCards::ObjectModel::WinUI3::AdaptiveColumnSet>(
                item);
        break;
    case ABI::AdaptiveCards::ObjectModel::WinUI3::ElementType::Container:
        baseCardElement =
            GetSharedModel<AdaptiveCards::BaseCardElement, ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement, AdaptiveCards::ObjectModel::WinUI3::AdaptiveContainer>(
                item);
        break;
    case ABI::AdaptiveCards::ObjectModel::WinUI3::ElementType::DateInput:
        baseCardElement =
            GetSharedModel<AdaptiveCards::BaseCardElement, ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement, AdaptiveCards::ObjectModel::WinUI3::AdaptiveDateInput>(
                item);
        break;
    case ABI::AdaptiveCards::ObjectModel::WinUI3::ElementType::FactSet:
        baseCardElement =
            GetSharedModel<AdaptiveCards::BaseCardElement, ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement, AdaptiveCards::ObjectModel::WinUI3::AdaptiveFactSet>(
                item);
        break;
    case ABI::AdaptiveCards::ObjectModel::WinUI3::ElementType::Image:
        baseCardElement =
            GetSharedModel<AdaptiveCards::BaseCardElement, ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement, AdaptiveCards::ObjectModel::WinUI3::AdaptiveImage>(
                item);
        break;
    case ABI::AdaptiveCards::ObjectModel::WinUI3::ElementType::ImageSet:
        baseCardElement =
            GetSharedModel<AdaptiveCards::BaseCardElement, ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement, AdaptiveCards::ObjectModel::WinUI3::AdaptiveImageSet>(
                item);
        break;
    case ABI::AdaptiveCards::ObjectModel::WinUI3::ElementType::NumberInput:
        baseCardElement =
            GetSharedModel<AdaptiveCards::BaseCardElement, ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement, AdaptiveCards::ObjectModel::WinUI3::AdaptiveNumberInput>(
                item);
        break;
    case ABI::AdaptiveCards::ObjectModel::WinUI3::ElementType::Media:
        baseCardElement =
            GetSharedModel<AdaptiveCards::BaseCardElement, ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement, AdaptiveCards::ObjectModel::WinUI3::AdaptiveMedia>(
                item);
        break;
    case ABI::AdaptiveCards::ObjectModel::WinUI3::ElementType::RichTextBlock:
        baseCardElement =
            GetSharedModel<AdaptiveCards::BaseCardElement, ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement, AdaptiveCards::ObjectModel::WinUI3::AdaptiveRichTextBlock>(
                item);
        break;
    case ABI::AdaptiveCards::ObjectModel::WinUI3::ElementType::TextBlock:
        baseCardElement =
            GetSharedModel<AdaptiveCards::BaseCardElement, ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement, AdaptiveCards::ObjectModel::WinUI3::AdaptiveTextBlock>(
                item);
        break;
    case ABI::AdaptiveCards::ObjectModel::WinUI3::ElementType::TextInput:
        baseCardElement =
            GetSharedModel<AdaptiveCards::BaseCardElement, ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement, AdaptiveCards::ObjectModel::WinUI3::AdaptiveTextInput>(
                item);
        break;
    case ABI::AdaptiveCards::ObjectModel::WinUI3::ElementType::TimeInput:
        baseCardElement =
            GetSharedModel<AdaptiveCards::BaseCardElement, ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement, AdaptiveCards::ObjectModel::WinUI3::AdaptiveTimeInput>(
                item);
        break;
    case ABI::AdaptiveCards::ObjectModel::WinUI3::ElementType::ToggleInput:
        baseCardElement =
            GetSharedModel<AdaptiveCards::BaseCardElement, ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement, AdaptiveCards::ObjectModel::WinUI3::AdaptiveToggleInput>(
                item);
        break;
    case ABI::AdaptiveCards::ObjectModel::WinUI3::ElementType::Table:
        baseCardElement =
            GetSharedModel<AdaptiveCards::BaseCardElement, ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement, AdaptiveCards::ObjectModel::WinUI3::AdaptiveTable>(
                item);
        break;
    case ABI::AdaptiveCards::ObjectModel::WinUI3::ElementType::Unsupported:
        baseCardElement =
            GetSharedModel<AdaptiveCards::BaseCardElement, ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement, AdaptiveCards::ObjectModel::WinUI3::AdaptiveUnsupportedElement>(
                item);
        break;
    case ABI::AdaptiveCards::ObjectModel::WinUI3::ElementType::Custom:
        baseCardElement = std::make_shared<CustomElementWrapper>(copy_from_abi<winrt::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement>(item));
        break;
    }

    if (baseCardElement == nullptr)
    {
        return E_INVALIDARG;
    }

    return S_OK;
}

std::shared_ptr<AdaptiveCards::BaseCardElement> GenerateSharedElement(winrt::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement const& item)
{
    std::shared_ptr<AdaptiveCards::BaseCardElement> returned;
    THROW_IF_FAILED(GenerateSharedElement(item.as<ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement>().get(), returned));
    return returned;
}

HRESULT GenerateSharedElements(_In_ ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement*>* items,
                               std::vector<std::shared_ptr<AdaptiveCards::BaseCardElement>>& containedElements)
{
    containedElements.clear();

    IterateOverVector<ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement>(
        items, [&](ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement* item) {
            std::shared_ptr<AdaptiveCards::BaseCardElement> baseCardElement;
            RETURN_IF_FAILED(GenerateSharedElement(item, baseCardElement));
            containedElements.push_back(std::move(baseCardElement));

            return S_OK;
        });

    return S_OK;
}

std::vector<std::shared_ptr<AdaptiveCards::BaseCardElement>> GenerateSharedElements(
    winrt::Windows::Foundation::Collections::IVector<winrt::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement> const& items)
{
    std::vector<std::shared_ptr<AdaptiveCards::BaseCardElement>> containedElements;
    for (auto&& item : items)
    {
        containedElements.emplace_back(GenerateSharedElement(item));
    }
    return containedElements;
}

HRESULT GenerateSharedAction(_In_ ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveActionElement* action,
                             std::shared_ptr<AdaptiveCards::BaseActionElement>& sharedAction)
{
    ABI::AdaptiveCards::ObjectModel::WinUI3::ActionType actionType;
    RETURN_IF_FAILED(action->get_ActionType(&actionType));

    switch (actionType)
    {
    case ABI::AdaptiveCards::ObjectModel::WinUI3::ActionType::OpenUrl:
        sharedAction =
            GetSharedModel<AdaptiveCards::BaseActionElement, ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveActionElement, AdaptiveCards::ObjectModel::WinUI3::AdaptiveOpenUrlAction>(
                action);
        break;
    case ABI::AdaptiveCards::ObjectModel::WinUI3::ActionType::ShowCard:
        sharedAction =
            GetSharedModel<AdaptiveCards::BaseActionElement, ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveActionElement, AdaptiveCards::ObjectModel::WinUI3::AdaptiveShowCardAction>(
                action);
        break;
    case ABI::AdaptiveCards::ObjectModel::WinUI3::ActionType::Submit:
        sharedAction =
            GetSharedModel<AdaptiveCards::BaseActionElement, ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveActionElement, AdaptiveCards::ObjectModel::WinUI3::AdaptiveSubmitAction>(
                action);
        break;
    case ABI::AdaptiveCards::ObjectModel::WinUI3::ActionType::ToggleVisibility:
        sharedAction =
            GetSharedModel<AdaptiveCards::BaseActionElement, ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveActionElement, winrt::AdaptiveCards::ObjectModel::WinUI3::implementation::AdaptiveToggleVisibilityAction>(
                action);
        break;
    case ABI::AdaptiveCards::ObjectModel::WinUI3::ActionType::Execute:
        sharedAction =
            GetSharedModel<AdaptiveCards::BaseActionElement, ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveActionElement, AdaptiveCards::ObjectModel::WinUI3::AdaptiveExecuteAction>(
                action);
        break;
    case ABI::AdaptiveCards::ObjectModel::WinUI3::ActionType::Custom:
        sharedAction = std::make_shared<CustomActionWrapper>(copy_from_abi<winrt::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveActionElement>(action));
        break;
    case ABI::AdaptiveCards::ObjectModel::WinUI3::ActionType::Unsupported:
        sharedAction =
            GetSharedModel<AdaptiveCards::BaseActionElement, ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveActionElement, AdaptiveCards::ObjectModel::WinUI3::AdaptiveUnsupportedAction>(
                action);
    }

    return S_OK;
}

std::shared_ptr<AdaptiveCards::BaseActionElement>
GenerateSharedAction(_In_ winrt::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveActionElement const& action)
{
    std::shared_ptr<AdaptiveCards::BaseActionElement> result;
    THROW_IF_FAILED(GenerateSharedAction(action.as<ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveActionElement>().get(), result));
    return result;
}

HRESULT GenerateSharedActions(_In_ ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveActionElement*>* actions,
                              std::vector<std::shared_ptr<AdaptiveCards::BaseActionElement>>& containedElements)
{
    containedElements.clear();

    IterateOverVector<ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveActionElement>(
        actions, [&](ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveActionElement* action) {
            std::shared_ptr<AdaptiveCards::BaseActionElement> baseActionElement;
            GenerateSharedAction(action, baseActionElement);
            containedElements.push_back(baseActionElement);
            return S_OK;
        });

    return S_OK;
}

std::vector<std::shared_ptr<AdaptiveCards::BaseActionElement>> GenerateSharedActions(
    _In_ winrt::Windows::Foundation::Collections::IVector<winrt::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveActionElement> const& actions)
{
    std::vector<std::shared_ptr<AdaptiveCards::BaseActionElement>> containedElements;
    for (auto&& action : actions)
    {
        containedElements.emplace_back(GenerateSharedAction(action));
    }
    return containedElements;
}


std::unordered_map<std::string, AdaptiveCards::SemanticVersion> GenerateSharedRequirements(
    winrt::Windows::Foundation::Collections::IVector<winrt::AdaptiveCards::ObjectModel::WinUI3::AdaptiveRequirement> const& adaptiveRequirements)
{
    std::unordered_map<std::string, AdaptiveCards::SemanticVersion> sharedRequirements;

    for (auto&& requirement : adaptiveRequirements)
    {
        auto name = HStringToUTF8(requirement.Name());
        auto version = HStringToUTF8(requirement.Version());

        if (version == "*")
        {
            version = "0";
        }

        sharedRequirements.emplace(std::move(name), std::move(version));
    }

    return sharedRequirements;
}

HRESULT GenerateSharedInlines(ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveInline*>* inlines,
                              std::vector<std::shared_ptr<AdaptiveCards::Inline>>& containedElements)
{
    containedElements.clear();

    IterateOverVector<ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveInline, ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveInline>(
        inlines, [&](ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveInline* thisInline) {
            // We only support text runs for now
            ComPtr<AdaptiveCards::ObjectModel::WinUI3::AdaptiveTextRun> adaptiveTextRun =
                PeekInnards<AdaptiveCards::ObjectModel::WinUI3::AdaptiveTextRun>(thisInline);
            if (adaptiveTextRun == nullptr)
            {
                return E_INVALIDARG;
            }

            std::shared_ptr<AdaptiveCards::TextRun> sharedTextRun;
            RETURN_IF_FAILED(adaptiveTextRun->GetSharedModel(sharedTextRun));
            containedElements.push_back(std::AdaptivePointerCast<AdaptiveCards::Inline>(sharedTextRun));
            return S_OK;
        });

    return S_OK;
}

winrt::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement
GenerateElementProjection(const std::shared_ptr<AdaptiveCards::BaseCardElement>& baseElement)
{
    ComPtr<ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement> element;
    THROW_IF_FAILED(GenerateElementProjection(baseElement, &element));
    return copy_from_abi<winrt::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement>(element.Get());
}

HRESULT GenerateElementProjection(_In_ const std::shared_ptr<AdaptiveCards::BaseCardElement>& baseElement,
                                  _COM_Outptr_ ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement** projectedElement) noexcept
try
{
    *projectedElement = nullptr;
    switch (baseElement->GetElementType())
    {
    case CardElementType::TextBlock:
        RETURN_IF_FAILED(MakeAndInitialize<::AdaptiveCards::ObjectModel::WinUI3::AdaptiveTextBlock>(
            projectedElement, std::AdaptivePointerCast<AdaptiveCards::TextBlock>(baseElement)));
        break;
    case CardElementType::Image:
        RETURN_IF_FAILED(MakeAndInitialize<::AdaptiveCards::ObjectModel::WinUI3::AdaptiveImage>(
            projectedElement, std::AdaptivePointerCast<AdaptiveCards::Image>(baseElement)));
        break;
    case CardElementType::Container:
        RETURN_IF_FAILED(MakeAndInitialize<::AdaptiveCards::ObjectModel::WinUI3::AdaptiveContainer>(
            projectedElement, std::AdaptivePointerCast<AdaptiveCards::Container>(baseElement)));
        break;
    case CardElementType::ColumnSet:
        RETURN_IF_FAILED(MakeAndInitialize<::AdaptiveCards::ObjectModel::WinUI3::AdaptiveColumnSet>(
            projectedElement, std::AdaptivePointerCast<AdaptiveCards::ColumnSet>(baseElement)));
        break;
    case CardElementType::FactSet:
        RETURN_IF_FAILED(MakeAndInitialize<::AdaptiveCards::ObjectModel::WinUI3::AdaptiveFactSet>(
            projectedElement, std::AdaptivePointerCast<AdaptiveCards::FactSet>(baseElement)));
        break;
    case CardElementType::ImageSet:
        RETURN_IF_FAILED(MakeAndInitialize<::AdaptiveCards::ObjectModel::WinUI3::AdaptiveImageSet>(
            projectedElement, std::AdaptivePointerCast<AdaptiveCards::ImageSet>(baseElement)));
        break;
    case CardElementType::ChoiceSetInput:
        RETURN_IF_FAILED(MakeAndInitialize<::AdaptiveCards::ObjectModel::WinUI3::AdaptiveChoiceSetInput>(
            projectedElement, std::AdaptivePointerCast<AdaptiveCards::ChoiceSetInput>(baseElement)));
        break;
    case CardElementType::DateInput:
        RETURN_IF_FAILED(MakeAndInitialize<::AdaptiveCards::ObjectModel::WinUI3::AdaptiveDateInput>(
            projectedElement, std::AdaptivePointerCast<AdaptiveCards::DateInput>(baseElement)));
        break;
    case CardElementType::Media:
        RETURN_IF_FAILED(MakeAndInitialize<::AdaptiveCards::ObjectModel::WinUI3::AdaptiveMedia>(
            projectedElement, std::AdaptivePointerCast<AdaptiveCards::Media>(baseElement)));
        break;
    case CardElementType::NumberInput:
        RETURN_IF_FAILED(MakeAndInitialize<::AdaptiveCards::ObjectModel::WinUI3::AdaptiveNumberInput>(
            projectedElement, std::AdaptivePointerCast<AdaptiveCards::NumberInput>(baseElement)));
        break;
    case CardElementType::TextInput:
        RETURN_IF_FAILED(MakeAndInitialize<::AdaptiveCards::ObjectModel::WinUI3::AdaptiveTextInput>(
            projectedElement, std::AdaptivePointerCast<AdaptiveCards::TextInput>(baseElement)));
        break;
    case CardElementType::TimeInput:
        RETURN_IF_FAILED(MakeAndInitialize<::AdaptiveCards::ObjectModel::WinUI3::AdaptiveTimeInput>(
            projectedElement, std::AdaptivePointerCast<AdaptiveCards::TimeInput>(baseElement)));
        break;
    case CardElementType::ToggleInput:
        RETURN_IF_FAILED(MakeAndInitialize<::AdaptiveCards::ObjectModel::WinUI3::AdaptiveToggleInput>(
            projectedElement, std::AdaptivePointerCast<AdaptiveCards::ToggleInput>(baseElement)));
        break;
    case CardElementType::ActionSet:
        RETURN_IF_FAILED(MakeAndInitialize<::AdaptiveCards::ObjectModel::WinUI3::AdaptiveActionSet>(
            projectedElement, std::AdaptivePointerCast<AdaptiveCards::ActionSet>(baseElement)));
        break;
    case CardElementType::RichTextBlock:
        RETURN_IF_FAILED(MakeAndInitialize<::AdaptiveCards::ObjectModel::WinUI3::AdaptiveRichTextBlock>(
            projectedElement, std::AdaptivePointerCast<AdaptiveCards::RichTextBlock>(baseElement)));
        break;
    case CardElementType::Column:
        RETURN_IF_FAILED(MakeAndInitialize<::AdaptiveCards::ObjectModel::WinUI3::AdaptiveColumn>(
            projectedElement, std::AdaptivePointerCast<AdaptiveCards::Column>(baseElement)));
        break;
    case CardElementType::Table:
        RETURN_IF_FAILED(MakeAndInitialize<::AdaptiveCards::ObjectModel::WinUI3::AdaptiveTable>(
            projectedElement, std::AdaptivePointerCast<AdaptiveCards::Table>(baseElement)));
        break;
    case CardElementType::Custom:
    {
        auto e = std::AdaptivePointerCast<::AdaptiveCards::ObjectModel::WinUI3::CustomElementWrapper>(baseElement)->GetWrappedElement();
        *projectedElement = e.as<ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement>().detach();
    }
        break;
    case CardElementType::Unknown:
    default:
        RETURN_IF_FAILED(MakeAndInitialize<::AdaptiveCards::ObjectModel::WinUI3::AdaptiveUnsupportedElement>(
            projectedElement, std::AdaptivePointerCast<AdaptiveCards::UnknownElement>(baseElement)));
        break;
    }

    return S_OK;
}
CATCH_RETURN();

HRESULT GenerateContainedElementsProjection(
    const std::vector<std::shared_ptr<AdaptiveCards::BaseCardElement>>& containedElements,
    _In_ ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement*>* projectedParentContainer) noexcept
try
{
    for (auto& containedElement : containedElements)
    {
        ComPtr<ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement> projectedContainedElement;
        GenerateElementProjection(containedElement, &projectedContainedElement);
        if (projectedContainedElement != nullptr)
        {
            RETURN_IF_FAILED(projectedParentContainer->Append(projectedContainedElement.Detach()));
        }
    }
    return S_OK;
}
CATCH_RETURN();

winrt::Windows::Foundation::Collections::IVector<winrt::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement> GenerateContainedElementsProjection(
    const std::vector<std::shared_ptr<AdaptiveCards::BaseCardElement>>& containedElements)
{
    std::vector<winrt::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement> elements;
    for (auto&& containedElement : containedElements)
    {
        elements.emplace_back(GenerateElementProjection(containedElement));
    }
    return winrt::single_threaded_vector<winrt::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement>(std::move(elements));
}

winrt::Windows::Foundation::Collections::IVector<winrt::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveActionElement> GenerateActionsProjection(const std::vector<std::shared_ptr<AdaptiveCards::BaseActionElement>>& containedActions)
{
    std::vector<winrt::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveActionElement> actions;
    for (auto&& containedAction : containedActions)
    {
        actions.emplace_back(GenerateActionProjection(containedAction));
    }
    return winrt::single_threaded_vector<winrt::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveActionElement>(std::move(actions));
}

winrt::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveActionElement GenerateActionProjection(
    const std::shared_ptr<AdaptiveCards::BaseActionElement>& action)
{
    if (!action)
    {
        return nullptr;
    }

    winrt::com_ptr<ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveActionElement> abiElement;

    switch (action->GetElementType())
    {
    case ActionType::OpenUrl:
        abiElement = MakeOrThrow<::AdaptiveCards::ObjectModel::WinUI3::AdaptiveOpenUrlAction>(
            std::AdaptivePointerCast<AdaptiveCards::OpenUrlAction>(action));
        break;
    case ActionType::ShowCard:
        abiElement = MakeOrThrow<::AdaptiveCards::ObjectModel::WinUI3::AdaptiveShowCardAction>(
            std::AdaptivePointerCast<AdaptiveCards::ShowCardAction>(action));
        break;
    case ActionType::Submit:
        abiElement = MakeOrThrow<::AdaptiveCards::ObjectModel::WinUI3::AdaptiveSubmitAction>(
            std::AdaptivePointerCast<AdaptiveCards::SubmitAction>(action));
        break;
    case ActionType::ToggleVisibility:
        return winrt::make<winrt::AdaptiveCards::ObjectModel::WinUI3::implementation::AdaptiveToggleVisibilityAction>(
            std::AdaptivePointerCast<AdaptiveCards::ToggleVisibilityAction>(action));
        break;
    case ActionType::Execute:
        abiElement = MakeOrThrow<::AdaptiveCards::ObjectModel::WinUI3::AdaptiveExecuteAction>(
            std::AdaptivePointerCast<AdaptiveCards::ExecuteAction>(action));
        break;
    case ActionType::Custom:
        abiElement =
            std::AdaptivePointerCast<CustomActionWrapper>(action)->GetWrappedElement().as<ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveActionElement>();
        break;
    case ActionType::UnknownAction:
        abiElement = MakeOrThrow<::AdaptiveCards::ObjectModel::WinUI3::AdaptiveUnsupportedAction>(
            std::AdaptivePointerCast<AdaptiveCards::UnknownAction>(action));
        break;
    default:
        throw winrt::hresult_error(E_UNEXPECTED);
    }

    return copy_from_abi<winrt::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveActionElement>(abiElement.get());
}

HRESULT GenerateInlinesProjection(const std::vector<std::shared_ptr<AdaptiveCards::Inline>>& containedElements,
                                  ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveInline*>* projectedParentContainer) noexcept
try
{
    for (auto& containedElement : containedElements)
    {
        // Only support text runs for now
        if (containedElement->GetInlineType() != InlineElementType::TextRun)
        {
            return E_NOTIMPL;
        }

        ComPtr<ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveInline> projectedContainedElement;
        RETURN_IF_FAILED(MakeAndInitialize<::AdaptiveCards::ObjectModel::WinUI3::AdaptiveTextRun>(
            &projectedContainedElement, std::static_pointer_cast<AdaptiveCards::TextRun>(containedElement)));

        RETURN_IF_FAILED(projectedParentContainer->Append(projectedContainedElement.Detach()));
    }
    return S_OK;
}
CATCH_RETURN();

winrt::Windows::Foundation::Collections::IVector<winrt::AdaptiveCards::ObjectModel::WinUI3::AdaptiveRequirement> GenerateRequirementsProjection(
    const std::unordered_map<std::string, SemanticVersion>& sharedRequirements)
{
    std::vector<winrt::AdaptiveCards::ObjectModel::WinUI3::AdaptiveRequirement> results;
    for (const auto& sharedRequirement : sharedRequirements)
    {
        auto requirement =
            winrt::make_self<winrt::AdaptiveCards::ObjectModel::WinUI3::implementation::AdaptiveRequirement>(sharedRequirement);

        results.emplace_back(*requirement);
    }

    return winrt::single_threaded_vector<winrt::AdaptiveCards::ObjectModel::WinUI3::AdaptiveRequirement>(std::move(results));
}

HRESULT StringToJsonObject(const std::string& inputString, _COM_Outptr_ IJsonObject** result)
{
    HString asHstring;
    RETURN_IF_FAILED(UTF8ToHString(inputString, asHstring.GetAddressOf()));
    return HStringToJsonObject(asHstring.Get(), result);
}

winrt::Windows::Data::Json::JsonObject StringToJsonObject(const std::string& inputString)
{
    return HStringToJsonObject(UTF8ToHString(inputString));
}

HRESULT HStringToJsonObject(const HSTRING& inputHString, _COM_Outptr_ IJsonObject** result)
{
    ComPtr<IJsonObjectStatics> jObjectStatics;
    RETURN_IF_FAILED(GetActivationFactory(HStringReference(RuntimeClass_Windows_Data_Json_JsonObject).Get(), &jObjectStatics));
    ComPtr<IJsonObject> jObject;
    HRESULT hr = jObjectStatics->Parse(inputHString, &jObject);
    if (FAILED(hr))
    {
        RETURN_IF_FAILED(ActivateInstance(HStringReference(RuntimeClass_Windows_Data_Json_JsonObject).Get(), &jObject));
    }
    *result = jObject.Detach();
    return S_OK;
}

winrt::Windows::Data::Json::JsonObject HStringToJsonObject(winrt::hstring const& inputHString)
{
    winrt::Windows::Data::Json::JsonObject result{nullptr};
    if (!winrt::Windows::Data::Json::JsonObject::TryParse(inputHString, result))
    {
        result = winrt::Windows::Data::Json::JsonObject();
    }
    return result;
}

HRESULT JsonObjectToString(_In_ IJsonObject* inputJson, std::string& result)
{
    HString asHstring;
    RETURN_IF_FAILED(JsonObjectToHString(inputJson, asHstring.GetAddressOf()));
    return HStringToUTF8(asHstring.Get(), result);
}

std::string JsonObjectToString(_In_ winrt::Windows::Data::Json::IJsonObject const& inputJson)
{
    return HStringToUTF8(JsonObjectToHString(inputJson));
}

HRESULT JsonObjectToHString(_In_ IJsonObject* inputJson, _Outptr_ HSTRING* result)
{
    if (!inputJson)
    {
        return E_INVALIDARG;
    }
    ComPtr<IJsonObject> localInputJson(inputJson);
    ComPtr<IJsonValue> asJsonValue;
    RETURN_IF_FAILED(localInputJson.As(&asJsonValue));
    return (asJsonValue->Stringify(result));
}

winrt::hstring JsonObjectToHString(_In_ winrt::Windows::Data::Json::IJsonObject const& inputJson)
{
    if (!inputJson)
    {
        throw winrt::hresult_invalid_argument();
    }

    return inputJson.as<winrt::Windows::Data::Json::JsonValue>().Stringify();
}

HRESULT StringToJsonValue(const std::string inputString, _COM_Outptr_ IJsonValue** result)
{
    HString asHstring;
    RETURN_IF_FAILED(UTF8ToHString(inputString, asHstring.GetAddressOf()));
    return HStringToJsonValue(asHstring.Get(), result);
}

HRESULT HStringToJsonValue(const HSTRING& inputHString, _COM_Outptr_ IJsonValue** result)
{
    ComPtr<IJsonValueStatics> jValueStatics;
    RETURN_IF_FAILED(GetActivationFactory(HStringReference(RuntimeClass_Windows_Data_Json_JsonValue).Get(), &jValueStatics));
    ComPtr<IJsonValue> jValue;
    HRESULT hr = jValueStatics->Parse(inputHString, &jValue);
    if (FAILED(hr))
    {
        RETURN_IF_FAILED(ActivateInstance(HStringReference(RuntimeClass_Windows_Data_Json_JsonValue).Get(), &jValue));
    }
    *result = jValue.Detach();
    return S_OK;
}

HRESULT JsonValueToString(_In_ IJsonValue* inputValue, std::string& result)
{
    HString asHstring;
    RETURN_IF_FAILED(JsonValueToHString(inputValue, asHstring.GetAddressOf()));
    return HStringToUTF8(asHstring.Get(), result);
}

HRESULT JsonValueToHString(_In_ IJsonValue* inputJsonValue, _Outptr_ HSTRING* result)
{
    if (!inputJsonValue)
    {
        return E_INVALIDARG;
    }
    ComPtr<IJsonValue> localInputJsonValue(inputJsonValue);
    return (localInputJsonValue->Stringify(result));
}

HRESULT JsonCppToJsonObject(const Json::Value& jsonCppValue, _COM_Outptr_ IJsonObject** result)
{
    std::string jsonString = ParseUtil::JsonToString(jsonCppValue);
    return StringToJsonObject(jsonString, result);
}

winrt::Windows::Data::Json::JsonObject JsonCppToJsonObject(const Json::Value& jsonCppValue)
{
    return StringToJsonObject(ParseUtil::JsonToString(jsonCppValue));
}

HRESULT JsonObjectToJsonCpp(_In_ ABI::Windows::Data::Json::IJsonObject* jsonObject, _Out_ Json::Value* jsonCppValue)
{
    std::string jsonString;
    RETURN_IF_FAILED(JsonObjectToString(jsonObject, jsonString));

    *jsonCppValue = ParseUtil::GetJsonValueFromString(jsonString);

    return S_OK;
}

Json::Value JsonObjectToJsonCpp(_In_ winrt::Windows::Data::Json::IJsonObject const& jsonObject)
{
    return ParseUtil::GetJsonValueFromString(JsonObjectToString(jsonObject));
}

void RemoteResourceElementToRemoteResourceInformationVector(_In_ winrt::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveElementWithRemoteResources const& remoteResourceElement,
                                                            std::vector<RemoteResourceInformation>& resourceUris)
{
    for (auto&& resourceInformation : remoteResourceElement.GetResourceInformation())
    {
        RemoteResourceInformation uriInfo;
        uriInfo.url = HStringToUTF8(resourceInformation.Url());
        uriInfo.mimeType = HStringToUTF8(resourceInformation.MimeType());
        resourceUris.emplace_back(std::move(uriInfo));
    }
}


void SharedWarningsToAdaptiveWarnings(const std::vector<std::shared_ptr<AdaptiveCardParseWarning>>& sharedWarnings, winrt::Windows::Foundation::Collections::IVector<winrt::AdaptiveCards::ObjectModel::WinUI3::AdaptiveWarning> const& toAddTo)
{
    for (const auto& sharedWarning : sharedWarnings)
    {
        auto warning = winrt::make_self<winrt::AdaptiveCards::ObjectModel::WinUI3::implementation::AdaptiveWarning>(
            static_cast<winrt::AdaptiveCards::ObjectModel::WinUI3::WarningStatusCode>(sharedWarning->GetStatusCode()),
            UTF8ToHString(sharedWarning->GetReason()));
        toAddTo.Append(*warning);
    }
}


winrt::Windows::Foundation::Collections::IVector<winrt::AdaptiveCards::ObjectModel::WinUI3::AdaptiveWarning>
SharedWarningsToAdaptiveWarnings(const std::vector<std::shared_ptr<AdaptiveCardParseWarning>>& sharedWarnings)
{
    auto result = winrt::single_threaded_vector<winrt::AdaptiveCards::ObjectModel::WinUI3::AdaptiveWarning>();
    SharedWarningsToAdaptiveWarnings(sharedWarnings, result);
    return result;
}


HRESULT AdaptiveWarningsToSharedWarnings(
    _In_ ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveCards::ObjectModel::WinUI3::AdaptiveWarning*>* adaptiveWarnings,
    std::vector<std::shared_ptr<AdaptiveCardParseWarning>>& sharedWarnings)
{
    ComPtr<ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveCards::ObjectModel::WinUI3::AdaptiveWarning*>> localAdaptiveWarnings{
        adaptiveWarnings};
    ComPtr<IIterable<ABI::AdaptiveCards::ObjectModel::WinUI3::AdaptiveWarning*>> vectorIterable;
    RETURN_IF_FAILED(localAdaptiveWarnings.As<IIterable<ABI::AdaptiveCards::ObjectModel::WinUI3::AdaptiveWarning*>>(&vectorIterable));

    Microsoft::WRL::ComPtr<IIterator<ABI::AdaptiveCards::ObjectModel::WinUI3::AdaptiveWarning*>> vectorIterator;
    HRESULT hr = vectorIterable->First(&vectorIterator);

    boolean hasCurrent;
    THROW_IF_FAILED(vectorIterator->get_HasCurrent(&hasCurrent));

    while (SUCCEEDED(hr) && hasCurrent)
    {
        ComPtr<ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveWarning> adaptiveWarning;
        RETURN_IF_FAILED(vectorIterator->get_Current(&adaptiveWarning));

        HString message;
        RETURN_IF_FAILED(adaptiveWarning->get_Message(message.GetAddressOf()));

        ABI::AdaptiveCards::ObjectModel::WinUI3::WarningStatusCode statusCode;
        RETURN_IF_FAILED(adaptiveWarning->get_StatusCode(&statusCode));

        sharedWarnings.push_back(std::make_shared<AdaptiveCardParseWarning>(static_cast<AdaptiveCards::WarningStatusCode>(statusCode),
                                                                            HStringToUTF8(message.Get())));

        hr = vectorIterator->MoveNext(&hasCurrent);
    }

    return S_OK;
}


void AdaptiveWarningsToSharedWarnings(
    _In_ winrt::Windows::Foundation::Collections::IVector<winrt::AdaptiveCards::ObjectModel::WinUI3::AdaptiveWarning> const& adaptiveWarnings,
    std::vector<std::shared_ptr<AdaptiveCardParseWarning>>& sharedWarnings)
{
    for (auto&& adaptiveWarning : adaptiveWarnings)
    {
        sharedWarnings.emplace_back(
            std::make_shared<AdaptiveCardParseWarning>(static_cast<AdaptiveCards::WarningStatusCode>(adaptiveWarning.StatusCode()),
                                                       HStringToUTF8(adaptiveWarning.Message())));
    }
}

winrt::AdaptiveCards::ObjectModel::WinUI3::FallbackType MapSharedFallbackTypeToWinUI3(const AdaptiveCards::FallbackType type)
{
    switch (type)
    {
    case FallbackType::Drop:
    {
        return winrt::AdaptiveCards::ObjectModel::WinUI3::FallbackType::Drop;
    }

    case FallbackType::Content:
    {
        return winrt::AdaptiveCards::ObjectModel::WinUI3::FallbackType::Content;
    }

    case FallbackType::None:
    default:
    {
        return winrt::AdaptiveCards::ObjectModel::WinUI3::FallbackType::None;
    }
    }
}

AdaptiveCards::FallbackType MapWinUI3FallbackTypeToShared(const ABI::AdaptiveCards::ObjectModel::WinUI3::FallbackType type)
{
    switch (type)
    {
    case ABI::AdaptiveCards::ObjectModel::WinUI3::FallbackType::Drop:
    {
        return FallbackType::Drop;
    }

    case ABI::AdaptiveCards::ObjectModel::WinUI3::FallbackType::Content:
    {
        return FallbackType::Content;
    }

    case ABI::AdaptiveCards::ObjectModel::WinUI3::FallbackType::None:
    default:
    {
        return FallbackType::None;
    }
    }
}


AdaptiveCards::FallbackType MapWinUI3FallbackTypeToShared(winrt::AdaptiveCards::ObjectModel::WinUI3::FallbackType const& type)
{
    switch (type)
    {
    case winrt::AdaptiveCards::ObjectModel::WinUI3::FallbackType::Drop:
    {
        return FallbackType::Drop;
    }

    case winrt::AdaptiveCards::ObjectModel::WinUI3::FallbackType::Content:
    {
        return FallbackType::Content;
    }

    case winrt::AdaptiveCards::ObjectModel::WinUI3::FallbackType::None:
    default:
    {
        return FallbackType::None;
    }
    }
}

winrt::AdaptiveCards::ObjectModel::WinUI3::AdaptiveActionParserRegistration GetAdaptiveActionParserRegistrationFromSharedModel(
    const std::shared_ptr<ActionParserRegistration>& sharedActionParserRegistration)
{
    // Look up the well known action parser registration to see if we've got a custom action registration to pass
    if (auto sharedActionParser = sharedActionParserRegistration->GetParser(c_upwActionParserRegistration))
    {
        // The shared model wraps the passed in parsers. Get our SharedModelActionParser from it so we can retrieve the
        // IAdaptiveActionParserRegistration
        auto parserWrapper = std::static_pointer_cast<ActionElementParserWrapper>(sharedActionParser);
        auto sharedModelParser = std::static_pointer_cast<SharedModelActionParser>(parserWrapper->GetActualParser());

        return sharedModelParser->GetAdaptiveParserRegistration();
    }
    else
    {
        return *winrt::make_self<winrt::AdaptiveCards::ObjectModel::WinUI3::implementation::AdaptiveActionParserRegistration>();
    }
}

winrt::AdaptiveCards::ObjectModel::WinUI3::AdaptiveElementParserRegistration GetAdaptiveElementParserRegistrationFromSharedModel(
    const std::shared_ptr<AdaptiveCards::ElementParserRegistration>& sharedElementParserRegistration)
{
    // Look up the well known Element parser registration to see if we've got a custom Element registration to pass
    if (auto sharedElementParser = sharedElementParserRegistration->GetParser(c_uwpElementParserRegistration))
    {
        // The shared model wraps the passed in parsers. Get our SharedModelElementParser from it so we can retrieve the
        // IAdaptiveElementParserRegistration
        auto parserWrapper = std::static_pointer_cast<BaseCardElementParserWrapper>(sharedElementParser);
        auto sharedModelParser = std::static_pointer_cast<SharedModelElementParser>(parserWrapper->GetActualParser());

        return sharedModelParser->GetAdaptiveParserRegistration();
    }
    else
    {
        return *winrt::make_self<winrt::AdaptiveCards::ObjectModel::WinUI3::implementation::AdaptiveElementParserRegistration>();
    }
}
