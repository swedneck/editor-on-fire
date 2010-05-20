#ifndef EOF_BEAT_H
#define EOF_BEAT_H

//extern EOF_BEAT_MARKER eof_beat[EOF_MAX_BEATS];
//extern int eof_beats;

int eof_get_beat(unsigned long pos);	//Returns the beat number immediately before the specified position, or -1 if the timestamp does not occur within the chart
int eof_get_beat_length(int beat);		//Returns the difference in position between the specified beat marker and the next, or the difference between the last two beat markers if the beat marker specified is invalid
int eof_find_previous_anchor(int cbeat);	//Returns the beat number of the last anchor that occurs before the specified beat
int eof_find_next_anchor(int cbeat);	//Returns the beat number of the first first anchor that occurs after the specified beat, or -1 if there is no such anchor
int eof_beat_is_anchor(int cbeat);		//Returns nonzero if the specified beat number is an anchor based on its flag or a change in tempo from the prior beat, or if the specified beat number is negative
//int eof_beat_is_moveable(int cbeat);	//Undefined function
//int eof_beat_is_bpm_change(int cbeat);	//Undefined function
void eof_calculate_beats(void);			//Sets the timestamp of each beat existing for the duration of the chart audio
//void eof_change_bpm(int cbeat, unsigned long ppqn);	//Applies the change in timestamps based on the tempo change from the specified beat up to the next anchor
														//This function is not actually used in the program
void eof_realign_beats(int cbeat);		//Recalculates and applies the tempo of the anchors before and after the specified beat, updating beat timestamps, etc.
void eof_recalculate_beats(int cbeat);	//Recalculates and applies the tempo and timings on both sides of cbeat based on the previous and next anchors, such as when dragging a beat marker to define cbeat as an anchor

#endif
