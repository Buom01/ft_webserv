## Téléportation instantanée
> Permet de téléporter instantanément un joueur à une position donnée

- [Modèle](#modèle)
- [Utilisation](#utilisation)
- [Exemple](#exemple)

### Modèle
``` js
function mapcraft:built_in/instant_tp/main
```
#### Paramètres :

| MC_PlayerX | MC_PlayerY | MC_PlayerZ | MC_PlayerRx | MC_PlayerRy |
| --- | --- | --- | --- | --- |
| Position X du joueur | Position Y du joueur | Position Z du joueur | Position Rx du joueur | Position Ry du joueur |

#### Dans quel cas est-ce utile ?
- Vous générez des coordonnées via des variables et vous souhaitez téléporter le joueur dessus
- Vous souhaitez créer une commande ``home`` pour votre serveur
- Vous créez des points de contrôle pour votre jeu

### Utilisation

Pour téléporter un joueur à la position souhaitée, il suffit de modifier la valeur des variables ci-dessus par celle souhaiter après la téléportation. Puis appeler la fonction, le joueur sera immédiatement téléporté à la position indiquée.
La téléportation doit ce faire immédiatement après avoir modifié les variables, sinon celles-ci seront remises à leur état initial.

### Exemple

1. Le joueur sera téléporté après la modification aux coordonnées **(21, 65, -174, 12, 45)**
   ```js
   scoreboard players set @s MC_PlayerX 21
   scoreboard players set @s MC_PlayerY 65
   scoreboard players set @s MC_PlayerZ -174
   scoreboard players set @s MC_PlayerRx 12
   scoreboard players set @s MC_PlayerRy 45
   function mapcraft:built_in/instant_tp/main
   ```
2. Si certaines variables ne sont pas modifiées, la position et l'orientation sur ces axes resteront identiques lors de la téléportation. En d'autres termes si le joueur est en **(10, 20, 30, 0, 90)**, et que l'on modifie uniquement **X**, **Z** et **Rx**, sa position future sera **(51, 20, -78, 45, 90)**
   ```js
   scoreboard players set @s MC_PlayerX 51
   scoreboard players set @s MC_PlayerZ -78
   scoreboard players set @s MC_PlayerRx 45
   function mapcraft:built_in/instant_tp/main
   ```