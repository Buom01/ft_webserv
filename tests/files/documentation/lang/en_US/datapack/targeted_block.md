## Targeted block
> Allows to retrieve instantly and regardless of the distance the block targeted by the player

- [Template](#template)
- [Usage](#usage)
- [Example](#example)

### Template
``` js
function mapcraft:built_in/targeted_block/main
```
#### Parameters :

| MC_PlayerTargetX | MC_PlayerTargetY | MC_PlayerTargetZ |
| --- | --- | --- |
| Block X position | Block Y position | Block Z position |

#### In what cases is it useful ?
- You want to know in which direction your player is looking
- You want to know which block the player is looking at
- You want to create a mini game that requires a precise aiming device

### Usage

#### <span>&#x26a0;</span> All blocks are taken into consideration except those of type *air*
Called on the player the function to update these settings

### Example

1. After execution, the parameters will be equivalent to the value of the first block reached by the aiming
   ```js
   execute as @e[type=minecraft:player] at @s run function mapcraft:built_in/targeted_block/main
   ```