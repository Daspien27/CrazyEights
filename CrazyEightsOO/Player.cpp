#include "stdafx.h"
#include "Player.h"

#include <sstream>

Player::Player () : Name ("") 
{
}

Player::Player (const std::string& NameUse) :
	Name (NameUse)
{
}


Player::~Player ()
{
}

void Player::prompt_action ()
{
}

const std::string & Player::get_name () const
{
	return Name;
}

std::string Player::display_hand () const
{
	std::stringstream ss;

	for (auto const& c : Hand)
	{
		ss << PlayingCards::to_string (c) << " ";
	}
	

	return ss.str ();
}
void Player::place_in_hand (PlayingCards::Card card)
{
	Hand.push_back (card);
}
