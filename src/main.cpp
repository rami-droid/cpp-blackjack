#include <raylib.h>

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#include <iostream>
#include <sstream>
#include <vector>

//#include "lighting.h"

struct Card
{
	int suit;
	int score;
	Texture2D texture;
};

struct Player{
	int points;
	bool isWin;
	bool isPlaying;
};
//global variables
std::string suits[4] = {"Hearts", "Diamonds", "Clubs", "Spades"};
Player playerOne;

std::vector<Card> cards;
std::vector<Card> dealerCards;

//Light testLight;







Card selectCard() {
	int score = (rand() % 13) + 1;
	int suitIndex = rand()%4;

	std::stringstream ss;

	ss << suits[suitIndex] << " " << score << ".png";

	Image tempImg = LoadImage(ss.str().c_str());
	ImageResizeNN(&tempImg, 256, 384);

	Texture2D texture = LoadTextureFromImage(tempImg);
	SetTextureFilter(texture, TEXTURE_FILTER_POINT);

	UnloadImage(tempImg);

	return {suitIndex, score, texture};
}

struct dealerPlayer{
	int points;
	bool isWin;
	bool isPlaying;

	void dealerHit() {
		if (points < 17 && isPlaying == true) {
			dealerCards.push_back(selectCard());
			if (dealerCards[dealerCards.size()-1].score >= 10)
			{
				points += 10;
			} else {
				points += dealerCards[dealerCards.size()-1].score;
			}
			
			
		} else 
		{
			dealerStand();
		}
		playerOne.isPlaying = true;
		isPlaying = false;
	};

	void dealerStand() {
		if ( points > 21 || (21 - playerOne.points) < (21 - points))
		{
			isWin = false;
			playerOne.isWin = true;
		}
		
	};
};
dealerPlayer dealer;

int main ()
{


	srand(time(0));

	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(1280, 800, "Blackjack game");
	InitAudioDevice();

	/*testLight.position = {400, 400};
	testLight.color = {255, 255, 255, 255};*/


	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	Sound hit = LoadSound("sfx/hit.wav");

	Sound stand = LoadSound("sfx/stand.wav");

	Sound reset = LoadSound("sfx/reset.wav");

	Sound mainTheme = LoadSound("sfx/main_theme.mp3");

	

	PlaySound(mainTheme);


	Texture2D texture = selectCard().texture;

	cards.push_back(selectCard());
	playerOne.points += cards[0].score;

	Rectangle hitRec = { GetScreenWidth()/12.0f, 600, GetScreenWidth()/4.0f, 128};
	Rectangle standRec = { GetScreenWidth()/12.0f *8.0f, 600, GetScreenWidth()/4.0f, 128};

	Rectangle resetRect = {GetScreenWidth()/2 - GetScreenWidth()/8.0f, 500, GetScreenWidth()/4.0f, 128};

	Vector2 cardPos = {200, 200};
	const float cardSpacing = 192.0f;

	//init stuff
	playerOne.isWin = false;
	playerOne.isPlaying = true;
	
	
	

	SetTargetFPS(144);

	// game loop
	while (!WindowShouldClose())
	{

		if (!IsSoundPlaying(mainTheme))
		{
			PlaySound(mainTheme);
		}
		

			//hit button logic
		if(CheckCollisionPointRec(GetMousePosition(), hitRec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			
			PlaySound(hit);
			if(playerOne.isPlaying == true) {
				cards.push_back(selectCard());
				if (cards[cards.size()-1].score >= 10)
				{
					playerOne.points += 10;
				} else {
					playerOne.points += cards[cards.size()-1].score;
				}

				//toggle turns
			playerOne.isPlaying = false;
			dealer.isPlaying = true;
			dealer.dealerHit();
		}
			if (playerOne.points > 21)
			{
				playerOne.isWin = false;
				playerOne.isPlaying = false;
			}
			
		}

			//stand button logic
		if(CheckCollisionPointRec(GetMousePosition(), standRec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			PlaySound(stand);
			dealer.dealerHit();
			playerOne.isPlaying = false;
		}

			//reset button logic



		
		
		

		// drawing
		BeginDrawing();
		ClearBackground(DARKGREEN);

		//testLight.draw();

		//points drawing
		DrawText(TextFormat("your score: %i", playerOne.points), 5, 50, 80, WHITE);
		DrawText(TextFormat("dealer score: %i", dealer.points), GetScreenWidth()/5 * 4 - MeasureText(TextFormat("score: %i", playerOne.points), 100), 50, 80, WHITE);


		//buttons drawing
		DrawRectangleRec(hitRec,BLUE);
		DrawText("HIT",hitRec.x + hitRec.width/2 - MeasureText("HIT", 20), hitRec.y + hitRec.height/2 - 20, 40, WHITE);

		DrawRectangleRec(standRec,RED);
		DrawText("STAND",standRec.x + standRec.width/2 - MeasureText("STAND", 20), standRec.y + standRec.height/2 - 20, 40, WHITE);
		

		for (int i = 0; i < cards.size(); i++)
		{
			DrawTexture(cards[i].texture, cardPos.x + cardSpacing*i, cardPos.y, WHITE);
		}
		
		if (playerOne.isWin == false && playerOne.isPlaying == false) 
		{
			DrawRectangle (0, 0, GetScreenWidth(), GetScreenHeight(), Color{0, 0, 0, 128});

			DrawText("You Lost...", GetScreenWidth()/2 - MeasureText("You Lost...", 80), GetScreenHeight()/2 - 80, 160, WHITE);

			DrawRectangleRec(resetRect,ORANGE);
			DrawText("AGAIN",resetRect.x + resetRect.width/2 - MeasureText("AGAIN", 20), resetRect.y + resetRect.height/2 - 20, 40, WHITE);

		} else if (playerOne.isWin == true && playerOne.isPlaying == false) {
			DrawRectangle (0, 0, GetScreenWidth(), GetScreenHeight(), Color{0, 0, 0, 128});
			DrawText("You Won!", GetScreenWidth()/2 - MeasureText("You  Won!", 80), GetScreenHeight()/2 - 80, 160, WHITE);

			DrawRectangleRec(resetRect,ORANGE);
			DrawText("AGAIN",resetRect.x + resetRect.width/2 - MeasureText("AGAIN", 20), resetRect.y + resetRect.height/2 - 20, 40, WHITE);
		}

		if(CheckCollisionPointRec(GetMousePosition(), resetRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			PlaySound(reset);
			cards = {};
			dealerCards = {};
			playerOne.isWin = false;
			playerOne.isPlaying = true;
			dealer.points = 0;
			playerOne.points = 0;
			dealer.isPlaying = false;
			DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), DARKGREEN);

		}

		DrawCircleGradient(400, 400, 100.0f, {255, 255, 255, 255}, {255, 255, 255, 0});
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	for (int i = 0; i < cards.size(); i++)
	{
		UnloadTexture(cards[i].texture);
	}
	

	// destroy the window and cleanup the OpenGL context
	CloseAudioDevice();
	CloseWindow();
	return 0;
}