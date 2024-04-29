#include <GL4D/gl4du.h>
#include <GL4D/gl4dp.h>
#include <GL4D/gl4duw_SDL2.h>
#include <GL4D/gl4dh.h>
#include "audioHelper.h"
#include "animations.h"
#include <assert.h>
#include <stdlib.h>
#include <GL4D/gl4dg.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
static GLuint _quadId = 0;
void animation_vide(int state) {
	switch(state) {
	case GL4DH_DRAW:
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		return;
	default: return;
	}
}
void transition_fondu(void (* a0)(int), void (* a1)(int), Uint32 t, Uint32 et, int state) {
	int vp[4], i;
	GLint tId;
	static GLuint tex[2], pId;
	switch(state) {
	case GL4DH_INIT:
		glGetIntegerv(GL_VIEWPORT, vp);
		glGenTextures(2, tex);
		for(i = 0; i < 2; i++) {
			glBindTexture(GL_TEXTURE_2D, tex[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, vp[2], vp[3], 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		}
		pId = gl4duCreateProgram("<vs>shaders/basic.vs", "<fs>shaders/mix.fs", NULL);
		return;
	case GL4DH_FREE: if(tex[0]) { glDeleteTextures(2, tex); tex[0] = tex[1] = 0; } return;
	case GL4DH_UPDATE_WITH_AUDIO:
		if(a0) a0(state);
		if(a1) a1(state);
		return;
	default:
		glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &tId);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,  tex[0],  0);
		if(a0) a0(state);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,  tex[1],  0);
		if(a1) a1(state);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,  tId,  0);
		glDisable(GL_DEPTH);
		glUseProgram(pId);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex[1]);
		if(et / (GLfloat)t > 1) { fprintf(stderr, "%d-%d -- %f\n", et, t, et / (GLfloat)t); exit(0); }
		glUniform1f(glGetUniformLocation(pId, "dt"), et / (GLfloat)t);
		glUniform1i(glGetUniformLocation(pId, "tex0"), 0);
		glUniform1i(glGetUniformLocation(pId, "tex1"), 1);
		gl4dgDraw(_quadId);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		return;
	}
}
void green(int state) {
	static double mp = 0.0;
	switch(state) {
	case GL4DH_INIT: return;
	case GL4DH_FREE: gl4dpDeleteScreen(); return;
	case GL4DH_UPDATE_WITH_AUDIO: {
		int l = ahGetAudioStreamLength(), i;
		short * s = (short *)ahGetAudioStream();
		double m = 0.0;
		for(i = 0; i < l / 2; ++i)
		m += fabs(s[i] / (double)(1 << 15));
		mp = m / (l / 2);
		return;
	}
	default:
		glClearColor(0.0f, 5.0f * mp, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		return;
	}
}
void blue(int state) {
	switch(state) {
	case GL4DH_INIT: return;
	case GL4DH_FREE: gl4dpDeleteScreen(); return;
	case GL4DH_UPDATE_WITH_AUDIO: return;
	default:
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		return;
	}
}
void red(int state) {
	switch(state) {
	case GL4DH_INIT: return;
	case GL4DH_FREE: gl4dpDeleteScreen(); return;
	case GL4DH_UPDATE_WITH_AUDIO: return;
	default:
		glClearColor(0.37f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		return;
	}
}
void audio_analyzer(int state) {
	static GLuint screen = 0;
	int l, i;
	Sint16 * s;
	static Sint16 t[256];
	static int w, h; 
	switch(state) { 
	case GL4DH_INIT:
		screen = gl4dpInitScreen(); 
		w = gl4dpGetWidth(); 
		h = gl4dpGetHeight(); 
		return; 
	case GL4DH_FREE:
		if(screen) { gl4dpSetScreen(screen); gl4dpDeleteScreen(); gl4duClean(GL4DU_ALL); screen = 0; } return; 
	case GL4DH_UPDATE_WITH_AUDIO:
		s = (Sint16 *)ahGetAudioStream();
		l = ahGetAudioStreamLength();
		if(l >= 2 * 256) for(i = 0; i < 256; i++) t[i] = h / 2 + (h / 2) * s[i] / ((1 << 15) - 1.0); 
		return; 
	default:
		gl4dpSetScreen(screen); 
		gl4dpClearScreen(); 
		for(i = 0; i < 256 - 1; i++) { 
		int x0, y0, x1, y1;
		x0 = (i * (w - 1)) / (256 - 1); 
		y0 = t[i]; 
		x1 = ((i + 1) * (w - 1)) / (256 - 1); 
		y1 = t[i + 1]; 
		gl4dpSetColor(rand());
		gl4dpLine(x0, y0, x1, y1);
		} 
		gl4dpUpdateScreen(NULL);
		return;
	}
}
void setupAnim(int state, const char* fsPath, Anim* anim) {
	switch (state) {
	case GL4DH_INIT:
		anim->progId = gl4duCreateProgram("<vs>shaders/basic.vs", fsPath, NULL);
		anim->quadId = gl4dgGenQuadf();
		anim->lastTime = SDL_GetTicks(); break;
	case GL4DH_FREE: gl4dgDelete(anim->quadId); gl4duClean(GL4DU_ALL); break;
	}
}
void runAnim(Anim* anim, float timeScale) {
	Uint32 currentTime = SDL_GetTicks();
	float deltaTime = (anim->lastTime > 0) ? (currentTime - anim->lastTime) / 1000.0f : 0.0f;
	anim->lastTime = currentTime;
	anim->time += deltaTime * timeScale;
	glUseProgram(anim->progId);
	glUniform1f(glGetUniformLocation(anim->progId, "time"), anim->time);
	int width, height;
	SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), &width, &height);
	glUniform2f(glGetUniformLocation(anim->progId, "resolution"), (float) width, (float) height);
	gl4dgDraw(anim->quadId);
	glUseProgram(0);
}
void sun_wave(int state) {
	static Anim anim = {0};
	setupAnim(state, "<fs>shaders/sun_wave.fs", &anim);
	if (state == GL4DH_DRAW) { runAnim(&anim, 1.0f); }
}
void sun_sphere(int state) {
	static Anim anim = {0};
	setupAnim(state, "<fs>shaders/sun_sphere.fs", &anim);
	if (state == GL4DH_DRAW) { runAnim(&anim, 1.0f); }
}
void sun(int state) {
	static Anim anim = {0};
	setupAnim(state, "<fs>shaders/sun.fs", &anim);
	if (state == GL4DH_DRAW) { runAnim(&anim, 1.0f); }
}
void sphere(int state) {
	static Anim anim = {0};
	setupAnim(state, "<fs>shaders/sphere.fs", &anim);
	switch (state) {
	default: runAnim(&anim, 1.0f); return;
	}
}
void gravity_wave(int state) {
	static Anim anim = {0};
	setupAnim(state, "<fs>shaders/gravity_wave.fs", &anim);
	if (state == GL4DH_DRAW) { runAnim(&anim, 1.0f); }
}
void wave(int state) {
	static Anim anim = {0};
	setupAnim(state, "<fs>shaders/wave.fs", &anim);
	if (state == GL4DH_DRAW) { runAnim(&anim, 1.0f); }
}
void wave2(int state) {
	static Anim anim = {0};
	setupAnim(state, "<fs>shaders/wave2.fs", &anim);
	if (state == GL4DH_DRAW) { runAnim(&anim, 1.0f); }
}
void strip(int state) {
	static Anim anim = {0};
	setupAnim(state, "<fs>shaders/strip.fs", &anim);
	if (state == GL4DH_DRAW) { runAnim(&anim, 3.0f); }
}
void animationsInit(void) { if(!_quadId) _quadId = gl4dgGenQuadf(); }
