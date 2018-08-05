#include "stdafx.h"
#include "Player.h"

#include <sstream>
#include <iterator>

Player::Player (const std::string& NameUse) :
	Name (NameUse)
{
}

std::vector<std::vector<playing_cards::Card>> Player::permute_all_combinations (std::vector<playing_cards::Card> PossibleMatching) const
{
	std::vector<std::vector<playing_cards::Card>> Combinations;

	if (!PossibleMatching.empty ())
	{
		auto const CardSort = [](auto const& A, auto const& B)
		{ return static_cast<char> (A.second) < static_cast<char> (B.second); };

		std::sort (PossibleMatching.begin (), PossibleMatching.end (), CardSort);

		do {

			std::vector<std::vector<playing_cards::Card>> VecOfPossibleMatchingRanks;

			std::transform (PossibleMatching.begin (), PossibleMatching.end (), std::back_inserter (VecOfPossibleMatchingRanks),
				[](auto const& C) { return std::vector<playing_cards::Card>{ C }; });

			std::partial_sum (VecOfPossibleMatchingRanks.begin (), VecOfPossibleMatchingRanks.end (), std::back_inserter (Combinations),
				[](auto RunningContainer, auto const& C)
			{
				RunningContainer.push_back (C[0]);
				return RunningContainer;
			});
		} while (std::next_permutation (PossibleMatching.begin (), PossibleMatching.end (), CardSort));
	}

	return Combinations;
}

void Player::find_possible_matching_rank_moves(playing_cards::Rank PromptedRank,
                                               std::vector<std::vector<playing_cards::Card>>& Combinations,
                                               std::vector<playing_cards::Card> PossibleMoves) const
{
	std::vector<playing_cards::Card> PossibleMatchingRanks;

	std::copy_if (PossibleMoves.begin (), PossibleMoves.end (), std::back_inserter (PossibleMatchingRanks),
	                     [&PromptedRank](auto const& C) { return C.first == PromptedRank; });

	auto RankCombs = permute_all_combinations(PossibleMatchingRanks);

	Combinations.insert (Combinations.end (), RankCombs.begin (), RankCombs.end ());
}

void Player::find_possible_matching_suit_moves(playing_cards::Rank PromptedRank,
                                               std::vector<std::vector<playing_cards::Card>>& Combinations,
                                               std::vector<playing_cards::Card> PossibleMoves)
{
	for (auto const& SuitCard : PossibleMoves)
	{
		std::vector<playing_cards::Card> OtherPossibleRanksMatchingSuit;

		std::remove_copy_if (Hand.begin (), Hand.end (), std::back_inserter (OtherPossibleRanksMatchingSuit),
		                     [&SuitCard](auto const& C) { return !(SuitCard.first == C.first && SuitCard.second != C.second); });

		auto SuitCombs = permute_all_combinations (OtherPossibleRanksMatchingSuit);

		std::transform (SuitCombs.begin (), SuitCombs.end (), std::back_inserter (Combinations),
			[&SuitCard](auto V) {V.insert (V.begin (), SuitCard); return V; });

		Combinations.push_back ({ SuitCard });
	}
}

void Player::find_possible_prompted_rank_moves(const playing_cards::Rank PromptedRank,
                                               std::vector<std::vector<playing_cards::Card>>& Combinations,
                                               const playing_cards::Rank CrazyRank)
{
	std::vector<playing_cards::Card> PossibleMatchingRank;

	std::copy_if (Hand.begin (), Hand.end(), std::back_inserter(PossibleMatchingRank),
	              [&PromptedRank](auto const& C)
	              {
		              return (C.first == PromptedRank || C.first == playing_cards::Rank::EIGHT);
	              });

	find_possible_matching_rank_moves (CrazyRank, Combinations, PossibleMatchingRank);

	if (PromptedRank != CrazyRank)
	{
		find_possible_matching_rank_moves(PromptedRank, Combinations, PossibleMatchingRank);
	}
}

void Player::find_possible_prompted_suit_moves(playing_cards::Suit PromptedSuit, playing_cards::Rank PromptedRank, std::vector<std::vector<playing_cards::Card>>&
                                               Combinations)
{
	std::vector<playing_cards::Card> PossibleMatchingSuits;

	std::copy_if (Hand.begin (), Hand.end (), std::back_inserter (PossibleMatchingSuits),
	              [&PromptedSuit](auto const& C) { return C.second == PromptedSuit; });

	find_possible_matching_suit_moves(PromptedRank, Combinations, PossibleMatchingSuits);
}

void Player::display_combinations(std::vector<std::vector<playing_cards::Card>> const& Combinations)
{
	std::for_each (Combinations.begin (), Combinations.end (),
	               [I = 1](auto const& Comb) mutable
	               {
		               auto const CombStr = std::accumulate (std::next (Comb.begin ()), Comb.end (), playing_cards::to_string (*Comb.begin ()),
		                                                     [](auto RunningString, auto const& C) { return RunningString + " " + playing_cards::to_string (C); });

		               std::cout << "\t[" << (I++) << "]\t" << CombStr << std::endl;
	               });
}

void Player::prompt_action (const playing_cards::Rank PromptedRank, const playing_cards::Suit PromptedSuit)
{
	std::vector<std::vector<playing_cards::Card>> Combinations;

	find_possible_prompted_rank_moves(PromptedRank, Combinations, playing_cards::Rank::EIGHT);

	find_possible_prompted_suit_moves(PromptedSuit, PromptedRank, Combinations);

	if (!Combinations.empty ())
	{
		display_combinations(Combinations);
		
		auto Choice = 0;

		std::cin >> Choice;
	}
	else
	{
		std::cout << "You have no moves available. Press enter to pick up a card." << std::endl;

		std::cin.ignore (std::numeric_limits<std::streamsize>::max (), '\n');
	}

}

const std::string & Player::get_name () const
{
	return Name;
}

std::string Player::display_hand () const
{
	return std::accumulate (std::next (Hand.begin ()), Hand.end (), playing_cards::to_string (*Hand.begin ()),
		[](auto HandStr, auto const& C) { return HandStr + " " + playing_cards::to_string (C); });
}

void Player::place_in_hand (const playing_cards::Card CardForHand)
{
	Hand.push_back (CardForHand);
}