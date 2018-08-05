#pragma once
#include "Deck.h"

class Command
{
public:
	Command () = default;
	virtual ~Command () = default;
	virtual void execute (Player& PlayerUse) = 0;
};

class UndoableCommand : public Command
{
public:
	UndoableCommand () = default;
	virtual  ~UndoableCommand () = default;
	virtual void undo (Player& PlayerUse) = 0;

};


class PickupACard : public UndoableCommand
{
	using Deck = playing_cards::Deck;
	using Card = playing_cards::Card;

	const int NumCardsToPickup;
	Deck& ThisDeck;
	Deck& ThisDiscard;

	std::vector<Card> OrderedCardsDealt {};
	std::unique_ptr<Player const> OrigPlayer {};
	std::unique_ptr<Deck const> OrigDeck {};
	std::unique_ptr<Deck const> OrigDiscard {};


public:
	PickupACard (Deck& DeckUse, Deck& DiscardUse, int NumCards);
	void execute (Player& PlayerUse) override;
	void undo (Player& PlayerUse) override;
};

