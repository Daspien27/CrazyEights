#pragma once

#include "Player.h"
#include "Deck.h"

class MainGame
{
	bool GameShouldStopRunning = false;
	std::vector<Player> Players;
	
	//Game State -- usually a comment like this means we may want a class/struct...
	int ActivePlayerNum = 0;
	PlayingCards::Deck Deck;
	PlayingCards::Deck Discard;
	PlayingCards::Suit CurrentSuit;

	static std::vector<Player> get_n_player_names (int N);

	std::vector<Player> PromptUserForPlayerList ();

	std::string display_discard_chain ();

public:
	MainGame ();

	MainGame (std::vector<std::string> PlayerNames);

	~MainGame ();

	void init ();
	int get_player_count ();
	void shuffle_players (std::vector<Player> &AllPlayers);
	void run ();
	PlayingCards::Suit PromptPlayerForSuit (Player& player);
};

void print_player_list (std::vector<Player> &AllPlayers);
