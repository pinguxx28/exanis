# Documentation

## General about the code

- All .c files should have a corresponding .h file in include/
- Only C style comments, so don't put C++ style comments (//)
- Names should be as long as needed but as short as neccessary
- If statements must ALWAYS have brackets EXCEPT if they contain continue, break, goto or return
- Comments with WARNING indicate a bad code example that must be changed
- Includes should be grouped in <> and "" includes they should be sorted by length
- .active describes if the struct exists, then it gets updated and drawn

- Function prefixes:
	- make_ -> returns a struct of suffix
	- create_ -> complex setup for suffix
	- append_ -> stores suffix for later
	- init_ -> makes suffix ready
    - handle_ -> suffix subfunction
	- update_ -> changes variables in suffix
	- draw_ -> adds suffix to draw buffer
	- clear_ -> removes suffix from draw buffer
