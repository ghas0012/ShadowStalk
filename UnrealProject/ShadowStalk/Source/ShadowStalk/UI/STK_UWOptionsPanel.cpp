// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

#include "STK_UWOptionsPanel.h"

#include "Components/Button.h"

bool USTK_UWOptionsPanel::Initialize()
{
    bool Success = Super::Initialize();
    if (!Success) return false;

    //Setup Input for Exit Button
    if (!ensure(ExitButton != nullptr)) return false;
    ExitButton->OnClicked.AddDynamic(this, &USTK_UWOptionsPanel::ExitPressed);
    ExitButton->OnClicked.AddDynamic(this, &USTK_UserWidget::PlayHoverSoundFX);

    return true;
}

///<summary>
///Removes the Options Panel widget from the viewport but keeps default input method.
///</summary>
void USTK_UWOptionsPanel::ExitPressed()
{
    this->RemoveFromViewport();
}
