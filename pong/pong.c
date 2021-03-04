#include "SDL2/SDL.h"
//#include "SDL2/SDL_ttf.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "stdbool.h"

bool checkCollision(SDL_Rect a, SDL_Rect b){
	int leftA, leftB, rightA, rightB, topA, topB, bottomA, bottomB;

	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	if(bottomA <= topB){
		return false;
	}
	
	if(topA >= bottomB){
		return false;
	}

	if(rightA <= leftB){
		return false;
	}

	if(leftA >= rightB){
		return false;
	}

	return true;
}

int main(int argc, char ** argv) {
	
	SDL_Window *window;
	SDL_Renderer *renderer;
	
	int fps = 60;
	int ticksPframe = 1000 / fps;
	bool quit = false;
	int p1vel = 0, p2vel = 0;
	int ballvelx = 0, ballvely = 0;
	int vel = 10;
//	int p1score = 0, p2score = 0;
	
	//TTF_Init;
      	SDL_Rect p1;
      	SDL_Rect p2;
      	SDL_Rect ball;
//	SDL_Rect p1ScoreRect;
//	SDL_Rect p2ScoreRect;
      	SDL_Event event;
      	SDL_Init(SDL_INIT_VIDEO);
//	TTF_Font * text;
//	SDL_Surface* p1SurfaceMsg;
//	SDL_Surface* p2SurfaceMsg;
//	SDL_Texture* p1ScoreMsg;
//	SDL_Texture* p2ScoreMsg;
//	SDL_Color color={0, 255, 0, 255};
	
	//Creating the window and renderer
	window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 750, SDL_WINDOW_OPENGL); 
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	p1.w = 20;
	p1.h = 150;
	p1.x = 10;
	p1.y = 300;

	p2.w = 20;
	p2.h = 150;
	p2.x = 970;
	p2.y = 300;

	ball.w = 10;
	ball.h = 10;
	ball.x = 490;
	ball.y = 365;

//	p1ScoreRect.x = 450;
//	p1ScoreRect.y = 10;
//	p1ScoreRect.w = 40;
//	p1ScoreRect.h = 50;
//
//	p2ScoreRect.x = 510;
//	p1ScoreRect.y = 10;
//	p1ScoreRect.w = 40;
//	p1ScoreRect.h = 50;
//
//	text = TTF_OpenFont("NotoSerif-BlackItalic.ttf", 24);
//	p1SurfaceMsg = TTF_RenderText_Solid(text, p1score, color);
//	p2SurfaceMsg = TTF_RenderText_Solid(text, p2score, color);
//	p1ScoreMsg = SDL_CreateTextureFromSurface(renderer, p1SurfaceMsg);
//	p2ScoreMsg = SDL_CreateTextureFromSurface(renderer, p2SurfaceMsg);


	ballvelx = -4;
	ballvely = 5;
	
	while(!quit){
		//Check first frame to compare later for smooth framerate
		int startFrame = SDL_GetTicks();
		//Handling keyboard events
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym){
						case SDLK_w: 
							p1vel = -vel;
							break;
						case SDLK_s: 
							p1vel = vel;
							break;
						case SDLK_i:
							p2vel = -vel;
							break;
						case SDLK_k:
							p2vel = vel;
							break;
					}
					break;
				case SDL_KEYUP:
					switch(event.key.keysym.sym){
						case SDLK_s:
							if(p1vel > 0){p1vel = 0;}
							break;
						case SDLK_w:
							if(p1vel < 0){p1vel = 0;}
							break;
						case SDLK_k:
							if(p2vel > 0){p2vel = 0;}
							break;
						case SDLK_i:
							if(p2vel < 0){p2vel = 0;}
							break;
					}
					break;
				case SDL_QUIT:
					quit = true;
					break;
			}
		}
		SDL_Delay(1);
		
		if(checkCollision(ball, p1) == true || checkCollision(ball, p2) == true){
			ballvelx = -ballvelx * 1.1;
		}

		if(ball.y <= 0 || ball.y >= 740){
			ballvely = -ballvely * 1.1;
		}

		if(ball.x <= 0){
			//p2score++;
			
			p1.w = 20;
			p1.h = 150;
			p1.x = 10;
			p1.y = 300;
		
			p2.w = 20;
			p2.h = 150;
			p2.x = 970;
			p2.y = 300;
		
			ball.w = 10;
			ball.h = 10;
			ball.x = 490;
			ball.y = 365;
			
	      		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	      		SDL_RenderClear(renderer);
	      		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	      		SDL_RenderDrawRect(renderer, &p1);
	      		SDL_RenderDrawRect(renderer, &p2);
	      		SDL_RenderDrawRect(renderer, &ball);
			SDL_RenderDrawLine(renderer, 500, 0, 500, 740);
	      		SDL_RenderPresent(renderer);

			SDL_Delay(3000);
		}
		
		if(ball.x + ball.w >= 1000){
			//p1score++;
			
			p1.w = 20;
			p1.h = 150;
			p1.x = 10;
			p1.y = 300;
		
			p2.w = 20;
			p2.h = 150;
			p2.x = 970;
			p2.y = 300;
		
			ball.w = 10;
			ball.h = 10;
			ball.x = 490;
			ball.y = 365;
			
	      		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	      		SDL_RenderClear(renderer);
	      		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	      		SDL_RenderDrawRect(renderer, &p1);
	      		SDL_RenderDrawRect(renderer, &p2);
	      		SDL_RenderDrawRect(renderer, &ball);
			SDL_RenderDrawLine(renderer, 500, 0, 500, 740);
	      		SDL_RenderPresent(renderer);

			SDL_Delay(3000);
		}

		//Updating position
		p1.y += p1vel;
		p2.y += p2vel;
		ball.x += ballvelx;
		ball.y += ballvely;

	      	//Drawing the scene
	      	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	      	SDL_RenderClear(renderer);
	      	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	      	SDL_RenderDrawRect(renderer, &p1);
	      	SDL_RenderDrawRect(renderer, &p2);
	      	SDL_RenderDrawRect(renderer, &ball);
		SDL_RenderDrawLine(renderer, 500, 0, 500, 740);
	      	SDL_RenderPresent(renderer);
		
		// Checking the last codes tick will help us make out how much time we've to wait before starting the next frame
		// Note that this isn't a perfect solution and it could be improved upon since it only really works for faster machines, however it works for something simple like this
		int lastFrame = SDL_GetTicks();
		if(lastFrame - startFrame < ticksPframe){
			SDL_Delay(ticksPframe - (lastFrame - startFrame));
		}
	}
	
	
//	TTF_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
