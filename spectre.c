#include <GL4D/gl4duw_SDL2.h>
#include <GL4D/gl4dg.h>
#include <GL4D/gl4dh.h>
#include <stdio.h>
#include "audioHelper.h"
static void init(void);
static void quit(void);
static void draw(void);
#define NB_E 1024
static GLuint _wW = 1024, _wH = 768;
static GLuint _cubeId = 0, _pId = 0, _quad = 0;
static float max_height = 1.0;
static int _hauteurs[NB_E];
void spectre(int state) {
	switch(state) {
	case GL4DH_INIT: init(); return;
	case GL4DH_FREE: quit(); return;
	case GL4DH_UPDATE_WITH_AUDIO: {
		int l = ahGetAudioStreamLength(), i;
		short * s = (short *)ahGetAudioStream();
		for(i = 0; i < l / 4; ++i) _hauteurs[i] = s[2 * i] >> 10;
		return;
	}
	default: draw(); return;
	}
}
void init(void) {
	SDL_GL_SetSwapInterval(1);
	_cubeId = gl4dgGenCubef();
	_pId = gl4duCreateProgram("<vs>shaders/spectre.vs", "<fs>shaders/spectre.fs", NULL);
	_quad = gl4dgGenQuadf();
	gl4duGenMatrix(GL_FLOAT, "modelView");
	gl4duGenMatrix(GL_FLOAT, "proj");
	gl4duBindMatrix("proj");
	gl4duLoadIdentityf();
	gl4duFrustumf(-1.0f, 1.0f, (-1.0f * _wH)  / _wW, (1.0f * _wH)  / _wW, 1.0f, 1000.0f);
	glLineWidth(0.3f);
	glEnable(GL_LINE_SMOOTH);
}
static double get_dt(void) {
	static double t0 = 0.0f;
	double t = gl4dGetElapsedTime(), dt = (t - t0) / 1000.0;
	t0 = t;
	return dt;
}
static void quit( void ) {
    if (_quad)
        gl4dgDelete(_quad);
    gl4duClean(GL4DU_ALL);
}
void draw(void) {
	int i; static GLfloat a = 0.0f;
	glClearColor(0, 0, 0, 255);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(_pId);
	glUniform1f(glGetUniformLocation(_pId, "maxHeight"), max_height);
	gl4duBindMatrix("modelView");
	gl4duLoadIdentityf();
	gl4duTranslatef(0.0f, 0.0f, -1.1f);
	for(i = 0; i < NB_E; ++i) {
		int j;
		GLfloat x = 2.0f * (i / (NB_E - 1.0f)) - 1.0f;
		gl4duPushMatrix();
		gl4duTranslatef(x, 0.0f, 0.0f);
		for(j = 0; j <= abs(_hauteurs[i]); ++j) {
		GLfloat y = (_hauteurs[i] < 0 ? -j : j) / 32.0f;
		gl4duPushMatrix();
		gl4duTranslatef(0.0f, y, 0.0f);
		gl4duScalef(0.02f, 0.02f, 0.02f);
		gl4duSendMatrices();
		gl4duPopMatrix();
		gl4dgDraw(_cubeId);
		}
		gl4duPopMatrix();
	}
	glUseProgram(0);
	a += 2.0f * M_PI * get_dt();
}

