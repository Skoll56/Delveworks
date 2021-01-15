'WSAD' moves the camera around.

'Spacebar' unlocks the cursor

'R' locks the cursor again

'E' spawns a bunch of bouncy balls into the room (They make noise on collision) (Also they sometimes fall out the room, but that's a physics engine problem, not a "this engine" problem)

'Q' deletes all the balls

'T' throws an exception from an OnTick function

C'thulhu doesn't like it when things bump into him.

Buttons can be clicked.


If you want to hide the Engine's UI context:
Initialise core with the "Release" variable in main

Else:
Initialise core with the "Debug" variable in main

Most of the feedback like collision events will pop up in the CMD console so best to leave that open.

'Escape' closes the game

Feel free to resize the window (Everything stretches to fit, although there are no anchors)