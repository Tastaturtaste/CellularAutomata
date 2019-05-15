# CellularAutomata
Small set of Cellular Automata


## Controls:
<pre>
 Space:     Pause/Unpause
 Right:     Speed Up
 Left:      Speed Down
 LMB:       Change Cell (hold down and drag to change all touched cells)
 Backspace: Reset Game  
 Escape:    Close Game
</pre>

## Gamedescriptions:

### Game of Life:  
  Cells are either dead or alive.  
  Dead cells become alive if exactly 3 neighboring cells are alive.  
  Living cells stay alive if either 2 or 3 neighboring cells are also alive.  
	<a href="https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life">Wikipedia: Game of Life</a>
  
  #### Cells:
<pre>
 Green:  Dead
 Yellow: Alive
 </pre>

### Wireworld:  
  Cells are either isolator, conductor, electron head or electron tail.  
  Isolators are inert.  
  Conductors become electron heads if either 1 or 2 neighboring cells are electron heads  
  Electron heads become electron tails.  
  Electron tails become conductors.  
	<a href="https://en.wikipedia.org/wiki/Wireworld">Wikipedia: Wireworld</a>
  
  #### Cells:
<pre>
 Brown:  Isolator
 Yellow: Conductor
 Red:    Electron head
 Blue:   Electron tail
</pre>
