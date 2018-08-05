#pragma once

namespace playing_cards
{
	class Deck;
}

namespace crazy_eights
{
	void shuffle_discard_into_deck (playing_cards::Deck& DeckUse, playing_cards::Deck& Discard);

}