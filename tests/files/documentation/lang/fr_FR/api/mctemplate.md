# MCtemplate

> Générer, afficher et modifier l'interface

- [Class: MCtemplate](#class%3A-mctemplate)
  - [constructor](#constructor)
  - [render](#render)
  - [getRaw](#getraw)
  - [renderRaw](#renderraw)
  - [parseRaw](#parseraw)
  - [updateLang](#updatelang)
  - [cleanNode](#cleannode)
- [Détails](#détails)
  - [Format](#format)
  - [Variables](#variables)
  - [Langue](#langue)
  - [Un exemple concret](#un-exemple-concret)

---

## Class: MCtemplate
Mapcraft utilise une approche fortement inspirée de plusieurs autres systèmes de templating.

Ce système tourne autour de deux axes :
1. Vos templates fonctionnent autour de `component`, représenté par des fichiers avec l'extension `.tp`
2. Vous pouvez générer au sein [d'élément](https://developer.mozilla.org/en-US/docs/Web/API/Element) vos `component`, la classe s'occupera automatiquement d'insérer les codes nécessaires pour fonctionner.

---
### constructor
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
constructor(directory: string, preGenerateID?: string);
```
Créer une nouvelle instance

- `directory` <span class="required">obligatoire</span> <xmp class="type"><string></xmp> Chemin vers le dossier où ce trouve les `components`.
- `preGenerateID` <span class="optional">optional</span> <xmp class="type"><string></xmp> Force l'utilisation de son propre identifiant. Attention, si cette fonction est mal utilisée, elle peut générer de nombreux dysfonctionnements

---
### render
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
render(DOMelement: Element, template: string, args: JSON): void;
```
Rendu du template dans un Élément

- `DOMelement` <span class="required">obligatoire</span> <xmp class="type"><Element></xmp> Élément dans lequel le template sera ajouté
- `template` <span class="required">obligatoire</span> <xmp class="type"><string></xmp> Nom du fichier du template avec l'extension
- `args` <span class="required">obligatoire</span> <xmp class="type"><JSON></xmp> Json valide pour la ou les variables de remplacement, ou `null`

---
### getRaw
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
getRaw(template: string): string;
```
Obtenir le code HTML brute du template

- `template` <span class="required">obligatoire</span> <xmp class="type"><string></xmp> Nom du fichier du template avec l'extension
- ++Retourne:++ <xmp class="type"><string></xmp> Données brutes du template

---
### renderRaw
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
renderRaw(DOMelement: Element, rawHTML: string, template: string, args: JSON): void;
```
Rendre les éléments générés dans des formats HTML bruts ; les données doivent être du HTML valide

- `DOMelement` <span class="required">obligatoire</span> <xmp class="type"><Element></xmp> Élément sur lequel le template sera ajouté
- `rawHTML` <span class="required">obligatoire</span> <xmp class="type"><string></xmp> HTML brut
- `template` <span class="required">obligatoire</span> <xmp class="type"><string></xmp> Nom du fichier du template avec l'extension
- `args` <span class="required">obligatoire</span> <xmp class="type"><JSON></xmp> Json valide pour la ou les variables de remplacement, ou `null`

---
### parseRaw
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
parseRaw(HTML: string, args: JSON): string;
```
Analyse du code HTML brut avec des variable(s)

- `HTML` <span class="required">obligatoire</span> <xmp class="type"><string></xmp> HTML brute
- `args` <span class="required">obligatoire</span> <xmp class="type"><JSON></xmp> Json valide pour la ou les variables de remplacement, ou `null`
- ++Retourne:++ <xmp class="type"><string></xmp> HTML brut avec les variables remplacées

---
### updateLang
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
updateLang(DOMelement: Element, args: JSON): void;
```
Mise à jour de la langue d'un `component` spécifique

- `DOMelement` <span class="required">obligatoire</span> <xmp class="type"><Element></xmp> Élément où ce trouve le template à mettre à jour
- `args` <span class="required">obligatoire</span> <xmp class="type"><JSON></xmp> Json valide pour la ou les variables de remplacement, ou `null`

---
### cleanNode
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
cleanNode(node: Element, RemoveParent?: boolean): void;
```
Nettoyer correctement un Nœud (https://developer.mozilla.org/en-US/docs/Web/API/Node)

- `node` <span class="required">obligatoire</span> <xmp class="type"><Element></xmp> Nœud à supprimer
- `RemoveParent` <span class="optional">optional</span> <xmp class="type"><boolean></xmp> Si `true`, la fonction supprime le nœud après avoir supprimé le ou les enfants. `false` par défaut

---
## Détails

### Format
Le format des templates comportes trois tags, dans lesquels vous devez écrire vos codes :
- [HTML]
- [CSS]
- [JS]

Les tags fonctionnent exactement comme ceux sur HTML (ouvrant et fermant), voici un exemple pour mieux comprendre:

```html
[HTML]
<div class="random">
	<h1>My title</h1>
	<p></p>
</div>
[/HTML]

[CSS]
.random {
	color: aqua;
}
[/CSS]

[JS]
document.querySelector('div.random p').innerText = 'Hello world';
[/JS]
```

Chaque tag est totalement optionnel, c'est-à-dire qu'il vous est possible de seulement écrire de l'HTML et du CSS sans JS.

Le texte en dehors de ces trois tags ne sera pas pris en compte lors de leur lecture, vous pouvez donc écrire des commentaires facilement.

---
### Variables
Les templates peuvent contenir des variables, que vous passerez en arguments lors de leur génération sous le format JSON.

Le format des variables est très simple: **{**_var_name_**}**

`mytemplate.tp`:
```html
[HTML]
<div class="{my_var_1}">
	<h1>{my_var_2}</h1>
</div>
[/HTML]
```

Cela donnera:

```js
const args = {
	my_var_1: 'variable-css-class',
	my_var_2: 'My wonderful text'
};

MCtemplate.render(document.getElementById('hello'), 'mytemplate.tp', args);
```

---
### Langue
MCtemplate supporte nativement les modules multilangues. Par défaut, la langue définie est `en_US`, néanmoins il est possible de façon simple d'en ajouter.

Dans le dossier de votre plugin à l'intérieur du dossier `lang`, ajouter les fichiers respectant les nomenclatures supportées suivantes:
<div style="display:flex; justify-content:space-evenly;">
	<div class="channel">
		<div class="send">en_US</div>
		<div class="receive">English</div>
	</div>
	<div class="channel">
		<div class="send">fr_FR</div>
		<div class="receive">Français</div>
	</div>
</div>

Un fichier de langue a pour squelette:
```json
{
	"Title": "My plugin",
	"Icon": "cog",
	"Notification" : "My plugin generate a notification",
	"Data" : {
		"One": "One",
		"Deep": {
			"MoreDeep": {
				"Two": "Two",
			},
			"Three": "Three",
		},
		...
	},
}
```
Allons plus en détail :
- `Title` Titre de votre plugin qui apparaitra dans le dashboard et le menu principal.
- `Icon`: Icône qui complémentera votre titre. La liste des icônes est a retrouver [ici](https://getuikit.com/docs/icon#library).
- `Notification`: Texte qui apparaitra lors d'une notification sur votre plugin.
- `Data`: Contient tous les textes que vous souhaitez traduire dans plusieurs langues.

Dans vos templates, il vous suffit d'ajouter la balise `lang` ainsi que son chemin en argument, à vos balises html pour pouvoir l'utiliser.<br/>
Reprenons l'exemple en haut, et appliquons-le à un template type:
```html
<h1 lang="One"></h1>
<h2 lang="Deep.MoreDeep.Two"></h2>
<h3 lang="Deep.Three"></h3>
```

---
### Un exemple concret
Pour mieux comprendre le fonctionnement de la classe, nous allons créer plusieurs components et les utiliser concrètement. Dans notre exemple, nous allons créer une To-Do-List.

#### Etape 1
Pour commencer, nous allons devoir créer nos `components`.

`main.tp`
```html
[HTML]
<div class="main">
	<div class="header">
		<img src="./img/logo.png" />
		<h4 lang="MainTitle"></h4>
	</div>
	<div id="form"></div>
	<button id="add-to-do" lang="AddButton"></button>
</div>
[/HTML]
[CSS]
.main {
	display: flex;
	flex-direction: column;
}
[/CSS]
```

`input.tp`
```html
[HTML]
<input type="text" value="{ID}"/>
<span class="close">X</span>
[/HTML]
[CSS]
.input {
	display: flex;
	flex-direction: row;
	flex-wrap: nowrap;
}
.close {
	color: red;
}
[/CSS]
```

#### Etape 2
Créons un module disponible en deux langues, `en_US` et `fr_FR`. Dans le dossier `lang`, nous allons créer deux fichiers:

`en_US.json`
```json
{
	"Title": "To-do list",
	"Icon": "list",
	"Notification" : "",
	"Data" : {
		"MainTitle": "My to-do list",
	},
}
```

`fr_FR.json`
```json
{
	"Title": "Liste des tâches",
	"Icon": "list",
	"Notification" : "",
	"Data" : {
		"MainTitle": "Ma liste des tâches",
	},
}
```

#### Etape 3
Initialisé la classe. Pour ce faire, il suffit de construire une nouvelle instance de la classe en indiquant le dossier dans lequel vos `components` sont placés.

1. Si vos `components` sont placés dans le même dossier que votre fichier principal, une variable globale très pratique indique le dossier courant (`__dirname`). Nous allons donc l'utiliser:
```js
const { MCtemplate } = require('mapcraft-api');

const TEMPLATE = new MCtemplate(__dirname);
```

2. A contrario, si ceux-ci sont placés dans un autre dossier, il est fortement recommandé d'utiliser [path](https://nodejs.org/api/path.html#pathjoinpaths):
```js
const path = require('path');
const { MCtemplate } = require('mapcraft-api');

const TEMPLATE = new MCtemplate(path.join(__dirname, 'template'));
```

#### Etape 4
Nous allons afficher le `components` principal dans la div **content**.<br/>

Dans le même temps, nous allons mettre à jour les balises `lang` présente avec les données que nous auront récupérées via la fonction `GetLang` de MCutilities.

```js
let LANG = MCutilities.getLang(__dirname, Mapcraft.config.Env.Lang);
TEMPLATE.render(document.getElementById('content'), 'main.tp', null);
TEMPLATE.updateLang(document.getElementById('content'), LANG.Data);
```

Puis ensuite, nous allons ajouter un `Event Listener` sur le bouton.
Lors du clic sur celui-ci, nous allons ajouter une nouvelle ligne à notre liste.

Nous en profiterons pour faire en sorte que si l'utilisateur clique sur la croix, la ligne soit supprimer.

```js
let lineID = Number(0);
const List = document.getElementById('form');
document.getElementById('add-to-do').addEventListener('click', (e) =>
{
	e.preventDefault();
	const NewLine = document.createElement('div');
	NewLine.classList.add('input');
	TEMPLATE.render(NewLine, 'input.tp', { ID: lineID++ });
	List.appendChild(NewLine);
	Newline.querySelector('span').addEventListener('click', () => TEMPLATE.cleanNode(NewLine, true));
});
```

#### Etape 5
Votre module est prêt à l'usage. Voici le code complet de votre main:

```js
const { MCtemplate } = require('mapcraft-api');
const TEMPLATE = new MCtemplate(__dirname);
let LANG = MCutilities.getLang(__dirname, Mapcraft.config.Env.Lang);

TEMPLATE.render(document.getElementById('content'), 'main.tp', null);
TEMPLATE.updateLang(document.getElementById('content'), LANG.Data);

let lineID = Number(0);
const List = document.getElementById('form');
document.getElementById('add-to-do').addEventListener('click', (e) =>
{
	e.preventDefault();
	const NewLine = document.createElement('div');
	NewLine.classList.add('input');
	TEMPLATE.render(NewLine, 'input.tp', { ID: lineID++ });
	List.appendChild(NewLine);
	Newline.querySelector('span').addEventListener('click', () => TEMPLATE.cleanNode(NewLine, true));
});
```
