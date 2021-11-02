// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
#pragma once

#include "ChoiceSetInput.h"
#include "AdaptiveInputElement.h"
#include "AdaptiveChoiceSetInput.g.h"

namespace winrt::AdaptiveCards::ObjectModel::WinUI3::implementation
{
    struct DECLSPEC_UUID("fa103f57-5d54-48ba-80a5-d8939b85e82d") AdaptiveChoiceSetInput : AdaptiveChoiceSetInputT<AdaptiveChoiceSetInput, ITypePeek>,
        implementation::AdaptiveInputElementBase
    {
        AdaptiveChoiceSetInput() : AdaptiveChoiceSetInput(std::make_shared<::AdaptiveCards::ChoiceSetInput>()) {}
        AdaptiveChoiceSetInput(std::shared_ptr<::AdaptiveCards::ChoiceSetInput> const& sharedChoiceSetInput);

        auto ElementType() { return WinUI3::ElementType::ChoiceSetInput; }

        // IAdaptiveChoiceSetInput
        property<hstring> Placeholder;
        property<bool> IsMultiSelect;
        property<bool> Wrap;
        property<hstring> Value;
        property<WinUI3::ChoiceSetStyle> ChoiceSetStyle;
        property<winrt::Windows::Foundation::Collections::IVector<WinUI3::AdaptiveChoiceInput>> Choices;

        virtual std::shared_ptr<::AdaptiveCards::BaseCardElement> GetSharedModel() override;

        // ITypePeek method
        void* PeekAt(REFIID riid) override { return PeekHelper(riid, this); }
    };
}

namespace winrt::AdaptiveCards::ObjectModel::WinUI3::factory_implementation
{
    struct AdaptiveChoiceSetInput : AdaptiveChoiceSetInputT<AdaptiveChoiceSetInput, implementation::AdaptiveChoiceSetInput>
    {
    };
}