#include "stdafx.h"
#include "MainGame.h"
#include "Deck.h"

std::vector<Player> MainGame::get_n_player_names (int N)
{
	std::vector<Player> Players (N);

	std::cout << "Please provide your players' names." << std::endl;

	std::cin.ignore (std::numeric_limits<std::streamsize>::max (), '\n');

	std::generate (Players.begin (), Players.end (), 
		[]() {
		std::cout << "Name:\t";
		std::string PlayerName;
		std::getline (std::cin, PlayerName);
		std::cout << "You have chosen:\t" << PlayerName << std::endl;
		return Player (PlayerName);
	});

	return Players;
}


std::vector<Player> MainGame::prompt_user_for_player_list ()
{
	const auto PlayerCount = get_player_count ();

	auto AllPlayers = get_n_player_names (PlayerCount);

	std::cout << "The players in your crazy eights game are:" << std::endl;

	print_player_list (AllPlayers);

	return AllPlayers;
}

std::string MainGame::display_discard_chain ()
{
	auto TopCard = Discard.peek_top_card ();

	if (TopCard == std::nullopt) return "Empty";

	return playing_cards::to_string (*TopCard);
}

MainGame::MainGame () :
	Players (prompt_user_for_player_list ()),
	Deck (playing_cards::DeckOrder::SHUFFLED),
	Discard (playing_cards::DeckOrder::EMPTY)
{
	init ();
}

MainGame::MainGame (std::vector<std::string> PlayerNames) :
	Players (PlayerNames.begin (), PlayerNames.end ()),
	Deck (playing_cards::DeckOrder::SHUFFLED),
	Discard (playing_cards::DeckOrder::EMPTY)
{
	init ();
}

void print_player_list (std::vector<Player> &AllPlayers)
{
	std::for_each (AllPlayers.begin (), AllPlayers.end (), 
		[](auto const& A) { std::cout << "\t" << A.get_name () << std::endl; });
}

int MainGame::get_player_count ()
{
	auto PlayerCount = 0;

	std::cout << "How many players would like to play crazy eights?" << std::endl;

	std::cin >> PlayerCount;

	while (PlayerCount < 2)
	{
		std::cout << "The number of players must be at least 2." << std::endl;

		std::cin >> PlayerCount;
	}

	return PlayerCount;
}

void MainGame::shuffle_players (std::vector<Player> &AllPlayers)
{
	std::random_device Rd;
	std::mt19937 G (Rd ());

	std::shuffle (AllPlayers.begin (), AllPlayers.end (), G);

	std::cout << "The shuffled turn order will be:" << std::endl;

	print_player_list (AllPlayers);
}

void MainGame::init ()
{
	shuffle_players (Players);

	Deck = playing_cards::Deck (playing_cards::DeckOrder::SHUFFLED);
	Discard = playing_cards::Deck (playing_cards::DeckOrder::EMPTY);

	ActivePlayerNum = Players.size() - 1;

	Deck.deal_n_to_each (8, Players);

	const auto InitialCard = Deck.pick_up_from_deck ();
	Discard.place_card_into_deck (InitialCard);

	CurrentSuit = InitialCard.second;

	if (InitialCard.first == playing_cards::Rank::EIGHT)
	{
		std::cout << Players[ActivePlayerNum].get_name () << ", you are the dealer and the starting card is an eight. Which suit would you like to start the game off with?" << std::endl;

		CurrentSuit = prompt_player_for_suit (Players[ActivePlayerNum]);
	}
}


void MainGame::run ()
{	
	//Game loop
	while (!GameShouldStopRunning)
	{
		auto& ActivePlayer = Players[ActivePlayerNum];
		
		std::cout << "It is " << ActivePlayer.get_name() << "'s turn. Your hand is:\t" << ActivePlayer.display_hand () << std::endl;

		std::cout << "The top chain to play on is:\t" << display_discard_chain () << std::endl;
		std::cout << "The current suit is:\t" << playing_cards::suit_full_name (CurrentSuit) << std::endl;
		

		auto TopCard = Discard.peek_top_card ();

		if (TopCard == std::nullopt) std::runtime_error ("Missing card indicating play");
		
		ActivePlayer.prompt_action ((*TopCard).first, CurrentSuit);

		++ActivePlayerNum %= Players.size ();
	}
}

playing_cards::Suit MainGame::prompt_player_for_suit (Player& Player) const
{
	std::cout << "Your hand is : \t" << Player.display_hand () << std::endl;
	std::cout << "Suit:\t";

	std::string Choice;
	std::getline (std::cin, Choice);

	auto PlayersChoice = playing_cards::string_to_suit (Choice);

	while (PlayersChoice == playing_cards::Suit::NO_SUIT)
	{
		std::cout << "Sorry, that suit is unrecognized. Please try entering a valid suit (spade, heart, club, diamond). Case does not matter." << std::endl;
		std::cout << "Suit:\t";

		std::getline (std::cin, Choice);

		PlayersChoice = playing_cards::string_to_suit (Choice);
	}

	return PlayersChoice;
}
