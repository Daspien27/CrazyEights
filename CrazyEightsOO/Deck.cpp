#include "stdafx.h"
#include "Deck.h"
#include "Player.h"

namespace playing_cards
{
	void Deck::build_standard_deck ()
	{
		Suit Suit;

		const auto AscGen = [N = Rank::ACE, &Suit] () mutable
		{
			auto R = N;
			N = static_cast<Rank> ((static_cast<int> (N) + 1) % (NUM_CARDS_PER_SUIT + 1));
			N = (static_cast<int> (N) == 0) ? Rank::KING : N;
			return std::make_pair (R, Suit);
		};

		const auto DescGen = [N = Rank::KING, &Suit] () mutable
		{
			auto R = N;
			N = static_cast<Rank> ((static_cast<int> (N) - 1) % (NUM_CARDS_PER_SUIT + 1));
			N = (static_cast<int> (N) == 0) ? Rank::KING : N;
			return std::make_pair (R, Suit);
		};

		Suit = Suit::HEART;
		std::generate (Cards.begin (), Cards.begin () + NUM_CARDS_PER_SUIT, AscGen); //A-K Heart

		Suit = Suit::CLUB;
		std::generate (Cards.begin () + NUM_CARDS_PER_SUIT, Cards.begin () + 2 * NUM_CARDS_PER_SUIT, AscGen); //A-K CLUB

		Suit = Suit::DIAMOND;
		std::generate (Cards.begin () + 2 * NUM_CARDS_PER_SUIT, Cards.begin () + 3 * NUM_CARDS_PER_SUIT, DescGen);
		//K-A DIAMOND

		Suit = Suit::SPADE;
		std::generate (Cards.begin () + 3 * NUM_CARDS_PER_SUIT, Cards.end (), DescGen); //K-A SPADE
	}

	void Deck::build_sorted_deck ()
	{
		Suit Suit;
		const auto SGen = [N = Rank::KING, &Suit] () mutable
		{
			auto R = N;
			N = static_cast<Rank> (static_cast<int> (N) - 1);

			return std::make_pair (R, Suit);
		};

		Suit = Suit::SPADE;
		std::generate (Cards.begin (), Cards.begin () + NUM_CARDS_PER_SUIT, SGen); //A-K SPADE

		Suit = Suit::DIAMOND;
		std::generate (Cards.begin () + NUM_CARDS_PER_SUIT, Cards.begin () + 2 * NUM_CARDS_PER_SUIT,
		               SGen); //A-K DIAMOND

		Suit = Suit::CLUB;
		std::generate (Cards.begin () + 2 * NUM_CARDS_PER_SUIT, Cards.begin () + 3 * NUM_CARDS_PER_SUIT,
		               SGen); //A-K CLUB

		Suit = Suit::HEART;
		std::generate (Cards.begin () + 3 * NUM_CARDS_PER_SUIT, Cards.end (), SGen); //A-K Heart
	}

	void Deck::shuffle_deck ()
	{
		std::random_device Rd;
		std::mt19937 G (Rd ());

		std::shuffle (Cards.begin (), Cards.end (), G);
	}

	void Deck::build_shuffled_deck ()
	{
		build_sorted_deck ();

		shuffle_deck ();
	}

	Deck::Deck (const DeckOrder Order, const bool IncludeJokers) :
		Cards (NUM_PLAYING_CARDS)
	{
		switch (Order)
		{
		case DeckOrder::STANDARD:

			build_standard_deck ();

			break;

		case DeckOrder::SORTED:

			build_sorted_deck ();

			break;

		case DeckOrder::SHUFFLED:

			build_shuffled_deck ();

			break;

		case DeckOrder::EMPTY:

			Cards.clear ();

			break;

		default:

			throw std::runtime_error ("Unrecognized deck order.");
		}

		if (IncludeJokers)
		{
			Cards.emplace (Cards.begin (), Rank::JOKER, Suit::NO_SUIT);
			Cards.emplace_back (Rank::JOKER, Suit::NO_SUIT);

			if (Order == DeckOrder::SHUFFLED) shuffle_deck ();
		}
	}

