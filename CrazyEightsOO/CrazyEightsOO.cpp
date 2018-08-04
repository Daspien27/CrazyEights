// CrazyEightsOO.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "MainGame.h"

int main()
{
	try
	{
		MainGame mg ({"Amund", "Steph"});

		mg.run ();
	}
	catch (std::exception& e)
	{
		std::cout << e.what () << std::endl;
	}

    return 0;
}

