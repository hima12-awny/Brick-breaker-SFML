#include<iostream>
#include<SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<ctime>

using namespace std;
using namespace sf;

const int rows = 6, col = 11;              // rows and columns of blocks

					   // make window with any size but still it's style 
                                           // is Fullscreen
RenderWindow window(VideoMode(1920, 1080), "hi", Style::Fullscreen);

void DrawBlocks(RectangleShape blks[rows][col])
{
	for (int j = 0; j < rows; j++) {
		for (int i = 0; i < col; i++) 
			window.draw(blks[j][i]);
	}
}

RectangleShape rocket() {
	RectangleShape roc(Vector2f(300, 15));
	roc.setPosition(Vector2f(10, window.getSize().y - 30));
	return roc;
}

CircleShape ball(int position_x, int position_y) {
	CircleShape ball(10.f);
	ball.setFillColor(Color(255, 255, 255));
	ball.setPosition(Vector2f(position_x, position_y));
	ball.setOrigin(15, 15);
	ball.setScale(0, 0);		           // make any ball "in first disappear" 
	return ball;
}

void blokcs(RectangleShape blks[][11]) {
	RectangleShape grec(Vector2f(150, 25));    // geniral RectangleShape to copy
	int y = 100;                               // virtical distance bettewn top row and top of window
	for (int i = 0; i < rows; i++)
	{
		int s = 0;                         // horizontal distance bettewn blocks
		for (int e = 0; e < col; e++)
		{
			blks[i][e] = grec;
			int x = 50 + 165 * s;       // 50 = horizontal distance bettewn lift siade window
						    // and first block. and more when we wll meet.	
			s++;
			blks[i][e].setPosition(Vector2f(x, y));
			blks[i][e].setFillColor(Color(44 + rand() % 255,44+ rand() % 255, 44+rand() % 255));
		}
		y += 50;                             // virtical distance bettewn blocks 
	}
}

Font font() {
	Font font;
	font.loadFromFile("./orange juice 2.0.ttf");
	return font;
}

Text text(Font font, Color color, int pos[], int chrSize, string str = " ") {
	Text txt;
	txt.setFont(font);
	txt.setFillColor(color);
	txt.setPosition(pos[0], pos[1]);
	txt.setCharacterSize(chrSize);
	txt.setString(str);
	return txt;
}

Sound sund(string name) {
	SoundBuffer buffer;
	buffer.loadFromFile(name);
	Sound sound;
	sound.setBuffer(buffer);
	return sound;
}

void blks_ball_osi(CircleShape ball, RectangleShape blks[][11], 
	float& xstep, float& ystep,int& scour) {
	for (int i = 0; i < rows; i++)
	{
		// check oscillision bettewn blks_ball "by top and bottom "
		for (int j = 0; j < col; j++) {
			FloatRect blksRec = blks[i][j].getGlobalBounds();
			FloatRect ballRec = ball.getGlobalBounds();
			if (ballRec.intersects(blksRec)
				&& ball.getPosition().x >= blks[i][j].getPosition().x
				&& ball.getPosition().x < blks[i][j].getPosition().x + blksRec.width) {
				blks[i][j].setScale(0, 0);
				ystep *= -1;
				scour++;
				//text1.setString("Score: " + to_string(scour));
			}

			// check oscillision bettewn blks_ball "by sides"
			if (ballRec.intersects(blksRec)
				&& (blksRec.top < ballRec.top + .5 * ballRec.height
					&& blksRec.top + blksRec.height + 2 > ballRec.top + .5 * ballRec.height)) {
				blks[i][j].setScale(0, 0);
				xstep *= -1;
				scour++;
			}
		}
	}
}

void player_ball_osi(CircleShape ball, RectangleShape player, float& xstep, float& ystep) {
	// check player_ball oscillision in half no. 1 from left to make
	// ball move by x = 1 (right) either xstep = 1 (right) or -1 (left)
	if (ball.getGlobalBounds().intersects(player.getGlobalBounds()) &&
		ball.getPosition().x <= player.getPosition().x + 125) {
		ystep *= -1;
		if (xstep > 0) { xstep *= -1; }
	}

	// check player_ball oscillision in half no. 2 from left to make
	// ball move by x = -1 (left) either xstep = 1 (right) or -1 
	if (ball.getGlobalBounds().intersects(player.getGlobalBounds()) &&
		ball.getPosition().x > player.getPosition().x + 125) {
		ystep *= -1;
		if (xstep < 0) { xstep *= -1; }
	}

	// this more features to avoide repetation of ball's motian

}

RectangleShape gift() {
	RectangleShape gift(Vector2f(25, 25));
	gift.setScale(0, 0);
	int gx = (window.getSize().x) / 20 +
			 rand() % (window.getSize().x - (window.getSize().x) / 20);

	int gy = 20 + rand() % (window.getSize().y / 2);

	gift.setPosition(Vector2f(gx, gy));
	gift.setFillColor(Color(255, 255, 255));

	return gift;
}


