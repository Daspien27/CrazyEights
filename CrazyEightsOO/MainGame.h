#pragma once

#include "Player.h"
#include "Deck.h"

class MainGame
{
	bool GameShouldStopRunning = false;
	std::vector<Player> Players{};
	
	//Game State -- usually a comment like this means we may want a class/struct...
	int ActivePlayerNum = 0;
	playing_cards::Deck Deck;
	playing_cards::Deck Discard;
	playing_cards::Suit CurrentSuit = playing_cards::Suit::NO_SUIT;

	static std::vector<Player> get_n_player_names (int N);

	static std::vector<Player> prompt_user_for_player_list ();

	std::string display_discard_chain ();

public:
	MainGame ();

	MainGame (std::vector<std::string> PlayerNames);

	void init ();
	static int get_player_count ();
	static void shuffle_players (std::vector<Player> &AllPlayers);
	void run ();
	playing_cards::Suit prompt_player_for_suit (Player& PromptedPlayer) const;
};

void print_player_list (std::vector<Player> &AllPlayers);
