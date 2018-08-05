#include "stdafx.h"
#include "CrazyEights.h"
#include "Deck.h"

namespace crazy_eights
{
	void shuffle_discard_into_deck (playing_cards::Deck& DeckUse, playing_cards::Deck& Discard)
	{
		auto const TopCard = Discard.pick_up_from_deck ();

		std::vector<playing_cards::Card> DiscardChain;

		switch (TopCard.first)
		{
		case playing_cards::Rank::QUEEN:

			if (TopCard.second != playing_cards::Suit::SPADE)
			{
				DiscardChain.push_back (TopCard);
			}

			break;

		case playing_cards::Rank::TWO:

			break;

		default:

			DiscardChain.push_back (TopCard);

			break;
		}

		if (DiscardChain.empty ())
		{
			DiscardChain.push_back (TopCard);

			auto const EndOfChain = std::find_if_not (Discard.Cards.rbegin (), Discard.Cards.rend (),
			                                    [] (auto const& C)
			                                    {
				                                    return (C.first == playing_cards::Rank::TWO
					                                    || (C.first == playing_cards::Rank::QUEEN && C.second ==
						                                    playing_cards::Suit::SPADE));
			                                    });

			auto RemainingChain = Discard.pick_up_n_from_deck (std::distance (Discard.Cards.rbegin (), EndOfChain));
			DiscardChain.insert (DiscardChain.end (), RemainingChain.begin (), RemainingChain.end ());
			
		}

		DeckUse.shuffle_into_this_deck (Discard);

		std::reverse (DiscardChain.begin (), DiscardChain.end ());
		Discard.place_cards_into_deck (DiscardChain);
	}
}
