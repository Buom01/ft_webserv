## Musique
![music_main](./srcs/img/data/plugins/music/main.png)
> Gérer les musiques, les bruitages ou les dialogues

- [Ajouter](#ajouter)
- [Modifier](#modifier)
- [Supprimer](#supprimer)
---
### Ajouter

![music_main](./srcs/img/data/plugins/music/add.png)

Cliquer sur ``Ajouter une musique``, puis sur la nouvelle fenêtre glisser un fichier audio en format ``.ogg``[^1]. Aucun autre format n'est autorisé (Minecraft n'autorise uniquement que ce format).
Ajouter un nom ainsi que le dossier dans lequel la musique sera sauvegarder.

---

### Modifier
![music_main](./srcs/img/data/plugins/music/form.png)

Vous trouverez ci-dessous une liste exhaustive des options disponibles :
- **Sound Name** : Nom de la musique
- **category** : l'option est dépréciée depuis la 1.10, mais vous permettra de mieux catégoriser votre musique
- **replace** : si actif, la musique remplacera la musique par défaut avec le même nom, ***non actif*** par défaut
- **subtitle** : sous-titre de la musique si l'option ``Afficher les sous-titres`` est actif dans le jeu, ***Optionnel***
- **attenuation_distance** : Modifie la distance d'atténuation de la musique, ***16*** par défaut
- **pitch** : définit la hauteur du son, ***1.0*** par défaut
- **preload** : indique que la musique doit être préchargée au moment où le pack de ressource est chargé, ***non actif*** par défaut
- **stream** : permet de lire la musique en mode stream. Recommandé pour les musiques de taille supérieur à 4 secondes. <br/> Dans le jeu un maximum de 4 instances de la même musique est possible avec cette option, contre un nombre très supérieur sans, ***non actif*** par défaut
- **type** : deux valeurs sont disponibles, ***sound*** par défaut :

  | Sound | Event |
  | --- | --- |
  | La valeur de **name** sera interprété comme le nom du fichier | La valeur de ***name*** sera interprété comme le nom d'un événement déjà défini dans le jeu. |

- **volume** : le volume du son dans le jeu, par défaut à ***1.0***
- **weight** : la probabilité que la musique soit sélectionnée si la musique est en ``event``, ***1.0*** par défaut
---
### Supprimer

Sélectionner votre musique dans la liste, descendez tout en bas de la zone de modification et cliquer sur ``Supprimer la musique``

[^1]: https://fr.wikipedia.org/wiki/Ogg
