## Bloc visé
> Permet de récupéré instantanément et peu importe la distance le bloc visé par le joueur

- [Modèle](#modèle)
- [Utilisation](#utilisation)
- [Exemple](#exemple)

### Modèle
``` js
function mapcraft:built_in/targeted_block/main
```
#### Paramètres :

| MC_PlayerTargetX | MC_PlayerTargetY | MC_PlayerTargetZ |
| --- | --- | --- |
| Position X du bloc | Position Y du bloc | Position Z du bloc |

#### Dans quel cas est-ce utile ?
- Vous souhaitez savoir dans quelle direction votre joueur regarde
- Vous souhaitez connaitre le bloc que le joueur vise précisément
- Vous créer un mini jeu nécessitant un organe de visée précis

### Utilisation

#### <span>&#x26a0;</span> Tous les blocs sont pris en compte excepté ceux de type *air*
Appelé sur le joueur la fonction pour mettre à jour ces paramètres

### Exemple

1. Après l’exécution, les paramètres seront équivalant à la valeur du premier bloc atteint par la visée
   ```js
   execute as @e[type=minecraft:player] at @s run function mapcraft:built_in/targeted_block/main
   ```