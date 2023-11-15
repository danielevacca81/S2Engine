// ConsoleUtils.h
//
#ifndef CONSOLEUTILS_H
#define CONSOLEUTILS_H

#ifdef _WIN32
#include <Windows.h>
#endif
#include <iostream>

namespace s2 {

/* These are the first 16 colors anyways. You test the other hundreds yourself.
After 15 they are all combos of different color text/backgrounds. */
struct console_color
{
private:
	enum Col { DBLUE = 1, GREEN, CYAN, DRED, DPURP, BROWN, LGREY, DGREY, BLUE, LIMEG, TEAL, RED, PURPLE, YELLOW, WHITE, B_B };

public:
	static console_color darkBlue() { return console_color( DBLUE ); }
	static console_color green() { return console_color( GREEN ); }
	static console_color cyan() { return console_color( CYAN ); }
	static console_color darkRed() { return console_color( DRED ); }
	static console_color darkPurple() { return console_color( DPURP ); }
	static console_color brown() { return console_color( BROWN ); }
	static console_color lightGray() { return console_color( LGREY ); }
	static console_color gray() { return console_color( DGREY ); }
	static console_color blue() { return console_color( BLUE ); }
	static console_color lime() { return console_color( LIMEG ); }
	static console_color teal() { return console_color( TEAL ); }
	static console_color red() { return console_color( RED ); }
	static console_color purple() { return console_color( PURPLE ); }
	static console_color yellow() { return console_color( YELLOW ); }
	static console_color white() { return console_color( WHITE ); }
	static console_color BB() { return console_color( B_B ); }

	friend std::ostream& operator<< ( std::ostream &s, const console_color &c );

private:
	Col color;
	console_color( const Col &c ) { color = c; }

};

#ifdef _WIN32
std::ostream& operator<< ( std::ostream &s, const console_color &c )
{
	HANDLE hcon = GetStdHandle( STD_OUTPUT_HANDLE );
	SetConsoleTextAttribute( hcon, c.color );
	return s;
}
#else
std::ostream& operator<< ( std::ostream &s, const console_color &c )
{
	return s;
}



}
#endif
#endif

