## Zone de détection
![trigger_base](./srcs/img/data/plugins/trigger/base.png) 
> Une zone de détection vous permet de détecter le passage d'un joueur à travers elle, et d'effectuer des actions en conséquence

- [Ajouter](#ajouter)
  - [Manuel](#manuel)
  - [Automatique](#automatique)
- [Modifier](#modifier)
- [Supprimer](#supprimer)
---
### Ajouter
Il est possible de créer de deux façon différentes, ``manuel`` ou ``automatique``. La méthode recommandé est ``automatique``.

- #### Manuel
|  |  |
| --- | --- |
| ![trigger_manuel](./srcs/img/data/plugins/trigger/add_manual.png) | Cliquer sur le bouton ``Créer une nouvelle zone``, et entrer les coordonnées ``XYZ`` des deux points représentants les extrémités de votre zone. |

- #### Automatique
| | |
| --- | --- |
| ![trigger_auto_1](./srcs/img/data/plugins/trigger/add_automatic_1.png) | Sélectionner le cursor et le module ``Zone de détection``. |
| ![trigger_auto_2](./srcs/img/data/plugins/trigger/add_automatic_2.png) | Sélectionner votre zone et cliquer sur ``Créer une nouvelle zone``. |
| ![trigger_auto_3](./srcs/img/data/plugins/trigger/add_automatic_3.png) | Cliquer sur la notification ou aller sur votre application, rentrer le nom de votre zone et validé. Vous pouvez fermer la fenêtre de création. |

### Modifier
| | |
| --- | --- |
| ![trigger_modify_1](./srcs/img/data/plugins/trigger/modify.png) | Cliquer sur le bouton avec le crayon pour modifier une zone. |
| ![trigger_modify_1](./srcs/img/data/plugins/trigger/action.png) | Cliquer sur le bouton avec la page pour modifier le fichier s’exécutant lors du passe du joueur.<br/> <p style="font-size: 1.2em"><span>&#x26a0;</span> Attention le fichier est un ``.mcfunction``, vous devez donc écrire un code mcfunction valide.</p> |

### Supprimer
![trigger_modify_1](./srcs/img/data/plugins/trigger/base.png)

Vous pouvez supprimer une ou plusieurs zones simultanément :
| Unique | Multiple |
| --- | --- |
| Cliquer sur le bouton rouge sur la ligne d'une zone de détection. | Sélectionné les zones avec leur bloc de sélection à gauche, puis cliquer sur la corbeille en bas. |
