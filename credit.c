#include <GL4D/gl4duw_SDL2.h>
#include <GL4D/gl4dg.h>
#include <GL4D/gl4dp.h>
#include <GL4D/gl4dh.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
static void init(void);
static void resize(int width, int height);
static void draw(void);
static void sortie(void);
static int _fontSize = 64;
static GLuint _quadId = 0;
static GLuint _pId = 0;
static GLuint _texId[1] = { 0 };
static char *_texte = NULL;
void credit_fin(int state) {
	switch(state) {
	case GL4DH_INIT:
		_texte = "\n                    Credits - Projet Demo64Ko                \n\n"
		"    - Shaders: \n    Shaders GL4Dummies\n    glslsandbox.com \n\n"
		"    - Musique: \n    Beethoven - 9th Symphony (Ode To Joy)\n\n"
		"    - Font: \n    Alagard - dafont.com\n\n"
		"    - Code d'ajouter et de modification: \n    Viet Nguyen - 20006303 - L3Y\n\n"
		"    - Code de base: \n  GL4Dummies - Monsieur Fares BELHADJ\n";
		init(); return;
	case GL4DH_FREE: sortie(); return;
	default: draw(); return;
	}
}
void init(void) {
	SDL_Surface * s = NULL, * d = NULL;
	TTF_Font * font = NULL;
	SDL_Color c = {255, 255, 255, 255};
	_quadId = gl4dgGenQuadf();
	_pId = gl4duCreateProgram("<vs>shaders/texte.vs", "<fs>shaders/texte.fs", NULL);
	if(!_pId) { fprintf(stderr, "Error creating shader program\n"); exit(1); }
	/* initialiser la lib SDL2_ttf */
	if(TTF_Init() == -1) {
		fprintf(stderr, "TTF_Init: %s\n", TTF_GetError()); exit(2);
	}
	/* chargement de la font */
	if( !(font = TTF_OpenFont("alagard.ttf", _fontSize)) ) {
		fprintf(stderr, "TTF_OpenFont: %s\n", TTF_GetError()); exit(2);
	}
	/* création d'une surface SDL avec le texte */
	s = TTF_RenderUTF8_Blended_Wrapped(font, _texte, c, 2048);
	if(s == NULL) {
		TTF_CloseFont(font);
		fprintf(stderr, "Erreur lors du TTF_RenderText\n"); exit(2);
	}
	/* création d'une surface vierge et "compatible transfert" GL */
	d = SDL_CreateRGBSurface(0, s->w, s->h, 32, R_MASK, G_MASK, B_MASK, A_MASK);
	assert(d);
	/* copie depuis s vers d */
	SDL_BlitSurface(s, NULL, d, NULL);
	/* libérer s */
	SDL_FreeSurface(s);
	/* transfert vers GL */
	glGenTextures(sizeof _texId / sizeof *_texId, _texId);
	assert(_texId[0] );
	glBindTexture(GL_TEXTURE_2D, _texId[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	/* transfert effectif */
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, d->w, d->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, d->pixels);
	glBindTexture(GL_TEXTURE_2D, 0);
	/* libérer d */
	SDL_FreeSurface(d);
	/* libérer la font */
	TTF_CloseFont(font);
	gl4duGenMatrix(GL_FLOAT, "model");
	gl4duGenMatrix(GL_FLOAT, "view");
	gl4duGenMatrix(GL_FLOAT, "proj_id");
	gl4duBindMatrix("proj_id");
	gl4duLoadIdentityf();
	/* gl4duFrustumf(-1.0f, 1.0f, (-1.0f * _wH)  / _wW, (1.0f * _wH)  / _wW, 1.0f, 1000.0f); */
	resize(1000, 800);
}
static void resize(int width, int height) {
	int newWidth = width * 0.8;
	int newHeight = height * 0.8;
	int xOffset = (width - newWidth) / 2;
	int yOffset = (height - newHeight) / 2;
	glViewport(xOffset, yOffset, newWidth, newHeight);
	gl4duBindMatrix("view");
	gl4duLoadIdentityf();
	gl4duFrustumf(-1.0f, 1.0f, (-1.0f * newHeight) / newWidth, (1.0f * newHeight) / newWidth, 1.0f, 1000.0f);
	gl4duBindMatrix("model");
}
static double get_dt(void) {
	static double t0 = 0.0f;
	double t = gl4dGetElapsedTime(), dt = (t - t0) / 1000.0;
	t0 = t;
	return dt;
}
void draw(void) {
	static GLfloat a = 0.0f;
	glClearColor(0, 0, 0, 255);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(_pId);
	gl4duBindMatrix("view");
	gl4duLoadIdentityf();
	gl4duBindMatrix("model");
	gl4duLoadIdentityf();
	gl4duSendMatrices();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texId[0]);
	glUniform1i(glGetUniformLocation(_pId, "use_tex"), 1);
	gl4dgDraw(_quadId);
	glUseProgram(0);
	a += 0.4f * M_PI * get_dt();
}
void sortie(void) {
	if(_texId[0]) {
		glDeleteTextures(sizeof _texId / sizeof *_texId, _texId);
		_texId[0] = 0;
		_texte = NULL;
	}
}

