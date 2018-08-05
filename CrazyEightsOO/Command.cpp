#include "stdafx.h"
#include "Command.h"

#include "Player.h"

PickupACard::PickupACard (Deck& DeckUse, Deck& DiscardUse, const int NumCards) : 
	NumCardsToPickup (NumCards), 
	ThisDeck (DeckUse),
	ThisDiscard (DiscardUse),
	OrderedCardsDealt (NumCards),
	OrigDeck (std::make_unique<Deck const> (DeckUse)),
	OrigDiscard (std::make_unique<Deck const> (DiscardUse))
{
}

void PickupACard::execute (Player& PlayerUse)
{
	OrigPlayer = std::make_unique<Player const> (PlayerUse);

	if (ThisDeck.num_remaining_cards () >= NumCardsToPickup)
	{
		OrderedCardsDealt = ThisDeck.pick_up_n_from_deck (NumCardsToPickup);

		PlayerUse.place_in_hand (OrderedCardsDealt);
	}
	else
	{
		auto CardsLeftToPickup = NumCardsToPickup - ThisDeck.num_remaining_cards ();

		OrderedCardsDealt = ThisDeck.pick_up_all_from_deck ();

		crazy_eights::shuffle_discard_into_deck (ThisDeck, ThisDiscard);

		if (ThisDeck.num_remaining_cards () < CardsLeftToPickup)
		{
			CardsLeftToPickup -= ThisDeck.num_remaining_cards ();

			auto RemainingCards = ThisDeck.pick_up_all_from_deck ();

			OrderedCardsDealt.insert (OrderedCardsDealt.end (), RemainingCards.begin (), RemainingCards.end ());

			PlayerUse.place_in_hand (OrderedCardsDealt);

			throw std::runtime_error ("Deck does not have enough cards left to pick up.");
		}

		auto RemainingCards = ThisDeck.pick_up_n_from_deck (CardsLeftToPickup);

		OrderedCardsDealt.insert (OrderedCardsDealt.end (), RemainingCards.begin (), RemainingCards.end ());

		PlayerUse.place_in_hand (OrderedCardsDealt);
	}
}

void PickupACard::undo (Player& PlayerUse)
{
	PlayerUse = *OrigPlayer;
	ThisDeck = *OrigDeck;
	ThisDiscard = *OrigDiscard;
}
