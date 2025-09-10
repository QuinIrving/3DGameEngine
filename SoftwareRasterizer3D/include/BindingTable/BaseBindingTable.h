#pragma once
/*
Architecture:

- We want some binding table to map keys to functions
- There is also states of the type of mode which will have different bindings
ex. Systems menu, way to move mouse with cursor around 2d screen/ui,. 3d game with way to move mouse to look around in 3d space.

MWe are swapping the table, so it just calls a function (in the background), so we don't need to worry about calling different function,
it all runs as it should.

functors? something like a void* we call? 


So example flow.
we get WM_KEYDOWN event
- do our basic keyboard updates (update bitset of states setting to 1 if the key is down.
- check the function key, then pass that to the Binding table which will pass back a function (or null). If there is a function, immediately fire it
- that function can do whatever, generate events, toggle some state etc. Not our problem.
^ same for KEY_UP and same for SYSKEY stuff

WM_CHAR/SYSCHAR
- keep a buffer of the letters pressed to be able to be read by the game at any point of time.
- will probably wan't to clear that buffer each cycle before PEEK_MESSAGE, just so we always start with a fresh buffer as obviously we don't care what was typed if we passed
the event that triggered it.
- be able to read in characters one at a time from it (pop it). Could create a queue of the char's just to keep it simple. Still want to make it quick though (perhaps std::move or something on the characters
if it's a full string? look into the best practice here

//// Now for Mouse
WM_LBUTTONDOWN
- same generally as WM_KEYDOWN and stuff
- keep track if a button on mouse is held down
- and get the action provided that we will run.
- Same for Rbutton and Middle button.

WM_MOUSEMOVE:
- We can simply keep updating our mouse position with the new position we receive,
- then call the action from this specific event of move (not sure if we even need, as it only matters for game as 2d should just allow cursor)

WM_MOUSEHOVER
- move on for now, perhaps also map it to a function (so it can potentially figure out the thing later to display a message for tooltip of it hovering

WM_DBLCLK
- keep it as a specific function mapping again, nothing crazy
	
WM_MOUSELEAVE
- Could capture if it's a left mouse button instead and on release just release capture, not sure what we want to do with this message.

WM_MOUSEWHEEL
- Pass to the function mapped to it, the number of occurances it scrolled and the direction (negative/positive number whole number?)
- keep an accumulator of the remainder before 120. 
- could potentially also just keep track in the mouse state the number of rotations but not sure if that even matters honestly. (should be game logic)


- remember, actions can generate events, so an action can either pass NULL which means no event, or an event to be passed into the buffer for later processing.

*/

// this could simply be 
class BaseBindingTable {
public:
	// A way to get the 

private:

};
