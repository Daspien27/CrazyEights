#include "stdafx.h"
#include "Deck.h"
#include "Player.h"

namespace PlayingCards {

	void Deck::build_standard_deck ()
	{
		Suit suit;

		auto AscGen = [n = Rank::ACE, &suit] () mutable
		{
			auto r = n;
			n = static_cast<Rank> ((static_cast<int> (n) + 1) % (NUM_CARDS_PER_SUIT + 1));
			n = (static_cast<int> (n) == 0) ? Rank::KING : n;
			return std::make_pair (r, suit);
		};

		auto DescGen = [n = Rank::KING, &suit] () mutable
		{
			auto r = n;
			n = static_cast<Rank> ((static_cast<int> (n) - 1) % (NUM_CARDS_PER_SUIT + 1));
			n = (static_cast<int> (n) == 0) ? Rank::KING : n;
			return std::make_pair (r, suit);
		};

		suit = Suit::HEART;
		std::generate (cards.begin (), cards.begin () + NUM_CARDS_PER_SUIT, AscGen); //A-K Heart

		suit = Suit::CLUB;
		std::generate (cards.begin () + NUM_CARDS_PER_SUIT, cards.begin () + 2 * NUM_CARDS_PER_SUIT, AscGen); //A-K CLUB

		suit = Suit::DIAMOND;
		std::generate (cards.begin () + 2 * NUM_CARDS_PER_SUIT, cards.begin () + 3 * NUM_CARDS_PER_SUIT, DescGen); //K-A DIAMOND

		suit = Suit::SPADE;
		std::generate (cards.begin () + 3 * NUM_CARDS_PER_SUIT, cards.end (), DescGen); //K-A SPADE
	}

	void Deck::build_sorted_deck ()
	{
		Suit suit;
		auto SGen = [n = Rank::KING, &suit] () mutable
		{
			auto r = n;
			n = static_cast<Rank> (static_cast<int> (n) - 1);

			return std::make_pair (r, suit);
		};

		suit = Suit::SPADE;
		std::generate (cards.begin (), cards.begin () + NUM_CARDS_PER_SUIT, SGen); //A-K SPADE

		suit = Suit::DIAMOND;
		std::generate (cards.begin () + NUM_CARDS_PER_SUIT, cards.begin () + 2 * NUM_CARDS_PER_SUIT, SGen); //A-K DIAMOND

		suit = Suit::CLUB;
		std::generate (cards.begin () + 2 * NUM_CARDS_PER_SUIT, cards.begin () + 3 * NUM_CARDS_PER_SUIT, SGen); //A-K CLUB

		suit = Suit::HEART;
		std::generate (cards.begin () + 3 * NUM_CARDS_PER_SUIT, cards.end (), SGen); //A-K Heart
	}

	void Deck::shuffle_deck ()
	{
		std::random_device rd;
		std::mt19937 g (rd ());

		std::shuffle (cards.begin (), cards.end (), g);
	}

	void Deck::build_shuffled_deck ()
	{
		build_sorted_deck ();

		shuffle_deck ();
	}

	Deck::Deck (DeckOrder order, bool IncludeJokers) :
		cards (NUM_PLAYING_CARDS)
	{
		switch (order)
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

			cards.clear ();

			break;

		default:

			throw std::runtime_error ("Unrecognized deck order.");

			break;
		}

		if (IncludeJokers)
		{
			cards.emplace (cards.begin (), Rank::JOKER, Suit::NO_SUIT);
			cards.emplace_back (Rank::JOKER, Suit::NO_SUIT);

			if (order == DeckOrder::SHUFFLED) shuffle_deck ();
		}

	}

	Deck::~Deck ()
	{
	}

	void Deck::deal (unsigned int NumCards, std::vector<Player>& Players)
	{
		if (NumCards > cards.size ()) throw std::runtime_error ("Not enough cards in deck to deal that many cards.");

		for (auto piter = Players.begin (); NumCards > 0; piter++, --NumCards)
		{
			if (piter == Players.end ()) piter = Players.begin ();

			auto card = pick_up_from_deck ();

			piter->place_in_hand (card);
		}

	}

	void Deck::deal_n_to_each (unsigned int NumCards, std::vector<Player>& Players)
	{
		if (NumCards * Players.size () > cards.size ()) throw std::runtime_error ("Not enough cards in deck to deal that many cards.");

		for (unsigned int i = 0; i < NumCards && !cards.empty (); ++i)
		{
			for (auto& p : Players)
			{
					auto card = pick_up_from_deck ();

					p.place_in_hand (card);
			}
		}
	}

	Card Deck::pick_up_from_deck ()
	{
		if (cards.empty ()) throw std::runtime_error ("No cards in deck remaining. Cannot pick up.");

		auto Card = cards.back ();
		cards.pop_back ();

		return Card;
	}

	void Deck::place_card_into_deck (Card card)
	{
		cards.push_back (card);
	}

	void Deck::shuffle_into_this_deck (Deck const & other_deck)
	{
		cards.insert (cards.end (), other_deck.cards.begin (), other_deck.cards.end ());
		shuffle_deck ();
	}

	void Deck::insert_deck_into_bottom_of_this_deck (Deck const& other_deck)
	{
		cards.insert (cards.begin (), other_deck.cards.begin (), other_deck.cards.end ());
	}

	std::optional<Card> Deck::peek_top_card ()
	{
		if (cards.empty ()) return std::nullopt;

		return std::make_optional (cards.back ());
	}

	std::string to_string (Card const & card)
	{
		return to_string (card.first) + to_string(card.second);
	}

	std::string to_string (Rank const & rank)
	{
		switch (rank)
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

			return " " + std::to_string (static_cast<int>(rank));

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

		return "Err";
	}

	std::string to_string (Suit const & suit)
	{
		return { static_cast<char> (suit) };
	}
	std::string suit_full_name (Suit const & suit)
	{
		switch (suit)
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

			break;
		}
		return "Err";
	}
	Suit string_to_suit (std::string const & SuitStr)
	{
		std::string lc_str = SuitStr;
		std::transform (lc_str.begin (), lc_str.end (), lc_str.begin (), ::tolower);

		if (lc_str == "club") return Suit::CLUB;
		if (lc_str == "diamond") return Suit::DIAMOND;
		if (lc_str == "heart") return Suit::HEART;
		if (lc_str == "spade") return Suit::SPADE;
		
		return Suit::NO_SUIT;
	}
};