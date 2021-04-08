// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

#include "STK_UWPauseMenu.h"

#include "ShadowStalk/UI/STK_UserWidget.h"
#include "ShadowStalk/UI/STK_UWOptionsPanel.h"
#include "ShadowStalk/UI/STK_UWInviteMenu.h"
#include "ShadowStalk/UI/STK_UWConfirmQuit.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "ShadowStalk/GameInstance/STK_GameInstance.h"

USTK_UWPauseMenu::USTK_UWPauseMenu(const FObjectInitializer& ObjectInitializer) :
    Super(ObjectInitializer)
{
    //Search for Options Panel Widget Blueprint
    {
        ConstructorHelpers::FClassFinder<UUserWidget> InviteMenuBPClass(TEXT("/Game/UI/WBP_InviteMenu"));
        if (!ensure(InviteMenuBPClass.Class != nullptr)) return;

        InviteMenuClass = InviteMenuBPClass.Class;
    }

    //Search for Options Panel Widget Blueprint
    {
        ConstructorHelpers::FClassFinder<UUserWidget> OptionsPanelBPClass(TEXT("/Game/UI/WBP_OptionsPanel"));
        if (!ensure(OptionsPanelBPClass.Class != nullptr)) return;

        OptionsPanelClass = OptionsPanelBPClass.Class;
    }

    //Search for Confirm Quit Dialogue Widget Blueprint
    {
        ConstructorHelpers::FClassFinder<UUserWidget> ConfirmQuitBPClass(TEXT("/Game/UI/WBP_QuitConfirm"));
        if (!ensure(ConfirmQuitBPClass.Class != nullptr)) return;

        ConfirmQuitClass = ConfirmQuitBPClass.Class;
    }
}

bool USTK_UWPauseMenu::Initialize()
{
    bool Success = Super::Initialize();
    if (!Success) return false;

    //Setup Input for Continue Button
    if (!ensure(InviteButton != nullptr)) return false;
    InviteButton->OnClicked.AddDynamic(this, &USTK_UWPauseMenu::OpenInviteMenu);
    InviteButton->OnHovered.AddDynamic(this, &USTK_UserWidget::PlayHoverSoundFX);

    //Setup Input for Options Button
    if (!ensure(OptionsButton != nullptr)) return false;
    OptionsButton->OnClicked.AddDynamic(this, &USTK_UWPauseMenu::OpenOptionsMenu);
    OptionsButton->OnHovered.AddDynamic(this, &USTK_UserWidget::PlayHoverSoundFX);

    //Setup Input for Quit Button
    if (!ensure(QuitButton != nullptr)) return false;
    QuitButton->OnClicked.AddDynamic(this, &USTK_UWPauseMenu::OpenConfirmQuitBox);
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

/// <summary>
/// Creates and sets up the Invite Menu Widget in the game's viewport.
/// </summary>
void USTK_UWPauseMenu::OpenInviteMenu()
{
    if (!ensure(InviteMenuClass != nullptr)) return;

    UWInviteMenu = CreateWidget<USTK_UWInviteMenu>(this, InviteMenuClass);
    if (!ensure(UWInviteMenu != nullptr)) return;

    UWInviteMenu->BGOverlay->SetVisibility(ESlateVisibility::Hidden);
    UWInviteMenu->AddToViewport();
}

/// <summary>
/// Creates and sets up the Options Panel Widget in the game's viewport.
/// </summary>
void USTK_UWPauseMenu::OpenOptionsMenu()
{
    if (!ensure(OptionsPanelClass != nullptr)) return;

    UWOptionsPanel = CreateWidget<USTK_UWOptionsPanel>(this, OptionsPanelClass);
    if (!ensure(UWOptionsPanel != nullptr)) return;

    UWOptionsPanel->BGOverlay->SetVisibility(ESlateVisibility::Hidden);
    UWOptionsPanel->AddToViewport();
}

/// <summary>
/// Creates and sets up the Quit Confirmation Widget in the game's viewport.
/// </summary>
void USTK_UWPauseMenu::OpenConfirmQuitBox()
{
    this->Teardown();

    if (!ensure(ConfirmQuitClass != nullptr)) return;

    UWConfirmQuit = CreateWidget<USTK_UWConfirmQuit>(this, ConfirmQuitClass);
    if (!ensure(UWConfirmQuit != nullptr)) return;

    UWConfirmQuit->Setup();
}

/// <summary>
/// Enables the player to quit the session and return to the main menu.
/// </summary>
void USTK_UWPauseMenu::ReturnToMainMenu()
{
    this->Teardown();

    auto GameInstance = Cast<USTK_GameInstance>(GetGameInstance());
    if (GameInstance == nullptr) return;

    GameInstance->SetupMainMenuWidget();

    UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
}
