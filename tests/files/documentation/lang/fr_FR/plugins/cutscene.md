## Cinématique

![Alt image](./srcs/img/data/plugins/cutscene/main.png)

> Créer des cinématiques de façon simple et professionnel 

- [Sélectionner une cinématique](#sélectionner-une-cinématique)
- [Créer une nouvelle cinématique](#créer-une-nouvelle-cinématique)
- [Modifier une cinématique](#modifier-une-cinématique)
  - [Ajouter un point](#ajouter-un-point)
  - [Supprimer un point](#supprimer-un-point)
  - [Ajouter une transition](#ajouter-une-transition)
  - [Modifier la durée d'une transition](#modifier-la-durée-d’une-transition)
- [Supprimer une cinématique](#supprimer-une-cinématique)
---
### Sélectionner une cinématique

Afin de travailler sur une cinématique, il est nécessaire d'en choisir une dans le menu déroulant situer en haut à gauche. Cliquer dessus pour les faire apparaître dans une liste en dessous de la zone.
#### <span>&#x26a0;</span> Pour que le système d'ajout de point soit fonctionnel, il est nécessaire d'avoir une cinématique de sélectionner
---
### Créer une nouvelle cinématique

![Alt image](./srcs/img/data/plugins/cutscene/main.png)

Pour créer une cinématique, vous pouvez soit le faire via l'application, soit directement dans le jeu :
| | |
| --- | --- |
| ![Alt image](./srcs/img/data/plugins/cutscene/create_manual.png) | Cliquer sur le bouton ``Créer une cinématique``, entrer le nom de celle-ci et valider le formulaire |
| ![Alt image](./srcs/img/data/plugins/cutscene/create_auto.png) | Cliquer sur le bouton ``Nouvelle cinématique``, ouvrer l'application, entrer le nom de celle-ci et valider le formulaire |

### Modifier une cinématique
![Alt image](./srcs/img/data/plugins/cutscene/panel.png)

 - #### Ajouter un point
   ![Alt image](./srcs/img/data/plugins/cutscene/add_point.png)
  
   Sélectionner le curseur et cliquer sur le bouton ``Ajouter un point``. Votre position ainsi que votre orientation servira de point pour votre cinématique.

- #### Supprimer un point
  Dans le panneau de la cinématique, cliquer sur le bouton de suppression le plus à droite du point

- #### Ajouter une transition
  Il existe 5 types de transitions (basées sur les courbes de bézier) [^1]  :
  | Transition | Explication |
  | --- | --- |
  | linear | La transition est constante, **par défaut** |
  | ease | La transition commence lentement, s'accélère fortement, puis ralentit progressivement vers la fin |
  | ease-in | La transition commence lentement, s'accélère progressivement jusqu'à la fin, où elle s'arrête brusquement |
  | ease-out| La transition commence brusquement, puis ralentit progressivement vers la fin |
  | ease-in-out | La transition commence lentement, s'accélère, puis ralentit vers la fin |

- #### Modifier la durée d'une transition
  La durée de transition représente le temps en seconde nécessaire entre le point actuel et le point suivant, modifiable via le champ ``durée`` du point.
---
### Supprimer une cinématique

![Alt image](./srcs/img/data/plugins/cutscene/delete.png)

Sélectionner une cinématique [(voir ici)](#sélectionner-une-cinématique); puis cliquer sur le bouton ``Supprimer la cinématique`` et valider votre demande

[^1]: https://fr.wikipedia.org/wiki/Courbe_de_B%C3%A9zier