#include <stdio.h>
#include <stdlib.h>
#include <GL4D/gl4du.h>
#include <GL4D/gl4dh.h>
#include <GL4D/gl4duw_SDL2.h>
#include "animations.h"
#include "audioHelper.h"
static void init(void);
static void quit(void);
static void resize(int w, int h);
static void keydown(int keycode);
/*!\brief tableau contenant les animations sous la forme de timeline */
static GL4DHanime _animations[] = {
	{ 1200, animation_vide, spectre, transition_fondu },
	{ 3000, spectre, gravity_wave, transition_fondu },
	{ 8000, gravity_wave, wave, transition_fondu },
	{ 4000, wave, sun_sphere, transition_fondu },
	{ 5000, sun_sphere, sphere, transition_fondu },
	{ 4500, sphere, audio_analyzer, transition_fondu },
	{ 1000, audio_analyzer, sun_wave, transition_fondu },
	{ 4000, sun_wave, red, transition_fondu },
	{ 1000, red, sun, transition_fondu },
	{ 2500, sun, red, transition_fondu },
	{ 3000, sound_sphere, wave2, transition_fondu },
	{ 3000, wave2, spectre, transition_fondu },
	{ 1000, spectre, red, transition_fondu },
	{ 2000, red, credit_fin, transition_fondu },
	{15000, credit_fin, NULL, NULL },
	{    0, NULL, NULL, NULL }
};
/*!\brief dimensions initiales de la fenêtre */
static GLfloat _dim[] = {1280, 720};
/*!\brief fonction principale : initialise la fenêtre, OpenGL, audio et lance la boucle principale (infinie).*/
int main(int argc, char ** argv) {
	if(!gl4duwCreateWindow(argc, argv, "Demo 64Ko", 
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
		_dim[0], _dim[1],
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP))
		return 1;
	init();
	atexit(quit);
	gl4duwResizeFunc(resize);
	gl4duwKeyDownFunc(keydown);
	gl4duwDisplayFunc(gl4dhDraw);
	ahInitAudio("odetojoy.mid");
	gl4duwMainLoop();
	return 0;
}
/*!\brief Cette fonction initialise les paramètres et éléments OpenGL ainsi que les divers données et fonctionnalités liées à la gestion des animations.*/
static void init(void) {
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	gl4dhInit(_animations, _dim[0], _dim[1], animationsInit);
	resize(_dim[0], _dim[1]);
}
/*!\brief paramétre la vue (viewPort) OpenGL en fonction des dimensions de la fenêtre.
 * \param w largeur de la fenêtre. \param w hauteur de la fenêtre.*/
static void resize(int w, int h) {
	_dim[0] = w; _dim[1] = h;
	glViewport(0, 0, _dim[0], _dim[1]);
	transition_fondu(NULL,NULL,0,0,4);
	gl4dhResize(w, h);
}
/*!\brief permet de gérer les évènements clavier-down.
 * \param keycode code de la touche pressée.*/
static void keydown(int keycode) {
	switch(keycode) {
	case SDLK_ESCAPE: exit(0);
	case 'q': exit(0);
	default: break;
	}
}
/*!\brief appelée à la sortie du programme (atexit).*/
static void quit(void) {
	ahClean();
	gl4duClean(GL4DU_ALL);
}