void windowBorder(CircleShape ball, float& xstep, float& ystep) {
	// avoid letting ball go out side from window from 
	if (ball.getPosition().x >= window.getSize().x - ball.getRadius()) xstep *= -1; // reight
	if (ball.getPosition().x - 10 < ball.getRadius()) xstep *= -1; // left
	if (ball.getPosition().y < ball.getRadius()) ystep *= -1; // top
}


int main(){
	srand((unsigned)time(NULL));

	bool start = 0; int scour = 0;
	RectangleShape player = rocket();    // make a player same as rocket

	RectangleShape blks[rows][col];
	blokcs(blks);                        // make array fill of our needed blocks 

	CircleShape balls[3] = { ball(500,500), ball(0,0), ball(0,0)};
	 // balls[0] >> our original ball with x-position = y-position = 500 
	 // balss[1] >> gift ball no. 1 with x-position = y-position = 0 
	 // balss[2] >> gift ball no. 2 with x-position = y-position = 0 

	balls[0].setScale(1, 1);                    // to make our original appear

	RectangleShape tripleGift = gift();        // our gift, it's behaver is releas two new ball 

	/*int pos1[2] = { 2, 275 };
	Text text1 = text(font(), { 255,255,255 }, pos1, 12, "score: " + to_string(scour));
	int pos2[2] = { 0,0 };
	Text text2 = text(font(), { 0,255,0 }, pos2, 25);

	Sound sound = sund("");*/

	float steps[3][2] = { {1,1}, {-1,-1}, {1,-1} }; 
	// steps[0][0], steps[0][1] = x-step, y-step for original ball
        // steps[1][0], steps[1][1] = x-step, y-step for gifted ball no. 1 
        // steps[2][0], steps[2][1] = x-step, y-step for gifted ball no. 2

	bool is_trible_gft = false;          // to know if the trible_gift is catched


	while (window.isOpen())
	{
		Event event;
		// close our game window if user entered q or Q
		while (window.pollEvent(event))
			if(event.TextEntered && 
				(event.text.unicode == 'q' || event.text.unicode == 'Q')) window.close();

		// start playing if user entered Sapce >> start = true
		Vector2f playerPos = player.getPosition();  
		if (Keyboard::isKeyPressed(Keyboard::Space)) start = true;

		if (start == true){

			balls[0].move(steps[0][0], steps[0][1]); // move original ball with  x-step1, y-step1

			// make player/rocket move with respect to our window game to not go out it.
			if (Keyboard::isKeyPressed(Keyboard::A) && playerPos.x > 10)
				player.move(-2.5, 0);
			if (Keyboard::isKeyPressed(Keyboard::D) && playerPos.x <
				window.getSize().x - 10 - player.getSize().x) player.move(2.5, 0);
		}

		//if (ball.getPosition().y > 310 - ball.getRadius())
		//{
		//	/*text1.setPosition(90, 140);
		//	text1.setCharacterSize(25);
		//	text1.setString("score " + to_string(scour));*/
		//}

		// check if balls[0] (original ball) if missed without trible_gfit case occur
		if (balls[0].getPosition().y > window.getSize().y - balls[0].getRadius()
			&& !(is_trible_gft)) {
			start = 0;
		}

		int missedBalls = 0;			// inform us nbr of missed balls
		for (int i = 0; i < 3; i++)
		{
			// check if any ball of 3 if missed with trible_gfit case occur
			if (balls[i].getPosition().y > window.getSize().y - balls[i].getRadius()){ 
				missedBalls++; 
				if (missedBalls == 3) start = 0;  // when the 3 balls is missed stop our game 
			}
		}

		// check if gift block osillite with player/rocket
		if (tripleGift.getGlobalBounds().intersects(player.getGlobalBounds())){
			tripleGift.setPosition(Vector2f(5000, 5000));  // make gift block disappear/out window 

			// releas our extra 2 ball from gift, 
			// from position x >> the center of player , y >> y of player -20
			for (int i = 1; i <= 2; i++)
				balls[i].setPosition(Vector2f(player.getPosition().x + player.getSize().x / 2,
					player.getPosition().y - 20));

			is_trible_gft = true;
		}

		// check if trible_gift if catshed ot make an extra 2 balls 
		if (is_trible_gft &&  start) {
			for (int i = 1; i < 3; i++)
			{
				balls[i].setScale(1, 1);                 // appear 
				balls[i].move(steps[i][0], steps[i][1]); // move
			}
		}

		// make every ball in balls respect 
		for (int i = 0; i < 3; i++){
			windowBorder   (balls[i],         steps[i][0], steps[i][1]); // the border of window
			player_ball_osi(balls[i], player, steps[i][0], steps[i][1]); // oscillition with player
			blks_ball_osi  (balls[i], blks,   steps[i][0], steps[i][1], scour); // oscillition with blocks
		}

		// make tripleGift when scour == random value (3:6) 
		if (scour >= int(3+rand()%3)) {
			tripleGift.setScale(1, 1); // apears
			tripleGift.move(0, .5);    // move verticly to down
		}

		window.clear();

		// draw every object in our game
		window.draw(player);
		DrawBlocks(blks);
		for (int i = 0; i < 3; i++) window.draw(balls[i]);
		window.draw(tripleGift);
		//window.draw(text1);
		//window.draw(text2);

		window.display();
	}
}
