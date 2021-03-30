// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

#include "STK_UWPauseMenu.h"

#include "ShadowStalk/UI/STK_UserWidget.h"
#include "ShadowStalk/UI/STK_UWOptionsPanel.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "ShadowStalk/GameInstance/STK_GameInstance.h"

USTK_UWPauseMenu::USTK_UWPauseMenu(const FObjectInitializer& ObjectInitializer) :
    Super(ObjectInitializer)
{
    //Search for Options Panel Widget Blueprint
    {
        ConstructorHelpers::FClassFinder<UUserWidget> OptionsPanelBPClass(TEXT("/Game/UI/WBP_OptionsPanel"));
        if (!ensure(OptionsPanelBPClass.Class != nullptr)) return;

        OptionsPanelClass = OptionsPanelBPClass.Class;
    }
}

bool USTK_UWPauseMenu::Initialize()
{
    bool Success = Super::Initialize();
    if (!Success) return false;

    //Setup Input for Continue Button
    if (!ensure(ContinueButton != nullptr)) return false;
    ContinueButton->OnClicked.AddDynamic(this, &USTK_UWPauseMenu::ExitPauseMenu);
    ContinueButton->OnHovered.AddDynamic(this, &USTK_UserWidget::PlayHoverSoundFX);

    //Setup Input for Options Button
    if (!ensure(OptionsButton != nullptr)) return false;
    OptionsButton->OnClicked.AddDynamic(this, &USTK_UWPauseMenu::OpenOptionsMenu);
    OptionsButton->OnHovered.AddDynamic(this, &USTK_UserWidget::PlayHoverSoundFX);

    //Setup Input for Quit Button
    if (!ensure(QuitButton != nullptr)) return false;
    QuitButton->OnClicked.AddDynamic(this, &USTK_UWPauseMenu::ReturnToMainMenu);
    QuitButton->OnHovered.AddDynamic(this, &USTK_UserWidget::PlayHoverSoundFX);

    //Setup Input for Exit Button
    if (!ensure(ExitButton != nullptr)) return false;
    ExitButton->OnClicked.AddDynamic(this, &USTK_UWPauseMenu::ExitPauseMenu);
    ExitButton->OnHovered.AddDynamic(this, &USTK_UserWidget::PlayHoverSoundFX);
    
    return true;
}

void USTK_UWPauseMenu::ExitPauseMenu()
{
    this->Teardown();
}

void USTK_UWPauseMenu::OpenOptionsMenu()
{
    if (!ensure(OptionsPanelClass != nullptr)) return;

    UWOptionsPanel = CreateWidget<USTK_UWOptionsPanel>(this, OptionsPanelClass);
    if (!ensure(UWOptionsPanel != nullptr)) return;

    UWOptionsPanel->BGOverlay->SetVisibility(ESlateVisibility::Hidden);
    UWOptionsPanel->AddToViewport();
}

void USTK_UWPauseMenu::ReturnToMainMenu()
{
    this->Teardown();

    auto GameInstance = Cast<USTK_GameInstance>(GetGameInstance());
    if (GameInstance == nullptr) return;

    GameInstance->SetupMainMenuWidget();

    UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
}
