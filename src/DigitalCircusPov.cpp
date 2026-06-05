/*
Original project: https://scratch.mit.edu/projects/1191104157
thanks, FUZZIE-WEASEL!

Arts: FUZZIE-WEASEL
Code SB3: FUZZIE-WEASEL
Code C++: Frank Steps

Music:         The free design
sound effects: FUZZIE-WEASEL

Plushies images: https://glitchproductions.store/collections/the-amazing-digital-circus
*/

// libraries
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

namespace ray{
	#include <raylib.h>
}

int main(){
	ray::InitWindow(700, 524, "POV: Digital Circus"); 
	ray::InitAudioDevice();

	ray::Music bubbles = ray::LoadMusicStream("src/OST/bubbles.mp3");
	ray::Sound click = ray::LoadSound("src/OST/click.wav");
	ray::Sound woouw = ray::LoadSound("src/OST/wouw.wav");

	ray::Texture nextButton = ray::LoadTexture("src/images/NextButton.png");
	ray::Texture nextButton_on = ray::LoadTexture("src/images/NextButton_on.png");

	const int framHands = 11;
	const int quantPlush = 12;

	std::vector <ray::Texture> plushies(quantPlush); 
	std::vector <ray::Texture> leftHandsV(framHands);
	std::vector <ray::Texture> hightHandsV(framHands);
	std::vector <int> randomPlushies(100);

	for(int i = 0; i < quantPlush; i++){
		plushies[i] = ray::LoadTexture(("src/images/plushies/p" + std::to_string(i) + ".png").c_str());
	}

	for(int i = 0; i < framHands; i++){
		leftHandsV[i] = ray::LoadTexture(("src/images/hands/leftH/leftH_" + std::to_string(i) + ".png").c_str());
		hightHandsV[i] = ray::LoadTexture(("src/images/hands/highH/highH_" + std::to_string(i) + ".png").c_str());
	}

	int handAtual = 0;
	float timer = 0.0f;
	float delay = 0.1f;

	unsigned seed = time(0);
	srand(seed);
	int randomPlush = 0;

	for (int i = 0; i < 100; i++) {
	    if (rand() % 100000 == 0) {
	        randomPlushies[i] = 9;
	    } else {
	        do {
	            randomPlushies[i] = rand() % quantPlush;
	        } while (randomPlushies[i] == 9);
	    }
	}

	const float scale = 0.65f;

	const float plushX = (700 - plushies[randomPlush].width * scale)/2;
	const float plushY = (524 - plushies[randomPlush].height * scale)/2;

	const float handX = (700 - leftHandsV[0].width * scale*1.1) + 20;
	const float handY = (524 - leftHandsV[0].height * scale*1.1) + 50;

	int plushAtual = 0;
	bool handShow = false; 

	
	float deltaShake = 5.0f;
	ray::Vector2 shake = {0.0f, 0.0f};


	ray::PlayMusicStream(bubbles);
	bubbles.looping = true;


	while(!ray::WindowShouldClose()){
		ray::UpdateMusicStream(bubbles);

		// mouse coordinates :: parallax effect
		ray::Vector2 mousePos = ray::GetMousePosition();
		ray::Vector2 center = {ray::GetScreenWidth()/2.0f, ray::GetScreenHeight()/2.0f};
		ray::Vector2 deltaMov = {(mousePos.x - center.x) * 2.1f, (mousePos.y - center.y) * 2.1f};

		ray::Rectangle buttonN = {plushX - deltaMov.x, (plushY - 150) - deltaMov.y, nextButton.width * scale, nextButton.height * scale};
		ray::Rectangle buttonPlush = {plushX - deltaMov.x, plushY - deltaMov.y, plushies[0].width * scale, plushies[0].height * scale};

		// timer
		timer += ray::GetFrameTime();

		if(timer >= delay){
			timer = 0.0f;
			handAtual++;

			if(handAtual >= (framHands-2)) {
				handAtual = 0;	
			} 
		}


		// shake
		shake.x = ray::GetRandomValue(-deltaShake, deltaShake);
		shake.y = ray::GetRandomValue(-deltaShake, deltaShake);

		ray::BeginDrawing();
			// render background
			ray::ClearBackground(ray::BLACK);

			// render plushies 
			if(ray::CheckCollisionPointRec(mousePos, buttonPlush)){
				ray::DrawTextureEx(plushies[randomPlushies[plushAtual]], {plushX + shake.x - deltaMov.x, plushY + shake.y - deltaMov.y},  0,  scale,  ray::WHITE);
				if(ray::IsMouseButtonPressed(ray::MOUSE_BUTTON_LEFT)){
					ray::PlaySound(woouw);
				}
			} else {
				ray::DrawTextureEx(plushies[randomPlushies[plushAtual]], {plushX - deltaMov.x, plushY - deltaMov.y},  0,  scale,  ray::WHITE);
			}


			// render button
			if(ray::CheckCollisionPointRec(mousePos, buttonN)){

				ray::DrawTextureEx(nextButton_on, {(plushX) + shake.x - deltaMov.x, (plushY - 150) + shake.y - deltaMov.y}, 0, scale*1.05, ray::WHITE);
				
				if(ray::IsMouseButtonPressed(ray::MOUSE_LEFT_BUTTON)){
					ray::PlaySound(click);
					plushAtual++;
					if(plushAtual >= 100){
						plushAtual = 0;
					}
				}
			} else {
				ray::DrawTextureEx(nextButton, {plushX - deltaMov.x, (plushY - 150) - deltaMov.y}, 0, scale, ray::WHITE);
			}


			// render hands
			handShow = true;
			if(ray::IsMouseButtonDown(ray::MOUSE_BUTTON_LEFT)){
				if(ray::CheckCollisionPointRec(mousePos, buttonN)){
					ray::DrawTextureEx(hightHandsV[framHands-1], {handX - 50.0f, handY - 50.0f}, 0,  scale*1.1,  ray::WHITE);

				} else if (ray::CheckCollisionPointRec(mousePos, buttonPlush) ) {
					ray::DrawTextureEx(hightHandsV[framHands-2], {handX - 40, handY + (shake.y)*2}, 0,  scale*1.1,  ray::WHITE);
					ray::DrawTextureEx(leftHandsV[framHands-2], {(handX*(1/2) - 40), handY - (shake.y)*2}, 0,  scale*1.1,  ray::WHITE);
					handShow = false;
				} else {
					ray::DrawTextureEx(hightHandsV[handAtual], {handX, handY}, 0,  scale*1.1,  ray::WHITE);
				}
			
			} else if (ray::IsMouseButtonDown(ray::MOUSE_BUTTON_RIGHT) && ray::CheckCollisionPointRec(mousePos, buttonPlush)) { 
				
				// nothing yet...

			} else {
				ray::DrawTextureEx(hightHandsV[handAtual], {handX, handY}, 0,  scale*1.1,  ray::WHITE);
			}
			if(handShow){
				ray::DrawTextureEx(leftHandsV[handAtual], {handX*(1/2), handY},   0,  scale*1.1,  ray::WHITE);
			}

		ray::EndDrawing();
	}

	// unloads 
	for(int t = 0; t < quantPlush; t++){
		ray::UnloadTexture(plushies[t]);
	}

	for(int t = 0; t < framHands; t++){
		ray::UnloadTexture(hightHandsV[t]);
		ray::UnloadTexture(leftHandsV[t]);
	}
	ray::UnloadTexture(nextButton);
	ray::UnloadTexture(nextButton_on);
	ray::UnloadMusicStream(bubbles);
	ray::UnloadSound(click);
	ray::UnloadSound(woouw);

	ray::CloseAudioDevice();
	ray::CloseWindow();
	return 0;
}