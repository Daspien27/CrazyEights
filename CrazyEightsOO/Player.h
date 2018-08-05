#pragma once

#include "Deck.h"

class Player
{
	std::string Name{};
	std::vector<playing_cards::Card> Hand{};

public:

	Player () = default;

	explicit Player (const std::string& NameUse);

	std::vector<std::vector<playing_cards::Card>> permute_all_combinations(std::vector<playing_cards::Card> PossibleMatchingRanks) const;
	void find_possible_matching_rank_moves(playing_cards::Rank PromptedRank,
	                                       std::vector<std::vector<playing_cards::Card>>& Combinations,
	                                       std::vector<playing_cards::Card> PossibleMoves) const;
	void find_possible_matching_suit_moves(playing_cards::Rank PromptedRank,
	                                       std::vector<std::vector<playing_cards::Card>>& Combinations,
	                                       std::vector<playing_cards::Card> PossibleMoves);
	void find_possible_prompted_rank_moves(playing_cards::Rank PromptedRank,
	                                       std::vector<std::vector<playing_cards::Card>>& Combinations,
	                                       playing_cards::Rank CrazyRank = playing_cards::Rank::EIGHT);
	void find_possible_prompted_suit_moves(playing_cards::Suit PromptedSuit, playing_cards::Rank PromptedRank,
	                                       std::vector<std::vector<playing_cards::Card>>& Combinations);
	void display_combinations(std::vector<std::vector<playing_cards::Card>> const& Combinations);

	void prompt_action (playing_cards::Rank PromptedRank, playing_cards::Suit PromptedSuit);

	const std::string& get_name () const;
	std::string display_hand () const;

	void place_in_hand (playing_cards::Card CardForHand);
};