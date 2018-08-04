#include "stdafx.h"
#include "Player.h"

#include <sstream>
#include <iterator>

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

void Player::prompt_action (PlayingCards::Rank rank, PlayingCards::Suit suit)
{
	using PlayingCards::Card;
	std::vector<std::vector<Card>> combinations;
	std::multimap<PlayingCards::Rank, Card> possibleMoves;

	std::vector<Card> TempHand (Hand);
	
	std::transform (TempHand.begin (), TempHand.end (), std::inserter (possibleMoves, possibleMoves.begin ()),
		[](auto const& card) { return make_pair (card.first, card); });
		
	for (auto it = possibleMoves.begin (), end = possibleMoves.end (); it != end; it = possibleMoves.upper_bound (it->first))
	{
		auto range = possibleMoves.equal_range (it->first);

		auto n = std::distance (range.first, range.second);

		auto findcard = std::find_if (range.first, range.second,
			[](auto const& kv) { auto const& c = kv.second; !(rank == c.first || suit == c.second || c.first == PlayingCards::Rank::EIGHT); });

		for (auto r = n; r > 0; --r)
		{
			std::vector<bool> perm (n);
			std::fill (perm.end () - r, perm.end (), true);
						
			do {
				std::vector<Card> comb;
				comb.reserve (r);

				auto permIt = range.first;
		/*		
				std::transform (perm.begin (), perm.end (), std::back_inserter (comb),
					[&permIt, &possibleMoves] (auto const& perm) mutable {return (*(permIt++)).second; },
					[&permIt](auto const& perm) {if (!perm) permIt++; return perm; });
					*/
				combinations.push_back (comb);

			} while (std::next_permutation (perm.begin (), perm.end ()));
		}
	}

	auto bad_comb = std::remove_if (combinations.begin (), combinations.end (),
		[&rank, &suit](auto const& cards) { return !(rank == cards[0].first || suit == cards[0].second || cards[0].first == PlayingCards::Rank::EIGHT); });

	for (auto it = combinations.begin (); it != bad_comb; ++it)
	{
		std::cout << "\t[" << (std::distance (combinations.begin (), it) + 1) << "]\t";
		for (auto const& c : *it)
		{
			 std::cout << PlayingCards::to_string (c) << " ";
		}
		std::cout << std::endl;
	}


	int choice = 0;

	std::cin >> choice;
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
