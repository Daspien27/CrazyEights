#pragma once

#include "Deck.h"

class Player
{
	std::string Name{};
	std::vector<playing_cards::Card> Hand{};

public:

	Player () = default;

	explicit Player (const std::string& NameUse);

	void find_possible_eight_moves(std::vector<std::vector<playing_cards::Card>>& Combinations,
	                               std::vector<playing_cards::Card> PossibleMoves);
	void find_possible_matching_rank_moves(playing_cards::Rank PromptedRank,
	                                       std::vector<std::vector<playing_cards::Card>>& Combinations,
	                                       std::vector<playing_cards::Card> PossibleMoves);
	void find_possible_matching_suit_moves(playing_cards::Rank PromptedRank,
	                                       std::vector<std::vector<playing_cards::Card>>& Combinations,
	                                       std::vector<playing_cards::Card> PossibleMoves);

	void prompt_action (playing_cards::Rank PromptedRank, playing_cards::Suit PromptedSuit);

	const std::string& get_name () const;
	std::string display_hand () const;

	void place_in_hand (playing_cards::Card CardForHand);
};

