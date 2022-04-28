## Progrès

![Alt image](./srcs/img/data/plugins/advancement/main.png)

> Créez vos propres progrès

- [Sélectionner un progrès](#sélectionner-un-progrès)
- [Créer un nouveau progrès](#créer-un-nouveau-progrès)
- [Modifier un progrès](#modifier-un-progrès)
  - [Racine](#racine)
  - [Affichage](#affichage)
  - [Critères](#critères)
  - [Conditions requises](#conditions-requises)
  - [Récompenses](#récompenses)
- [Générer un progrès](#générer-un-progrès)
- [Supprimer un progrès](#supprimer-un-progrès)
---
### Sélectionner un progrès

<img src="./srcs/img/data/plugins/advancement/list.png" style="width: 250px;"/>

Pour sélectionner un progrès, cliquez sur celui que vous voulez dans la liste. Le progrès sélectionné aura une flèche affichée sur son côté gauche.

---
### Créer un nouveau progrès

<img src="./srcs/img/data/plugins/advancement/list.png" style="width: 250px;"/>

En haut de la liste, il suffit de cliquer sur le bouton ``Nouveau progrès``.

### Modifier un progrès
![modify_advancement](./srcs/img/data/plugins/advancement/graph.png)

Lorsqu'un progrès est sélectionné, un graphique composé de nœuds et de formulaires apparaît sur la droite :
1. Nom du progrès en cours (ne sera pas affiché dans le jeu).
2. Générer un progrès
3. Supprimer le progrès
4. Racine du progrès, ce nœud ne peut pas être supprimé.
5. Nœud sélectionné
6. Ajouter un parent au nœud sélectionné
7. Ajouter un enfant au nœud sélectionné
8. Sauvegarder le nœud sélectionné

Le formulaire situé sous cette zone vous permettra de modifier le nœud sélectionné.

#### Racine
![root_advancement](./srcs/img/data/plugins/advancement/root.png)

> Modifiez les informations d'affichage de votre progrès actuel. Ces données sont appliquées à tous les nœuds
- **Espace de nom** : Espace de nom du jeu 
- **Image de fond** : Texture d'arrière-plan appliquée dans la fenêtre correspondante

#### Affichage
![display_advancement](./srcs/img/data/plugins/advancement/display.png)

> Modifier l'affichage du nœud sélectionné dans le jeu
1. Titre du nœud :
  - **Titre**: Titre du nœud
  - **Couleur du titre**: Couleur du titre
  - **En gras / Italique / Souligné / Barré / Obfusqué**: Formatage du titre
2. Description du nœud :
  - **Description**: Description du nœud
  - **Couleur de la description**: Couleur de la description
  - **En gras / Italique / Souligné / Barré / Obfusqué**: Formatage de la description
3. Icône de nœud, peut être un bloc ou un élément.
4. Type de cadre :
   
   <img src="./srcs/img/data/plugins/advancement/frame.png" style="image-rendering: crisp-edges;image-rendering: pixelated;-ms-interpolation-mode: nearest-neighbor;"/>
   
   De gauche à droite : Normal / Objectif / Défi
5. Données NBT
6. Affiche une notification sur l'écran du joueur si un joueur réussi le progrès
7. Annonce dans le tchat si un joueur obtient la progression
8. Aucune notification ne sera affichée

#### Critères
![criteria_advancement](./srcs/img/data/plugins/advancement/criteria.png)

> Ajouter un critère au nœud courant

Pour ajouter un nouveau critère, cliquez sur le bouton en bas de la zone ``Ajouter un déclencheur``.
Un nouveau bloc apparaîtra alors, avec un nom pré-rempli généré aléatoirement. Vous pouvez le modifier, mais il ne doit pas y avoir deux noms équivalents sur le même nœud.
Dans la barre de recherche, vous pouvez sélectionner un des nombreux déclencheurs existants, et le personnaliser complètement. Une fois sélectionné, un formulaire apparaît en bas.
Pour supprimer un critère, cliquez sur la croix en haut à droite du bloc.

#### Conditions requises
![criteria_advancement](./srcs/img/data/plugins/advancement/requirements.png)

> Définir les modalités de validation du progrès.

L'idée derrière ces exigences est simple, il s'agit de savoir avec quels critères le progrès sera validé. Il est tout à fait possible de créer plusieurs façons. Par exemple, imaginons que nous ayons les critères A, B et C. Je peux décider que la progression sera réalisée si le joueur valide A et B OU B et C.

#### Récompenses
![criteria_advancement](./srcs/img/data/plugins/advancement/rewards.png)

> Définissez les récompenses et les actions à effectuer en cas de succès.

- **Recettes** : Ajoutez des recettes que le joueur recevra en récompense (il recevra le résultat des recettes mais ne les débloquera pas)
- **Tables de butin** : Ajoutez les tables de butin que le joueur recevra en récompense
- **Expériences** :  Indiquer ce que le joueur recevra en expérience
- **Fonction** : Écrivez une fonction Minecraft valide qui sera exécutée

---
### Générer un progrès

![criteria_advancement](./srcs/img/data/plugins/advancement/butons.png)

Sélectionnez un progrès [(voir ici)](#sélectionner-un-progrès); puis cliquez sur le bouton ``Générer l'intrégalité du progrès`` et validez votre demande. Selon la complexité du progrès, cela peut prendre quelques secondes.

---
### Supprimer un progrès

![criteria_advancement](./srcs/img/data/plugins/advancement/butons.png)

Sélectionnez un progrès [(voir ici)](#sélectionner-un-progrès); puis cliquez sur le bouton ``Supprimer l'intrégalité du progrès`` et validez votre demande. Attention, cela supprimera également les fichiers du Datapack correspondant s'il a été généré.