	void Deck::deal (unsigned int NumCards, std::vector<Player>& Players)
	{
		if (NumCards > Cards.size ()) throw std::runtime_error ("Not enough cards in deck to deal that many cards.");

		for (auto PIter = Players.begin (); NumCards > 0; ++PIter, --NumCards)
		{
			if (PIter == Players.end ()) PIter = Players.begin ();

			const auto TopCard = pick_up_from_deck ();

			PIter->place_in_hand (TopCard);
		}
	}

	void Deck::deal_n_to_each (unsigned int NumCards, std::vector<Player>& Players)
	{
		if (NumCards * Players.size () > Cards.size ())
			throw std::runtime_error (
				"Not enough cards in deck to deal that many cards.");

		for (unsigned int I = 0; I < NumCards && !Cards.empty (); ++I)
		{
			for (auto& P : Players)
			{
				const auto TopCard = pick_up_from_deck ();

				P.place_in_hand (TopCard);
			}
		}
	}

	Card Deck::pick_up_from_deck ()
	{
		if (Cards.empty ()) throw std::runtime_error ("No cards in deck remaining. Cannot pick up.");

		auto Card = Cards.back ();
		Cards.pop_back ();

		return Card;
	}

	void Deck::place_card_into_deck (Card card)
	{
		Cards.push_back (card);
	}

	void Deck::shuffle_into_this_deck (Deck const& OtherDeck)
	{
		Cards.insert (Cards.end (), OtherDeck.Cards.begin (), OtherDeck.Cards.end ());
		shuffle_deck ();
	}

	void Deck::insert_deck_into_bottom_of_this_deck (Deck const& OtherDeck)
	{
		Cards.insert (Cards.begin (), OtherDeck.Cards.begin (), OtherDeck.Cards.end ());
	}

	std::optional<Card> Deck::peek_top_card ()
	{
		if (Cards.empty ()) return std::nullopt;

		return std::make_optional (Cards.back ());
	}

	std::string to_string (Card const& Card)
	{
		return to_string (Card.first) + to_string (Card.second);
	}

	std::string to_string (Rank const& Rank)
	{
		switch (Rank)
		{
		case Rank::ACE: return " A";
		case Rank::TWO:
		case Rank::THREE:
		case Rank::FOUR:
		case Rank::FIVE:
		case Rank::SIX:
		case Rank::SEVEN:
		case Rank::EIGHT:
		case Rank::NINE:

			return " " + std::to_string (static_cast<int> (Rank));

		case Rank::TEN:

			return "10";

		case Rank::JACK:

			return " J";

		case Rank::QUEEN:

			return " Q";

		case Rank::KING:

			return " K";

		case Rank::JOKER:

			return " W";

		default:

			throw std::runtime_error ("Unrecognized card rank.");
		}
	}

	std::string to_string (Suit const& Suit)
	{
		return {static_cast<char> (Suit)};
	}

	std::string suit_full_name (Suit const& Suit)
	{
		switch (Suit)
		{
		case Suit::CLUB:

			return "club";

		case Suit::DIAMOND:

			return "diamond";
		case Suit::HEART:

			return "heart";
		case Suit::SPADE:

			return "spade";
		case Suit::NO_SUIT:

			return "no suit";
		default:

			throw std::runtime_error ("Unrecognized suit");
		}
	}

	Suit string_to_suit (std::string const& SuitStr)
	{
		auto LowercaseStr = SuitStr;
		std::transform (LowercaseStr.begin (), LowercaseStr.end (), LowercaseStr.begin (), tolower);

		if (LowercaseStr == "club") return Suit::CLUB;
		if (LowercaseStr == "diamond") return Suit::DIAMOND;
		if (LowercaseStr == "heart") return Suit::HEART;
		if (LowercaseStr == "spade") return Suit::SPADE;

		return Suit::NO_SUIT;
	}
};
