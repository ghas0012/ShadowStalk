Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

ShadowStalk User Guide

Shade Controls:

    W A S D           - Basic movement
    Mouse             - Look around
    Spacebar          - Jump
    L Ctrl (Hold)     - Crouch
    L Shift (Hold)    - Sprint

Monster Controls:

    W A S D           - Basic movement
    Mouse             - Look around
    Spacebar          - Jump
    L Shift (Hold)    - Sprint
    L Mouse Button    - Attack
    E                 - Execute (must be looking at downed "Shade")

ShadowStalk How-To:

    ShadowStalk is a heavily stylized asymmetrical co-op horror game where the players must work together to gather keys with the goal of escaping the confines of Limbo, while avoiding the grasp of the Monster.
	Keys will randomly spawn around the map and once a set amount have been collected, a randomly designated exit door will open, granting them a chance to escape and win the round.

    The Monster can stop a Shade from escaping Limbo by attacking them a total of 2 times, at which point they will get downed.
	The Monster can execute a downed shade. If the Monster has killed all Shades, then it wins the round.

Important Note:
	
	To play as the Monster, open Contents/Blueprints/Misc/BP_MatchGameMode and check the box "Play as Monster" under the "Logistics" category.
	
Gameplay Features:

    - Randomized Keys and Pickups.
    - Randomized exit doors to be unlocked with keys.
    - Custom User Interface (made with C++/UMG).
    - Player characters with custom movement component.
    - Monster player attack and execution.
    - Player character animations.
