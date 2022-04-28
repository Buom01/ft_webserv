## Instant teleportation
> Allows you to instantly teleport a player to a specified position

- [Template](#template)
- [Usage](#usage)
- [Example](#example)

### Template
``` js
function mapcraft:built_in/instant_tp/main
```
#### Parameters :

| MC_PlayerX | MC_PlayerY | MC_PlayerZ | MC_PlayerRx | MC_PlayerRy |
| --- | --- | --- | --- | --- |
| Player X position | Player Y position | Player Z position | Player Rx position | Player Ry position |

#### In what cases is it useful ?
- You generate coordinates via variables and you want to teleport the player to them
- You want to create a ``home`` control for your server
- You create checkpoints for your game

### Usage

To teleport a player to the desired position, just change the value of the above variables to the desired value after teleportation. Then call the function, the player will be immediately teleported to the indicated position.
The teleportation must be done immediately after modifying the variables, otherwise they will be reset to their initial state.

### Example

1. The player will be teleported after the modification to the coordinates **(21, 65, -174, 12, 45)**
   ```js
   scoreboard players set @s MC_PlayerX 21
   scoreboard players set @s MC_PlayerY 65
   scoreboard players set @s MC_PlayerZ -174
   scoreboard players set @s MC_PlayerRx 12
   scoreboard players set @s MC_PlayerRy 45
   function mapcraft:built_in/instant_tp/main
   ```
2. If some variables are not changed, the position and orientation on these axes will remain the same when teleporting. In other words, if the player is in **(10, 20, 30, 0, 90)**, and only **X**, **Z** and **Rx** are changed, his future position will be **(51, 20, -78, 45, 90)**
   ```js
   scoreboard players set @s MC_PlayerX 51
   scoreboard players set @s MC_PlayerZ -78
   scoreboard players set @s MC_PlayerRx 45
   function mapcraft:built_in/instant_tp/main
   ```