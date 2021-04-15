// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

 /*
  Author: Arianne Fennell
  Date Modified: 3/29/2021
  Comment/Description: Menu Interface class with the main job of retrieving the user's friends list.
  ChangeLog:
  A 3/29/2021: Implemented base MenuInterface class to work with Hosting.
  */

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "STK_MenuInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USTK_MenuInterface : public UInterface
{
	GENERATED_BODY()
};

class SHADOWSTALK_API ISTK_MenuInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void RetrieveFriendsList() = 0;
};
