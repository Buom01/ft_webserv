## Player
> Set of functions to retrieve several data on a player

- [Functions](#functions)
  - [ID](#id)
  - [Position](#position)
  - [Rotation](#rotation)
- [Example](#example)

## Functions
- #### ID
  > Retrieve the player unique identifier

  The identifier is in the range { [*-999 999 999*, *-100 000 000*], [*100 000 000*, *999 999 999*] }
  | Parameter | Command |
  |---|---|
  | ID | ``scoreboard players get @s MC_PlayerID`` |

- #### Position
  > Retrieve the player position

  | Parameter | Command |
  | --- | --- |
  | X | ``scoreboard players get @s MC_PlayerX`` |
  | Y | ``scoreboard players get @s MC_PlayerY`` |
  | Z | ``scoreboard players get @s MC_PlayerZ`` |

- #### Rotation
  > Retrieve the player rotation

  Allows to recover the rotation of a player 
  | Parameter | Command |
  | --- | --- |
  | Rx | ``scoreboard players get @s MC_PlayerRx`` |
  | Ry | ``scoreboard players get @s MC_PlayerRy`` |

## Example

1. We want to say ***Hello World*** if a player is in ``{X=10, Y={25..35}, Z=20}``
   ```js
   execute if score @s MC_PlayerX matches 10 if score @s MC_PlayerY matches 25..35 if score @s MC_PlayerZ matches 20 run say Hello World
   ```

2. We want to check if player looking at northwest
   ```js
   execute if score @s MC_PlayerRx matches 135..180 run say Hello World
   ```