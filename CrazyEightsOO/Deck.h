#pragma once

class Player;

namespace PlayingCards {

	const int NUM_PLAYING_CARDS = 52;
	const int NUM_PLAYING_CARDS_W_JOKERS = 54;
	const int NUM_CARDS_PER_SUIT = 13;
	const int NUM_SUITS = 4;

	enum class Suit : char
	{
		HEART = '\3',
		CLUB = '\5',
		DIAMOND = '\4',
		SPADE = '\6',

		NO_SUIT = 'J'
	};

	enum class Rank : int
	{
		ACE = 1,
		TWO = 2,
		THREE = 3,
		FOUR = 4,
		FIVE = 5,
		SIX = 6,
		SEVEN = 7,
		EIGHT = 8,
		NINE = 9,
		TEN = 10,
		JACK = 11,
		QUEEN = 12,
		KING = 13,

		JOKER = 99
	};

	enum class DeckOrder
	{
		STANDARD, //A-K Spades, A-K Diamond, K-A Clubs, K-A Hearts
		SORTED,
		SHUFFLED,
		EMPTY,
	};

	using Card = std::pair<Rank, Suit>;

	class Deck
	{
		std::vector<Card> cards;

		void build_standard_deck ();
		void build_sorted_deck ();
		void build_shuffled_deck ();
		

	public:
		Deck (DeckOrder order = DeckOrder::STANDARD, bool IncludeJokers = false);
		~Deck ();

		void deal (unsigned int NumCards, std::vector<Player>& Players); 
		void deal_n_to_each (unsigned int NumCards, std::vector<Player>& Players);

		Card pick_up_from_deck ();

		void place_card_into_deck (Card card);

		void shuffle_deck ();
		void shuffle_into_this_deck (Deck const& other_deck);
		void insert_deck_into_bottom_of_this_deck (Deck const& other_deck);
		
		std::optional<Card> peek_top_card ();
	};


	std::string to_string (Card const& card);
	std::string to_string (Rank const & rank);
	std::string to_string (Suit const & suit);
	std::string suit_full_name (Suit const& suit);
	Suit string_to_suit (std::string const& SuitStr);

};