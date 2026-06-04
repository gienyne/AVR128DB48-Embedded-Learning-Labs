/*
 ***************************************************************************************************
 * @author: David Lotz
 * @file:   song.h
 * @date:   12.01.2023
 *
 ***************************************************************************************************
 *
 * Dozent:
 * Jakob Czekansky (jakob.czekansky@mni.thm.de)
 *
 * Mikroprozessortechnik
 * Technische Hochschule Mittelhessen
 *
 ***************************************************************************************************
 */

#ifndef SONG_H_
#define SONG_H_


// sine-values to be played the DAC //
uint16_t sine_table[] = {
	0x200,0x232,0x263,0x294,0x2c3,0x2f1,0x31c,0x344,
	0x369,0x38b,0x3a9,0x3c3,0x3d8,0x3e9,0x3f5,0x3fd,
	0x3ff,0x3fd,0x3f5,0x3e9,0x3d8,0x3c3,0x3a9,0x38b,
	0x369,0x344,0x31c,0x2f1,0x2c3,0x294,0x263,0x232,
	0x200,0x1cd,0x19c,0x16b,0x13c,0x10e,0xe3,0xbb,
	0x96,0x74,0x56,0x3c,0x27,0x16,0xa,0x2,
	0x0,0x2,0xa,0x16,0x27,0x3c,0x56,0x74,
	0x96,0xbb,0xe3,0x10e,0x13c,0x16b,0x19c,0x1cd
	};

// Enumeration of all playable notes //
enum notes {
	h2	 = 987,
	ais2 = 932,
	a2   = 880,
	gis2 = 830,
	g2   = 783,
	fis2 = 739,
	f2	 = 698,
	e2	 = 659,
	dis2 = 622,
	d2	 = 587,
	cis2 = 554,
	c2	 = 523,
	h1	 = 493,
	ais1 = 466,
	a1	 = 440,
	gis1 = 415,
	g1	 = 391,
	fis1 = 369,
	f1	 = 349,
	e1	 = 329,
	dis1 = 311,
	d1	 = 293,
	cis1 = 277,
	c1	 = 261,
	h	 = 246,
	ais	 = 233,
	a	 = 220,
	gis	 = 207,
	g	 = 195,
	fis	 = 184,
	f	 = 174,
	e	 = 164,
	dis	 = 155,
	d	 = 146,
	cis	 = 138,
	c	 = 130,
	mute = 1		// Silence
	};

typedef struct {
	uint16_t bpm;				// beats per minute
	uint16_t length;			// length of the song, amount of succeeding notes
	uint16_t tone[100];			// sequence of all notes 
	uint16_t tone_length[100];	// length of each note, where tone_length[i] -> tone[i] 
} song;

const song mario = {
	330,
	100,
	{c1, mute, g, mute, e, mute, a, h, ais, a, mute, g, e1, g1, a1, f1, g1, mute, e1, c1, d1, h, mute , c1, mute, g, mute, e, mute, a, h, ais, a, mute, g, e1, g1, a1, f1, g1, e1, c1, d1, h, mute, c, mute, f1, fis1, f1, dis1, mute, e1, mute, gis, a, c1, mute, a, c1, d1, c, mute, g1, fis1, f1, dis1, mute, e1, gis, a, c1, mute, a, c1, d1, mute, dis1, mute, d1, mute, c1, mute, g, g, mute, g},
	{2,1,2,1,2,1,2,2,1,1,1,2,1,1,2,1,1,1,2,1,1,1,2,2,1,2,1,2,1,2,2,1,1,1,2,1,1,2,1,1,2,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,2,1,2,1,2,1,1,1,1}
};

const song sweep_up_down_1 = {
	200,
	24,
	{c1, cis1, d1,  dis1,  e1, f1, fis1, g1, gis1, a1, ais1, h1, c2, h1, ais1, a1, gis1, g1, fis1, f1, e1, dis1, d1, cis1},
	{ 1,    1,  1,     1,   1,  1,    1,  1,    1,  1,    1,  1,  1,  1,    1,  1,    1,  1,    1,  1,  1,    1,  1,    1}
};

const song sweep_up_down_0 = {
	200,
	24,
	{c, cis, d,  dis,  e, f, fis, g, gis, a, ais, h, c1, h, ais, a, gis, g, fis, f, e, dis, d, cis},
	{ 1,    1,  1,     1,   1,  1,    1,  1,    1,  1,    1,  1,  1,  1,    1,  1,    1,  1,    1,  1,  1,    1,  1,    1}
};


#endif /* SONG_H_ */