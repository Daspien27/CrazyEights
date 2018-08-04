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
	std::vector<std::vector<Card>> Combinations;
	std::multimap<PlayingCards::Rank, Card> MultiMapMoves;

	std::vector<Card> PossibleMoves;

	std::remove_copy_if (Hand.begin (), Hand.end(), std::back_inserter(PossibleMoves),
		[&rank, &suit](auto const& card) { return !(card.second == suit || card.first == rank || card.first == PlayingCards::Rank::EIGHT); });

	//Remove all 8's

	std::vector<Card> PossibleEights;

	std::remove_copy_if (PossibleMoves.begin (), PossibleMoves.end (), std::back_inserter (PossibleEights),
		[](auto const& card) { return card.first != PlayingCards::Rank::EIGHT; });

	std::transform (PossibleEights.begin (), PossibleEights.end (), std::inserter (MultiMapMoves, MultiMapMoves.begin ()),
		[](auto const& card) { return std::make_pair (PlayingCards::Rank::EIGHT, card); });

	if (!PossibleEights.empty ())
	{
		auto n = PossibleEights.size ();

		for (auto r = n; r > 0; --r)
		{
			std::vector<bool> perm (n);
			std::fill (perm.end () - r, perm.end (), true);

			do {
				std::vector<std::pair<bool, Card>> zip;

				std::transform (perm.begin (), perm.end (), PossibleEights.begin (), std::back_inserter (zip),
					[](auto const& perm, auto const& c) {return std::make_pair (perm, c); });
				

				auto zipIt = std::remove_if (zip.begin (), zip.end (),
					[](auto const& z) { return !z.first; });
				
				std::vector<Card> comb;
				
				std::transform (zip.begin (), zipIt, std::back_inserter (comb),
					[](auto const& z) { return z.second; });

				Combinations.push_back (comb);

			} while (std::next_permutation (perm.begin (), perm.end ()));
		}

	}

	//If the current rank isn't 8 we need all the possible moves for the current rank

	if (rank != PlayingCards::Rank::EIGHT)
	{
		std::vector<Card> PossibleMatchingRanks;

		std::remove_copy_if (PossibleMoves.begin (), PossibleMoves.end (), std::back_inserter (PossibleMatchingRanks),
			[&rank](auto const& card) { return card.first != rank; });

		std::transform (PossibleMatchingRanks.begin (), PossibleMatchingRanks.end (), std::inserter (MultiMapMoves, MultiMapMoves.begin ()),
			[&rank](auto const& card) { return std::make_pair (rank, card); });


		if (!PossibleMatchingRanks.empty ())
		{
			auto n = PossibleMatchingRanks.size ();

			for (auto r = n; r > 0; --r)
			{
				std::vector<bool> perm (n);
				std::fill (perm.end () - r, perm.end (), true);

				do {
					std::vector<std::pair<bool, Card>> zip;

					std::transform (perm.begin (), perm.end (), PossibleMatchingRanks.begin (), std::back_inserter (zip),
						[](auto const& perm, auto const& c) {return std::make_pair (perm, c); });


					auto zipIt = std::remove_if (zip.begin (), zip.end (),
						[](auto const& z) { return !z.first; });

					std::vector<Card> comb;

					std::transform (zip.begin (), zipIt, std::back_inserter (comb),
						[](auto const& z) { return z.second; });

					Combinations.push_back (comb);

				} while (std::next_permutation (perm.begin (), perm.end ()));
			}

		}
	}

	//Find all possible groups of lead by matching suits
	std::vector<Card> PossibleMatchingSuits;

	std::remove_copy_if (PossibleMoves.begin (), PossibleMoves.end (), std::back_inserter (PossibleMatchingSuits),
		[&rank](auto const& card) { return (card.first == rank || card.first == PlayingCards::Rank::EIGHT); });


	for (auto const& card : PossibleMatchingSuits)
	{
		std::vector<Card> OtherPossibleRanksMatchingSuit;

		std::remove_copy_if (Hand.begin (), Hand.end (), std::back_inserter (OtherPossibleRanksMatchingSuit),
			[&card](auto const& c) { return !(card.first == c.first && card.second != c.second); });

		std::transform (OtherPossibleRanksMatchingSuit.begin (), OtherPossibleRanksMatchingSuit.end (), std::inserter (MultiMapMoves, MultiMapMoves.begin ()),
			[&card](auto const& c) { return std::make_pair(card.first, c); });


		if (!OtherPossibleRanksMatchingSuit.empty ())
		{
			auto n = OtherPossibleRanksMatchingSuit.size ();

			for (auto r = static_cast<int>(n); r >= 0; --r)
			{
				std::vector<bool> perm (n);
				std::fill (perm.end () - r, perm.end (), true);

				do {
					std::vector<std::pair<bool, Card>> zip;

					std::transform (perm.begin (), perm.end (), OtherPossibleRanksMatchingSuit.begin (), std::back_inserter (zip),
						[](auto const& perm, auto const& c) {return std::make_pair (perm, c); });


					auto zipIt = std::remove_if (zip.begin (), zip.end (),
						[](auto const& z) { return !z.first; });

					std::vector<Card> comb;

					comb.push_back (card);

					std::transform (zip.begin (), zipIt, std::back_inserter (comb),
						[](auto const& z) { return z.second; });

					Combinations.push_back (comb);

				} while (std::next_permutation (perm.begin (), perm.end ()));
			}

		}
	}


	if (!Combinations.empty ())
	{
		int i = 1;
		for (const auto& comb : Combinations)
		{
			std::cout << "\t[" << (i++) << "]\t";
			for (auto const& c : comb)
			{
				std::cout << PlayingCards::to_string (c) << " ";
			}
			std::cout << std::endl;
		}


		int choice = 0;

		std::cin >> choice;
	}

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
