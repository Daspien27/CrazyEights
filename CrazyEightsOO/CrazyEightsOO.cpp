// CrazyEightsOO.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "MainGame.h"

int main ()
{
	try
	{
		MainGame Mg ({"Amund", "Steph"});

		Mg.run ();
	}
	catch (std::exception& E)
	{
		std::cout << E.what () << std::endl;
	}

	return 0;
}
