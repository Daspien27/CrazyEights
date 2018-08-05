#include "stdafx.h"
#include "Player.h"

#include <sstream>
#include <iterator>

Player::Player (const std::string& NameUse) :
	Name (NameUse)
{
}


void Player::find_possible_eight_moves(std::vector<std::vector<playing_cards::Card>>& Combinations,
                                       std::vector<playing_cards::Card> PossibleMoves)
{
	std::vector<playing_cards::Card> PossibleEights;

	std::remove_copy_if (PossibleMoves.begin (), PossibleMoves.end (), std::back_inserter (PossibleEights),
	                     [](auto const& C) { return C.first != playing_cards::Rank::EIGHT; });

	if (!PossibleEights.empty ())
	{
		const auto N = PossibleEights.size ();

		for (auto R = N; R > 0; --R)
		{
			std::vector<int> Perm (N);
			std::iota (Perm.end () - R, Perm.end (), 1);

			do {
				std::vector<std::pair<int, playing_cards::Card>> Zip;

				std::transform (Perm.begin (), Perm.end (), PossibleEights.begin (), std::back_inserter (Zip),
				                [](auto const& P, auto const& C) {return std::make_pair (P, C); });


				const auto ZipIt = std::remove_if (Zip.begin (), Zip.end (),
				                             [](auto const& Z) { return !Z.first; });
				
				std::vector<playing_cards::Card> Comb;
				
				std::transform (Zip.begin (), ZipIt, std::back_inserter (Comb),
				                [](auto const& Z) { return Z.second; });

				Combinations.push_back (Comb);

			} while (std::next_permutation (Perm.begin (), Perm.end ()));
		}

	}
}

void Player::find_possible_matching_rank_moves(playing_cards::Rank PromptedRank,
                                               std::vector<std::vector<playing_cards::Card>>& Combinations,
                                               std::vector<playing_cards::Card> PossibleMoves)
{
	std::vector<playing_cards::Card> PossibleMatchingRanks;

	std::remove_copy_if (PossibleMoves.begin (), PossibleMoves.end (), std::back_inserter (PossibleMatchingRanks),
	                     [&PromptedRank](auto const& C) { return C.first != PromptedRank; });

	if (!PossibleMatchingRanks.empty ())
	{
		const auto N = PossibleMatchingRanks.size ();

		for (auto R = N; R > 0; --R)
		{
			std::vector<int> Perm (N);
			std::iota (Perm.end () - R, Perm.end (), 1);

			do {
				std::vector<std::pair<int, playing_cards::Card>> Zip;

				std::transform (Perm.begin (), Perm.end (), PossibleMatchingRanks.begin (), std::back_inserter (Zip),
				                [](auto const& P, auto const& C) {return std::make_pair (P, C); });


				const auto ZipIt = std::remove_if (Zip.begin (), Zip.end (),
				                                   [](auto const& Z) { return !Z.first; });

				std::vector<playing_cards::Card> Comb;

				std::transform (Zip.begin (), ZipIt, std::back_inserter (Comb),
				                [](auto const& Z) { return Z.second; });

				Combinations.push_back (Comb);

			} while (std::next_permutation (Perm.begin (), Perm.end ()));
		}

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
			const auto N = OtherPossibleRanksMatchingSuit.size ();

			for (auto R = static_cast<int>(N); R >= 0; --R)
			{
				std::vector<int> Perm (N);
				std::iota (Perm.end () - R, Perm.end (), 1);

				do {
					std::vector<std::pair<int, playing_cards::Card>> Zip;

					std::transform (Perm.begin (), Perm.end (), OtherPossibleRanksMatchingSuit.begin (), std::back_inserter (Zip),
					                [](auto const& P, auto const& C) {return std::make_pair (P, C); });


					const auto ZipIt = std::remove_if (Zip.begin (), Zip.end (),
					                             [](auto const& Z) { return !Z.first; });

					std::vector<playing_cards::Card> Comb;

					Comb.push_back (SuitCard);

					std::transform (Zip.begin (), ZipIt, std::back_inserter (Comb),
					                [](auto const& Z) { return Z.second; });

					Combinations.push_back (Comb);

				} while (std::next_permutation (Perm.begin (), Perm.end ()));
			}

		}
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
