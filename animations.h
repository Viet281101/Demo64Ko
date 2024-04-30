#ifndef _ANIMATIONS_H
#define _ANIMATIONS_H

#include <GL4D/gl4du.h>
#include <GL4D/gl4dp.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif
	typedef struct Anim {
		float time;
		GLuint progId;
		GLuint quadId;
		Uint32 lastTime;
	} Anim;
	/* animations */
	extern void animation_vide(int state);
	extern void animationsInit(void);
	extern void green(int state);
	extern void red(int state);
	extern void audio_analyzer(int state);
	extern void sun_wave(int state);
	extern void sun_sphere(int state);
	extern void sound_sphere(int state);
	extern void sun(int state);
	extern void sphere(int state);
	extern void gravity_wave(int state);
	extern void wave(int state);
	extern void wave2(int state);
	extern void credit_fin(int state);
	/* transition */
	extern void transition_fondu(void (* a0)(int), void (* a1)(int), Uint32 t, Uint32 et, int state);
	extern void spectre(int state);
#ifdef __cplusplus
}
#endif

#endif

