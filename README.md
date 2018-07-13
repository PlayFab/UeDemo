# UeDemo
This repository contains a simple Breakout game with the UE PlayFab Plugin integrated. It depends on the PlayFab UE Plugin in the UE Marketplace.

# The game consists of three levels
1. Start Screen - allows the user to register or login with an account. The DevSecret is registered on the Start Screen as it is needed to upload high scores.
2. Gameplay - the Breakout game
3. Leaderboard - shows the player display names for the high scores page

# Calls to PlayFab services:
1. ClientAPI::RegisterPlayFabUser() - used to create a new user account for the associated titleId
2. ClientAPI::LoginWithPlayFab() - used to login with an existing user account for the associated titleId
3. ServerAPI::UpdatePlayerStatistics() - used to save the current player's score
4. ClientAPI::GetLeaderboard() - retrieves high scores

# Instructions to run:
1. Clone repository: git clone https://github.com/PlayFab/UeDemo.git
2. Right click the uproject file and Generate Visual Studio project files
3. Open and run the solution file (or open the uproject directly)
4. Select the StartScreen level
5. Play
