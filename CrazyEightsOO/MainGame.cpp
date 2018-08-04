#include "stdafx.h"
#include "MainGame.h"
#include "Deck.h"

std::vector<Player> MainGame::get_n_player_names (int N)
{
	std::vector<Player> Players (N);

	std::cout << "Please provide your players' names." << std::endl;

	std::cin.ignore (std::numeric_limits<std::streamsize>::max (), '\n');

	auto Gen = []() {
		std::cout << "Name:\t";
		std::string PlayerName;
		std::getline (std::cin, PlayerName);
		std::cout << "You have chosen:\t" << PlayerName << std::endl;
		return Player (PlayerName);
	};

	std::generate (Players.begin (), Players.end (), Gen);

	return Players;
}


std::vector<Player> MainGame::PromptUserForPlayerList ()
{
	int PlayerCount = get_player_count ();

	std::vector<Player> AllPlayers = get_n_player_names (PlayerCount);

	std::cout << "The players in your crazy eights game are:" << std::endl;

	print_player_list (AllPlayers);

	return AllPlayers;
}

std::string MainGame::display_discard_chain ()
{
	auto top_card = Discard.peek_top_card ();

	if (top_card == std::nullopt) return "Empty";

	return PlayingCards::to_string (*top_card);
}

MainGame::MainGame () :
	Players (PromptUserForPlayerList ()),
	ActivePlayerNum (0),
	Deck (PlayingCards::DeckOrder::SHUFFLED),
	Discard (PlayingCards::DeckOrder::EMPTY)
{
	init ();
}

MainGame::MainGame (std::vector<std::string> PlayerNames) :
	Players (PlayerNames.begin (), PlayerNames.end ()),
	ActivePlayerNum (0),
	Deck (PlayingCards::DeckOrder::SHUFFLED),
	Discard (PlayingCards::DeckOrder::EMPTY)
{
	std::vector<Player> players (PlayerNames.begin (), PlayerNames.end ());

	init ();
}


MainGame::~MainGame ()
{
}

void print_player_list (std::vector<Player> &AllPlayers)
{
	auto PrintNameWTab = [](auto const& a) { std::cout << "\t" << a.get_name () << std::endl; };

	std::for_each (AllPlayers.begin (), AllPlayers.end (), PrintNameWTab);
}

int MainGame::get_player_count ()
{
	int PlayerCount = 0;

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
	std::random_device rd;
	std::mt19937 g (rd ());

	std::shuffle (AllPlayers.begin (), AllPlayers.end (), g);

	std::cout << "The shuffled turn order will be:" << std::endl;

	print_player_list (AllPlayers);
}

void MainGame::init ()
{
	shuffle_players (Players);

	ActivePlayerNum = 0;
}


void MainGame::run ()
{	
	//Game loop
	using PlayingCards::Suit;

	std::cout << static_cast<char>(Suit::HEART) << static_cast<char>(Suit::DIAMOND) 
		<< static_cast<char>(Suit::SPADE) << static_cast<char>(Suit::CLUB) << std::endl;


	Deck.deal_n_to_each (8, Players);
	
	auto card = Deck.pick_up_from_deck ();
	Discard.place_card_into_deck (card);

	CurrentSuit = card.second;

	if (card.first == PlayingCards::Rank::EIGHT)
	{
		std::cout << Players[ActivePlayerNum].get_name () << ", the starting card is an eight. Which suit would you like to start the game off with?" << std::endl;

		std::cout << "Your hand is : \t" << Players[ActivePlayerNum].display_hand () << std::endl;

		std::cout << "Suit:\t";

		PlayingCards::Suit PlayersChoice;
		std::string choice;

		std::cin.ignore (std::numeric_limits<std::streamsize>::max (), '\n');

		std::getline (std::cin, choice);
		
		PlayersChoice = PlayingCards::string_to_suit (choice);

		while (PlayersChoice == PlayingCards::Suit::NO_SUIT)
		{
			std::cout << "Sorry, that suit is unrecognized. Please try entering a valid suit (spade, heart, club, diamond). Case does not matter." << std::endl;

			std::cout << "Suit:\t";

			std::getline (std::cin, choice);

			PlayersChoice = PlayingCards::string_to_suit (choice);
		}


		CurrentSuit = PlayersChoice;
	}

	while (!GameShouldStopRunning)
	{
		auto& ActivePlayer = Players[ActivePlayerNum];
		
		std::cout << "It is " << ActivePlayer.get_name() << "'s turn. Your hand is:\t" << ActivePlayer.display_hand () << std::endl;

		std::cout << "The top chain to play on is:\t" << display_discard_chain () << std::endl;
		std::cout << "The current suit is:\t" << PlayingCards::suit_full_name (CurrentSuit) << std::endl;
		ActivePlayer.prompt_action ();

		++ActivePlayerNum %= Players.size ();


	}
}
