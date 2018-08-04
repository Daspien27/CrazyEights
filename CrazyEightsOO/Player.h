#pragma once

#include "Deck.h"

class Player
{
	std::string Name;
	std::vector<PlayingCards::Card> Hand;

public:
	Player ();

	explicit Player (const std::string& NameUse);

	~Player ();

	void prompt_action ();

	const std::string& get_name () const;
	std::string display_hand () const;

	void place_in_hand (PlayingCards::Card card);
};

