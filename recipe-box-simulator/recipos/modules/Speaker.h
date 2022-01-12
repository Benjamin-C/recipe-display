/* speaker.h
 *
 * Speaker module for RecipOS
 * Uses the speaker driver to handle audio playback
 *
 * Author: Benjamin Crall
 */

#ifndef RECIPOS_MODULES_SPEAKER_H_
#define RECIPOS_MODULES_SPEAKER_H_

// TODO implement speaker
class Speaker {
	Speaker(void);
	// Beep at frequency for duration ms
	void beep(int frequency, int duration);
};

#endif /* RECIPOS_MODULES_SPEAKER_H_ */
