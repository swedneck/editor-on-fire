#ifndef EOF_PATHING_H
#define EOF_PATHING_H

#include "song.h"

typedef struct
{
	unsigned long note_start;		//The note at which this deployment started the last time it was processed, or ULONG_MAX if this cache entry is undefined
	unsigned long note_end;			//The first note after which the above star power deployment ended the last time it was processed
	unsigned long note_end_native;	//The track's native number for note index note_end
	unsigned long multiplier;		//The score multiplier in effect
	unsigned long hitcounter;		//The note hit counter in effect
	unsigned long score;			//The total score in effect
	double sp_meter;				//The star power meter level
	double sp_meter_t;				//The uncapped star power meter level
} EOF_SP_PATH_SCORING_STATE;

typedef struct
{
	unsigned long *deployments;					//An array defining the note index number of each deployment, ie deployments[0] being the first SP deployment, deployments[1] being the second, etc.
	EOF_SP_PATH_SCORING_STATE *deploy_cache;	//An array of data about score and star power status at the first note after each end of star power deployments from the previous solution evaluation,
												// allowing much/most of the score processing to be re-used between subsequent solutions where most of the deployments occur at the same indexes
	unsigned long num_deployments;				//The number of star power deployments in this solution

	//These arrays are sized to only store data about the target track difficulty's notes and not all of the track's notes
	double *note_measure_positions;		//An array of double floats the defines the position (defined in measures) of each note in the specified track difficulty
	double *note_beat_lengths;			//An array of double floats defining the length (defined in beats) of each note in the specified track difficulty
	unsigned long note_count;			//The number of elements in the above arrays and the number of notes in the target track difficulty, used for bounds checking

	unsigned long track;				//The track number being processed
	unsigned char diff;					//The difficulty number being processed
	unsigned long score;				//The estimated score if all notes in the processed track difficulty are hit, and all sustain star power notes are whammied for bonus star power
	unsigned long deployment_notes;		//The number of notes played during star power deployment
} EOF_SP_PATH_SOLUTION;

int eof_note_is_last_longest_gem(EOF_SONG *sp, unsigned long track, unsigned long note);
	//Returns nonzero if the note is the longest gem at its position (ie. for identifying the longest gem in a disjointed chord)
	//Returns nonzero if the note does not have disjointed status, as it is the longest note at its position
	//In the event of a tie between this gem and a later one being the same length, zero is returned
	//This allows SP pathing logic to only score SP bonus for one representative gem in a disjointed chord
	//Assumes all notes are sorted
	//Return zero on error

unsigned long eof_note_count_gems_extending_to_pos(EOF_SONG *sp, unsigned long track, unsigned long note, unsigned long pos);
	//Returns the number of notes starting at the specified note's timestamp which extend to or beyond the specified position
	//Returns 1 if the specified note does not have disjointed status
	//Assumes all notes are sorted
	//Returns 0 on error

int eof_note_is_last_in_sp_phrase(EOF_SONG *sp, unsigned long track, unsigned long note);
	//Returns nonzero if the specified note has star power and either of the following is true:
	// 1.  There is not another note that follows it
	// 2.  There is another note that follows it, and it does not have star power

unsigned long eof_translate_track_diff_note_index(EOF_SONG *sp, unsigned long track, unsigned char diff, unsigned long index);
	//Accepts an index specifying the note number within a track difficulty and returns the real index for that note in the track's note[] array
	//Return ULONG_MAX if the index is invalid or upon error

double eof_get_measure_position(unsigned long pos);
	//Returns the specified timestamp in measures, using beat statistics generated by eof_process_beat_statistics
	//If beat statistics are not already cached, eof_process_beat_statistics() is called with focus on the active track
	//Returns 0.0 on error

unsigned long eof_ch_path_find_next_deployable_sp(EOF_SP_PATH_SOLUTION *solution, unsigned long start_index);
	//Given the beat length information about solution's track difficulty, start parsing notes in the track difficulty
	// starting at the specified note index and return the first note index at which 50% or more of the star power meter
	// has been filled, allowing for star power to be deployed
	//Expects that the EOF_NOTE_TFLAG_SP_END flag has been set appropriately by eof_menu_track_find_ch_sp_path()
	//Returns ULONG_MAX on error

int eof_evaluate_ch_sp_path_solution(EOF_SP_PATH_SOLUTION *solution, unsigned long solution_num, int logging);
	//Determines the validity of the proposed star path solution for the specified track difficulty, setting the score and deployment_notes value in the passed structure
	//The score is calculated with scoring rules for Clone Hero
	//The solution's score and deployment_notes variables are modified to contain the values calculated for the solution
	//Returns zero if the proposed solution is invalid (ie. calling for star power deployment while it is already deployed, or when there is insufficient star power)
	//solution_num is the solution number being tested, to be logged and used for debugging
	//If logging is nonzero, scoring details such as the number points awarded per note, when star power deploys and ends, etc. is logged

int eof_menu_track_find_ch_sp_path(void);
	//Determines optimum star power deployment for the active track difficulty

#endif
