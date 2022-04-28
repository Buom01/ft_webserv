## Joueur
> Ensemble de fonctions permettant de récupérer plusieurs données sur un lecteur

- [Fonctions](#fonctions)
  - [ID](#id)
  - [Position](#position)
  - [Rotation](#rotation)
- [Exemple](#exemple)

## Fonctions
- #### ID
  > Récupérer l'identifiant unique du joueur

  L'identifiant est compris dans la plage {[*-999 999 999*, *-100 000 000*], [*100 000 000*, *999 999 999*] }
  | Paramètre | Commande |
  |---|---|
  | ID | ``scoreboard players get @s MC_PlayerID`` |

- #### Position
  > Récupérer la position du joueur

  | Paramètre | Commande |
  | --- | --- |
  | X | ``scoreboard players get @s MC_PlayerX`` |
  | Y | ``scoreboard players get @s MC_PlayerY`` |
  | Z | ``scoreboard players get @s MC_PlayerZ`` |

- #### Rotation
  > Récupérer la rotation du joueur

  | Paramètre | Commande |
  | --- | --- |
  | Rx | ``scoreboard players get @s MC_PlayerRx`` |
  | Ry | ``scoreboard players get @s MC_PlayerRy`` |

## Exemple

1. Nous voulons dire ***Hello World*** si un joueur se trouve à ``{X=10, Y={25..35}, Z=20}``.
   ```js
   execute if score @s MC_PlayerX matches 10 if score @s MC_PlayerY matches 25..35 if score @s MC_PlayerZ matches 20 run say Hello World
   ```

2. Nous voulons vérifier si le joueur regarde le nord-ouest
   ```js
   execute if score @s MC_PlayerRx matches 135..180 run say Hello World
   ```