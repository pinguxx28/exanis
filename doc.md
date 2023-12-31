# Documentation

## General about the code

- All .c files should have a corresponding .h file in include/
- .active describes if the struct exists, then it gets updated and drawn

- Function prefixes:
	- make_ -> returns a struct of suffix
	- create_ -> complex setup for suffix
	- append_ -> stores suffix for later
	- init_ -> makes suffix ready
	- update_ -> changes variables in suffix
	- draw_ -> adds suffix to draw buffer
	- clear_ -> removes suffix from draw buffer
