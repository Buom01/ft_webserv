## Recette
![recipe_main](./srcs/img/data/plugins/recipe/main.png)
> Créez vos propres recettes ou modifiez celles qui existent déjà.

- [Liste](#Liste)
- [Type](#type)
- [Editeur](#editeur)
- [Boutons](#boutons)

---
### Liste
![recipe_list](./srcs/img/data/plugins/recipe/list.png)

Dans la partie gauche de l'interface, vous trouverez la liste contenant toutes les recettes que vous avez créées, triées par nom de groupe. Pour modifier une recette, il suffit de cliquer dessus pour l'ouvrir dans l'éditeur.

### Type
Vous trouverez ici tous les types de modèles disponibles :
| | |
| --- | --- |
| ![recipe_type_1](./srcs/img/data/plugins/recipe/type/player.png) | ``Établi du joueur`` |
| ![recipe_type_1](./srcs/img/data/plugins/recipe/type/craft_table.png) | ``Établi`` |
| ![recipe_type_1](./srcs/img/data/plugins/recipe/type/furnace.png) | ``Fourneau`` |
| ![recipe_type_1](./srcs/img/data/plugins/recipe/type/blast_furnace.png) | ``Haut fourneau`` |
| ![recipe_type_1](./srcs/img/data/plugins/recipe/type/campfire.png) | ``Feu de camp`` |
| ![recipe_type_1](./srcs/img/data/plugins/recipe/type/smoker.png) | ``Fumoir`` |
| ![recipe_type_1](./srcs/img/data/plugins/recipe/type/stonecutter.png) | ``Tailleur de pierre`` |
| ![recipe_type_1](./srcs/img/data/plugins/recipe/type/smithing.png) | ``Table de forgeron`` |

Pour choisir un des modèles, il suffit de cliquer sur l'icône correspondante. Cependant, lorsque vous créez une recette, celle-ci ne sera pas sauvegardée à moins que vous n'appuyiez sur le bouton ``Créer une recette``.

### Editeur
![recipe_editor_table](./srcs/img/data/plugins/recipe/table.png)

Après avoir choisi le type de modèle, pour modifier le contenu d'une case, cliquez dessus.

![recipe_editor_table](./srcs/img/data/plugins/recipe/modal.png)

Une fenêtre s'ouvre alors contenant une liste de blocs et d'éléments existants. Pour l'ajouter, cliquez sur celui que vous voulez. Pour supprimer le contenu de la case, cliquez sur la poubelle dans l'onglet des bloc.

Selon le type de modèle, deux types de formulaires existent :

![recipe_editor_form_1](./srcs/img/data/plugins/recipe/form_1.png)

- **Informe** : La recette n'aura pas besoin d'un modèle spécifique pour être créée
- **Position exacte** : Le contraire de l'option précédente
- **Groupe** : Définir le groupe dans lequel la recette sera créée (permet de mieux trier les recettes)
- **Nom du fichier de sortie** : Nom de sortie de la recette

![recipe_editor_form_2](./srcs/img/data/plugins/recipe/form_2.png)

- **Gain d'expérience**: Gain d'expérience reçu par le joueur
- **Temps d'attente**: Temps d'attente pour la création de la recette (en secondes)
- **Groupe** : Définir le groupe dans lequel la recette sera créée (permet de mieux trier les recettes)
- **Nom du fichier de sortie** : Nom de sortie de la recette

### Boutons
![recipe_editor_button](./srcs/img/data/plugins/recipe/button.png)

- **Supprimer la recette**: Supprimer la recette sélectionnée
- **Créer la recette**: Créer ou modifier la recette en cours
