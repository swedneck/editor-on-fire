#ifndef EOF_NOTES_H
#define EOF_NOTES_H

#include <allegro.h>
#include "main.h"
#include "song.h"

//This structure is passed to eof_expand_notes_window_macro() so that it can alter Notes panel settings when relevant macros are read
typedef struct
{
	//Print controls
	int xpos, ypos;	//The current output coordinates of text being printed to the Notes panel
	int color;		//The text color in use
	int bgcolor;	//The background color used for that text
	int allowempty;	//A %EMPTY% macro was parsed, allow an empty output line to print to the Notes panel

	//Parsing controls
	BITMAP *screen;	//The screen to which the output is to be printed
	int flush;		//Set to nonzero if a %FLUSH% macro is parsed, signaling eof_expand_notes_window_text() to output the current content of the output buffer
					// and then resume parsing the current line of text
	int contentprinted;	//Set to nonzero if content was printed for a line, such as by using %FLUSH% even if the output buffer is empty when the end of line is parsed
	int symbol;		//After a flush, this character will be printed to the Notes panel using the symbol font (ie. to print guitar tab characters)

} EOF_NOTES_PANEL_CONTROLS;

int eof_expand_notes_window_text(char *src_buffer, char *dest_buffer, unsigned long dest_buffer_size, EOF_NOTES_PANEL_CONTROLS *controls);
	//Parses the content of the source buffer, expanding macros with eof_expand_notes_window_macro() and populating the destination buffer of the given size
	//The destination buffer is guaranteed to be NULL terminated
	//The controls structure is passed to eof_expand_notes_window_macro(), which alters it appropriately when relevant control macros are parsed
	//If a macro is not recognized, the destination buffer is populated with a message indicating the first macro in the input buffer that wasn't recognized
	//Returns zero on error, and writes any failed macro text conversion
	//Returns 1 on a normal conversion
int eof_expand_notes_window_macro(char *macro, char *dest_buffer, unsigned long dest_buffer_size, EOF_NOTES_PANEL_CONTROLS *controls);
	//Matches the macro against supported macro strings
	//If a match is found, the corresponding text is generated and written to the destination buffer
	//The controls structure is altered appropriately when relevant control macros are parsed
	//Returns 1 if a normal macro is expanded to the destination buffer, or if a control macro was processed
	//Returns 2 if the macro is a conditional test that evaluates as false, which signals that the all content from the macro until the end of the line or next %ENDIF% instance is to be skipped
	//Returns 3 if the macro is a conditional test that evaluates as true, in which case the destination buffer is emptied
	//Returns zero on error or if no matching string is found for the macro
int eof_read_macro_color(char *string, int *color);
	//Accepts a string and performs a comparison against known color names
	//If a match is found, *color is set to the appropriate Allegro color
	//If no match is found, but the string is composed of 6 hexadecimal characters,
	//  it is treated as an RGB color code and that color is created and returned through *color
	//Returns 0 if no color was determined or upon error

#endif