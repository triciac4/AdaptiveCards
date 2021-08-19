// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
#pragma once

#include "ImageSet.h"

namespace AdaptiveCards::ObjectModel::WinUI3
{
    class DECLSPEC_UUID("04ed4200-de21-4587-8bc5-74b000809985") AdaptiveImageSet
        : public Microsoft::WRL::RuntimeClass<Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::RuntimeClassType::WinRtClassicComMix>,
                                              ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveImageSet,
                                              ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement,
                                              Microsoft::WRL::CloakedIid<ITypePeek>,
                                              Microsoft::WRL::CloakedIid<AdaptiveCards::ObjectModel::WinUI3::AdaptiveCardElementBase>>
    {
        AdaptiveRuntime(AdaptiveImageSet);

    public:
        AdaptiveImageSet();

        HRESULT RuntimeClassInitialize() noexcept;
        HRESULT RuntimeClassInitialize(const std::shared_ptr<AdaptiveCards::ImageSet>& sharedImageSet);

        // IAdaptiveImageSet
        IFACEMETHODIMP get_Images(
            _COM_Outptr_ ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveCards::ObjectModel::WinUI3::AdaptiveImage*>** columns) override;

        IFACEMETHODIMP get_ImageSize(_Out_ ABI::AdaptiveCards::ObjectModel::WinUI3::ImageSize* imageSize) override;
        IFACEMETHODIMP put_ImageSize(ABI::AdaptiveCards::ObjectModel::WinUI3::ImageSize imageSize) override;

        // IAdaptiveCardElement
        IFACEMETHODIMP get_ElementType(_Out_ ABI::AdaptiveCards::ObjectModel::WinUI3::ElementType* elementType) override;

        IFACEMETHODIMP get_Spacing(_Out_ ABI::AdaptiveCards::ObjectModel::WinUI3::Spacing* spacing) override
        {
            return AdaptiveCardElementBase::get_Spacing(spacing);
        }
        IFACEMETHODIMP put_Spacing(ABI::AdaptiveCards::ObjectModel::WinUI3::Spacing spacing) override
        {
            return AdaptiveCardElementBase::put_Spacing(spacing);
        }

        IFACEMETHODIMP get_Separator(_Out_ boolean* separator) override
        {
            return AdaptiveCardElementBase::get_Separator(separator);
        }
        IFACEMETHODIMP put_Separator(boolean separator) override
        {
            return AdaptiveCardElementBase::put_Separator(separator);
        }

        IFACEMETHODIMP get_IsVisible(_Out_ boolean* isVisible) override
        {
            return AdaptiveCardElementBase::get_IsVisible(isVisible);
        }
        IFACEMETHODIMP put_IsVisible(boolean isVisible) override
        {
            return AdaptiveCardElementBase::put_IsVisible(isVisible);
        }

        IFACEMETHODIMP get_Id(_Outptr_ HSTRING* id) override { return AdaptiveCardElementBase::get_Id(id); }
        IFACEMETHODIMP put_Id(_In_ HSTRING id) override { return AdaptiveCardElementBase::put_Id(id); }

        IFACEMETHODIMP get_FallbackType(_Out_ ABI::AdaptiveCards::ObjectModel::WinUI3::FallbackType* fallback) override
        {
            return AdaptiveCardElementBase::get_FallbackType(fallback);
        }
        IFACEMETHODIMP get_FallbackContent(_COM_Outptr_ ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement** content) override
        {
            return AdaptiveCardElementBase::get_FallbackContent(content);
        }

        IFACEMETHODIMP put_FallbackType(ABI::AdaptiveCards::ObjectModel::WinUI3::FallbackType fallback) override
        {
            return AdaptiveCardElementBase::put_FallbackType(fallback);
        }

        IFACEMETHODIMP put_FallbackContent(_In_ ABI::AdaptiveCards::ObjectModel::WinUI3::IAdaptiveCardElement* content) override
        {
            return AdaptiveCardElementBase::put_FallbackContent(content);
        }

        IFACEMETHODIMP get_ElementTypeString(_Outptr_ HSTRING* value) override
        {
            return AdaptiveCardElementBase::get_ElementTypeString(value);
        }

        IFACEMETHODIMP get_Height(_Out_ ABI::AdaptiveCards::ObjectModel::WinUI3::HeightType* height) override
        {
            return AdaptiveCardElementBase::get_Height(height);
        }
        IFACEMETHODIMP put_Height(ABI::AdaptiveCards::ObjectModel::WinUI3::HeightType height) override
        {
            return AdaptiveCardElementBase::put_Height(height);
        }

        IFACEMETHODIMP get_AdditionalProperties(_COM_Outptr_ ABI::Windows::Data::Json::IJsonObject** result) override
        {
            return AdaptiveCardElementBase::get_AdditionalProperties(result);
        }
        IFACEMETHODIMP put_AdditionalProperties(_In_ ABI::Windows::Data::Json::IJsonObject* value) override
        {
            return AdaptiveCardElementBase::put_AdditionalProperties(value);
        }

        IFACEMETHODIMP get_Requirements(
            _COM_Outptr_ ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveCards::ObjectModel::WinUI3::AdaptiveRequirement*>** requirements) override
        {
            return AdaptiveCardElementBase::get_Requirements(requirements);
        }

        IFACEMETHODIMP ToJson(_COM_Outptr_ ABI::Windows::Data::Json::IJsonObject** result) override
        {
            return AdaptiveCardElementBase::ToJson(result);
        }

        virtual HRESULT GetSharedModel(std::shared_ptr<AdaptiveCards::BaseCardElement>& sharedModel) override;

        // ITypePeek method
        void* PeekAt(REFIID riid) override { return PeekHelper(riid, this); }

    private:
        Microsoft::WRL::ComPtr<ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveCards::ObjectModel::WinUI3::AdaptiveImage*>> m_images;
        ABI::AdaptiveCards::ObjectModel::WinUI3::ImageSize m_imageSize;
    };

    ActivatableClass(AdaptiveImageSet);
}