// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

#include "STK_UWCreditsPanel.h"

#include "Components/Button.h"

bool USTK_UWCreditsPanel::Initialize()
{
    bool Success = Super::Initialize();
    if (!Success) return false;

    //Setup Input for Exit Button
    if (!ensure(ExitButton != nullptr)) return false;
    ExitButton->OnClicked.AddDynamic(this, &USTK_UWCreditsPanel::ExitPressed);
    ExitButton->OnHovered.AddDynamic(this, &USTK_UserWidget::PlayHoverSoundFX);

    return true;
}

///<summary>
///Removes the Credits Panel widget from the viewport but keeps default input method.
///</summary>
void USTK_UWCreditsPanel::ExitPressed()
{
    this->RemoveFromViewport();
}
