#include "stdafx.h"
#include "Player.h"

#include <sstream>
#include <iterator>

Player::Player (const std::string& NameUse) :
	Name (NameUse)
{
}


void Player::find_possible_eight_moves(std::vector<std::vector<playing_cards::Card>>& Combinations,
                                       std::vector<playing_cards::Card> PossibleMoves) const
{
	std::vector<playing_cards::Card> PossibleEights;

	std::remove_copy_if (PossibleMoves.begin (), PossibleMoves.end (), std::back_inserter (PossibleEights),
	                     [](auto const& C) { return C.first != playing_cards::Rank::EIGHT; });

	if (!PossibleEights.empty ())
	{
		const auto CardSort = [](auto const& A, auto const& B)
		{ return static_cast<char> (A.second) < static_cast<char> (B.second); };

		std::sort (PossibleEights.begin (), PossibleEights.end (), CardSort);

		do {

			std::vector<std::vector<playing_cards::Card>> VecOfPossibleMatchingRanks;

			std::transform (PossibleEights.begin (), PossibleEights.end (), std::back_inserter (VecOfPossibleMatchingRanks),
				[](auto const& C) { return std::vector<playing_cards::Card>{ C }; });

			std::partial_sum (VecOfPossibleMatchingRanks.begin (), VecOfPossibleMatchingRanks.end (), std::back_inserter (Combinations),
				[](auto A, auto const& B)
			{
				A.push_back (B[0]);
				return A;
			});
		} while (std::next_permutation (PossibleEights.begin (), PossibleEights.end (), CardSort));
	}
}

void Player::find_possible_matching_rank_moves(playing_cards::Rank PromptedRank,
                                               std::vector<std::vector<playing_cards::Card>>& Combinations,
                                               std::vector<playing_cards::Card> PossibleMoves) const
{
	std::vector<playing_cards::Card> PossibleMatchingRanks;

	std::remove_copy_if (PossibleMoves.begin (), PossibleMoves.end (), std::back_inserter (PossibleMatchingRanks),
	                     [&PromptedRank](auto const& C) { return C.first != PromptedRank; });

	if (!PossibleMatchingRanks.empty ())
	{
		const auto CardSort = [](auto const& A, auto const& B) 
		{ return static_cast<char> (A.second) < static_cast<char> (B.second); };

		std::sort (PossibleMatchingRanks.begin (), PossibleMatchingRanks.end (), CardSort);

		do {

			std::vector<std::vector<playing_cards::Card>> VecOfPossibleMatchingRanks;

			std::transform (PossibleMatchingRanks.begin (), PossibleMatchingRanks.end (), std::back_inserter (VecOfPossibleMatchingRanks),
				[](auto const& C) { return std::vector<playing_cards::Card>{ C }; });

			std::partial_sum (VecOfPossibleMatchingRanks.begin (), VecOfPossibleMatchingRanks.end (), std::back_inserter(Combinations),
				[](auto A, auto const& B)
			{
				A.push_back (B[0]); 
				return A;
			});
		} while (std::next_permutation (PossibleMatchingRanks.begin (), PossibleMatchingRanks.end (), CardSort));
	}
}

void Player::find_possible_matching_suit_moves(playing_cards::Rank PromptedRank,
                                               std::vector<std::vector<playing_cards::Card>>& Combinations,
                                               std::vector<playing_cards::Card> PossibleMoves)
{
	std::vector<playing_cards::Card> PossibleMatchingSuits;

	std::remove_copy_if (PossibleMoves.begin (), PossibleMoves.end (), std::back_inserter (PossibleMatchingSuits),
	                     [&PromptedRank](auto const& C) { return (C.first == PromptedRank || C.first == playing_cards::Rank::EIGHT); });


	for (auto const& SuitCard : PossibleMatchingSuits)
	{
		std::vector<playing_cards::Card> OtherPossibleRanksMatchingSuit;

		std::remove_copy_if (Hand.begin (), Hand.end (), std::back_inserter (OtherPossibleRanksMatchingSuit),
		                     [&SuitCard](auto const& C) { return !(SuitCard.first == C.first && SuitCard.second != C.second); });

		if (!OtherPossibleRanksMatchingSuit.empty ())
		{
			const auto CardSort = [](auto const& A, auto const& B)
			{ return static_cast<char> (A.second) < static_cast<char> (B.second); };

			std::sort (OtherPossibleRanksMatchingSuit.begin (), OtherPossibleRanksMatchingSuit.end (), CardSort);

			do {

				std::vector<std::vector<playing_cards::Card>> VecOfPossibleMatchingSuits;

				std::transform (OtherPossibleRanksMatchingSuit.begin (), OtherPossibleRanksMatchingSuit.end (), std::back_inserter (VecOfPossibleMatchingSuits),
					[](auto const& C) { return std::vector<playing_cards::Card>{ C }; });


				std::partial_sum (VecOfPossibleMatchingSuits.begin (), VecOfPossibleMatchingSuits.end (), VecOfPossibleMatchingSuits.begin (),
					[](auto A, auto const& B)
				{
					A.push_back (B[0]);
					return A;
				});

				std::transform (VecOfPossibleMatchingSuits.begin (), VecOfPossibleMatchingSuits.end (), std::back_inserter(Combinations),
					[&SuitCard](auto V) {V.insert (V.begin (), SuitCard); return V; });


			} while (std::next_permutation (OtherPossibleRanksMatchingSuit.begin (), OtherPossibleRanksMatchingSuit.end (), CardSort));
		}

		Combinations.push_back ({ SuitCard });
	}
}

void Player::prompt_action (playing_cards::Rank PromptedRank, playing_cards::Suit PromptedSuit)
{
	using playing_cards::Card;
	using playing_cards::Rank;

	std::vector<std::vector<Card>> Combinations;

	std::vector<Card> PossibleMoves;

	std::remove_copy_if (Hand.begin (), Hand.end(), std::back_inserter(PossibleMoves),
		[&PromptedRank, &PromptedSuit](auto const& C) { return !(C.second == PromptedSuit || C.first == PromptedRank || C.first == Rank::EIGHT); });


	find_possible_eight_moves(Combinations, PossibleMoves);

	if (PromptedRank != Rank::EIGHT)
	{
		find_possible_matching_rank_moves(PromptedRank, Combinations, PossibleMoves);
	}

	find_possible_matching_suit_moves(PromptedRank, Combinations, PossibleMoves);

	if (!Combinations.empty ())
	{
		auto I = 1;
		for (const auto& Comb : Combinations)
		{
			std::cout << "\t[" << (I++) << "]\t";
			for (auto const& C : Comb)
			{
				std::cout << playing_cards::to_string (C) << " ";
			}
			std::cout << std::endl;
		}


		auto Choice = 0;

		std::cin >> Choice;
	}

}

const std::string & Player::get_name () const
{
	return Name;
}

std::string Player::display_hand () const
{
	std::stringstream Ss;

	for (auto const& C : Hand)
	{
		Ss << playing_cards::to_string (C) << " ";
	}
	

	return Ss.str ();
}
void Player::place_in_hand (const playing_cards::Card CardForHand)
{
	Hand.push_back (CardForHand);
}
