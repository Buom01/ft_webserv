# Démarrage

Démarrer la création d'un plugin peut sembler une tâche ardue. Néanmoins ne vous inquiétez pas, à la fin de ce guide, vous disposerez de votre premier plugin entièrement fonctionnel que vous pourrez partager à tous les autres utilisateurs.

- [Prérequis](#pr%C3%A9requis)
- [Créer votre plugin](#cr%C3%A9er-votre-plugin)
  - [Structure d'un plugin](#structure-d%E2%80%99un-plugin)
- [Commencer le développement](#commencer-le-développement)
- [Explication](#explication)
  - [Initialisation](#initialisation)
  - [Composant](#composant)
  - [Shell](#shell)
- [Fichier de template](#fichier-de-template)
- [Distribuer votre plugin](#distribuer-votre-plugin)
---

## Prérequis

Pour développer votre plugin, vous aurez besoin d'installer `Node.js` (16.14.0) et `NPM`:
  - Via un gestionnaire de version, [Windows](https://github.com/coreybutler/nvm-windows/releases), [MacOS / Linux](https://github.com/nvm-sh/nvm/releases/latest) (recommandé)
  - Via l'installateur direct [Node.js](https://nodejs.org)

Pour vérifier que Node.js a été installé correctement, tapez les commandes suivantes dans votre terminal:

```bash
node -v
npm -v
```

Les commandes devraient imprimer respectivement les versions de Node.js et NPM

## Créer votre plugin

Les plugins suivent la même structure que les autres projets sous Node.js. Néanmoins pour vous simplifier grandement la vie, Mapcraft contient un `CLI` (outil en ligne de commande).
<div style="display: inline-flex; align-items: center;">
	<div style="margin-right: .7em; min-width: 70%;">
		<p>Pour y accéder, ouvrer votre application, et aller dans le module <b>Option</b>. Dans l'onglet <b>Général</b>, aller à la partie <b>Outil de développement</b> et cliquer sur le bouton <code>Créer un nouveau plugin</code></p>
	</div>
	<img style="max-width: 30%" src="./srcs/img/data/getting_started/create_plugin.png"/>
</div>

---

<div style="display: inline-flex; align-items: center;">
	<div style="margin-right: .7em; min-width: 70%;">
		<p>Une nouvelle fenêtre va s'ouvrir. Suivez les différentes étapes en remplissant avec les informations demandées:</p>
		<ol>
			<li>Nom de votre plugin</li>
			<li>Nom afficher dans le logiciel</li>
			<li>Version de votre plugin, il est recommandé de suivre <a href="https://docs.npmjs.com/about-semantic-versioning">la logique de NPM</a></li>
			<li>Nom de l'auteur du plugin, le vôtre en toute logique</li>
			<li>Description de votre plugin, une description courte expliquant basiquement ce que fait votre plugin</li>
			<li>Choix de la licence, si vous n'avez aucune idée, prenez la licence par défaut</li>
			<li>Nom du fichier principal, c'est ce fichier qui devra contenir le point d'accès à votre plugin</li>
			<li>Nom du dossier dans lequel les fichiers de langues seront disponible</li>
			<li>Est-ce que votre plugin affichera une notification s'il détecte une commande ?</li>
		</ol>
	</div>
	<img style="max-width: 30%" src="./srcs/img/data/getting_started/cli_create.png"/>
</div>

---
Votre nouveau plugin sera dans le dossier suivant:
- Windows: `%appdata%/Mapcraft/plugins/[nom de votre plugin]` 
- Mac: `~/.config/Mapcraft/plugins/[nom de votre plugin]` ou `$XDG_CONFIG_HOME/Mapcraft/plugins/[nom de votre plugin]`
- Linux: `~/Library/Application Support/Mapcraft/plugins/[nom de votre plugin]`

### Structure d'un plugin
Un plugin de base contient au minimum l'arborescence suivante:

```text
[nom_du_plugin]
  │
  ├─ 🗀 lang
  │   ╰─ *.json
  ├─ 🗀 node_modules
  │   ╰─ **/*
  ├─ index.js
  ├─ LICENCE
  ├─ [nom_du_plugin].tp
  ├─ package.json
  ├─ package-lock.json
  ╰─ shell.js
```

- `lang`: contient les fichiers de langues de votre application, par défaut celui-ci doit avoir `en_US.json`
- `node_modules`: contient les dépendances de votre plugin
- `index.js` ou le nom de votre fichier d'entrée: le fichier d'entrée de votre plugin
- `LICENCE`: la licence de votre plugin
- `[nom_du_plugin].tp`: le fichier de template de votre application, voir [MCtemplate](api/mctemplate)
- `package.json`: contient toutes les informations de votre plugin. Celui-ci est entièrement compatible avec les gestionnaires de paquets
- `package-lock.json`: fichier permettant à npm de reproduire exactement l'installation de vos dépendances
- `shell.js`: fichier permettant l'ajout au shell de Mapcraft de vos propres commandes, voir [MCshell](api/mcshell)

Votre fichier `package.json` devrait ressembler à ceci:
```json
{
    "name": "hello_world",
    "title": "Hello World",
    "version": "0.0.1",
    "author": "John Doe",
    "description": "My wonderful plugin",
    "license": "gpl-3.0",
    "uuid": "e942b9fd-e6b2-4711-960d-7f3211a4a486",
    "icon": "icon.png",
    "isNotification": true,
    "bin": {
        "component": "index.js",
        "command": "hello_world",
        "lang": "lang"
    },
    "dependencies": {
        "mapcraft-api": "^1.8.1"
    }
}
```
Comme vous le constatez, l'api de Mapcraft est déjà installé et directement disponible. Et comme tout projet Node.js, il vous est possible d'installer tous les paquets dont vous avez besoin pour son développement. Néanmoins la seule dépendence obligatoire est `mapcraft-api`.

Cependant n'oubliez pas que les `devDependencies` ne seront pas inclus pour l'utilisateur.

---
## Commencer le développement

Pour commencer, il faut bien comprendre comment Mapcraft va utiliser votre plugin.

Celui-ci va dans un premier temps lire votre fichier `package.json` pour en extraire toutes les informations, et initialiser votre plugin comme un module (via un `require`).
Pour ce faire, le point d'entrée doit être une ++classe++ avec une fonction appelée `main`, sans argument; puis celle-ci doit être exportée.

```ts
class Component
{
	static main()
	{
		...
	}
}

module.exports = Component;
```

L'interface de Mapcraft possède une [HTMLDivElement](https://developer.mozilla.org/en-US/docs/Web/API/HTMLDivElement) avec l'identifiant `content`, dans laquelle tous les plugins installés sont affichés.
C'est-à-dire que pour commencer à afficher votre plugin, vous devrez a minima rendre la base de celui-ci dans cette div, exemple:
|||
|---|---|
| ✓ | <pre><code class="language-ts">TEMPLATE.render(document.getElementById('content'), 'hello_world.tp', LANG.Data);</code></pre> |
| ✗ | <pre><code class="language-ts">TEMPLATE.render(document.getElementById('random-id'), 'hello_world.tp', LANG.Data);</code></pre> |

Par la suite, vous pourrez accéder à tous les Element que vous créer, ce qui veut dire que si dans votre template vous disposez d'une div avec l'identifiant `hello`, vous pourrez faire le code suivant sans aucun problème:
<pre><code class="language-ts">document.getElementById('hello')</code></pre>

Vous aurez aussi accès à absolument [toutes les API](https://developer.mozilla.org/en-US/docs/Web/API) disponibles dans un navigateur classique, si vous savez coder un site internet, vous savez faire un plugin.
Néanmoins pour éviter tous conflits (imaginons que par hasard vous créer un élément avec un identifiant qui existe déjà dans l'interface du logiciel), il est fortement recommandé de travailler uniquement sur la `div content`, comme ceci :
```ts
const CONTENT = document.getElementById('content');

CONTENT.getElementById('my-div').addEventListener('click', () => console.log('click in content'), false);
CONTENT.querySelectorAll('a.my-class').forEach((el) => ...);
...
```

De cette manière, vous ne modifiez uniquement que vos propres codes sans risque d'interférer avec l'interface du logiciel

## Explication

Pour aller plus loin, ouvrez le fichier d'entrée de votre application (ici `index.js`). Nous allons étape par étapes comprendre le code écrit et son utilité.

### Initialisation
<div class="gs-flex">
	<div>
		<ol>
			<li>Importer les modules suivants:
				<ul>
					<li><b>Mapcraft</b> Accéder à toutes les informations du logiciel</li>
					<li><b>MCipc</b> Envoyer et/ou réceptionner des informations du processus principal</li>
					<li><b>MCtemplate</b> Généré, afficher et modifier l'interface</li>
					<li><b>MCutilities</b> Contient un ensemble de fonction utiles</li>
				</ul>
			</li>
			<li>Récupérer tout le contenu du fichier <code>package.json</code>. Cela permettra d'avoir accès à toutes les informations nécessaires à son bon fonctionnement
				<p>Explication des arguments:</p>
				<ul>
					<li>Emplacement dans lequel se trouve le fichier, ici à la racine du plugin, soit <code>__dirname</code></li>
				</ul>
			</li>
			<li>Initialisation d'une nouvelle instance de MCtemplate.
				Explication des arguments:
				<ul>
					<li>Emplacement de tous nos fichiers template <code>*.tp</code>, ici à la racine du plugin, soit <code>__dirname</code></li>
					<li>UUID du plugin pour permettre à notre instance de seulement géré ces éléments (cela permet à plusieurs instances de cohabiter sans problème)</li>
				</ul>
			</li>
			<li>Création d'une variable qui contiendra les données des fichiers de langues</li>
			<li>Création d'une fonction qui va simplement mettre à jour les données de la variable <code>LANG</code> selon la langue choisi par l'utilisateur
				<p>La fonction appelé renvoi les données préformater dans le bon format de langue</p>
				Explication des arguments:
				<ul>
					<li>Emplacement du plugin, ici <code>__dirname</code></li>
					<li>Langue choisie par l'utilisateur, récupérable via <code>Mapcraft.config.Env.Lang</code></li>
					<li>Dossier dans lequel ce trouve les fichiers de langue, celui-ci étant défini dans le <b>package.json</b>, on peut y accéder via <code>PACKAGE.bin.lang</code></li>
				</ul>
			</li>
		</ol>
	</div>
	<pre><code class="language-ts">
a. const { Mapcraft, MCipc, MCtemplate, MCutilities } = require('mapcraft-api');
</br>
b. const PACKAGE = MCutilities.getPackage(__dirname);
c. const TEMPLATE = new MCtemplate(__dirname, PACKAGE.uuid);
d. let LANG;
e. const updateLANG = () =>
{
	LANG = MCutilities.getLang(__dirname, Mapcraft.config.Env.Lang, PACKAGE.bin.lang);
};
	</code></pre>
</div>

---
### Composant
<div class="gs-flex">
	<div>
		<p>Le composant principal de votre plugin est celui qui sera exporté et utilisé par Mapcraft</p>
		<p>Celui-ci doit respectées plusieurs points pour fonctionner:</p>
		<ol style="list-style-type: number">
			<li>Il doit s'agir d'une classe</li>
			<li>La fonction d'entrée est et sera toujours nommé <code>main</code>. Celle-ci ne doit avoir aucun argument</li>
			<li>La classe peut contenir un constructeur, mais néanmoins Mapcraft n'initialise jamais une classe avec <code>new</code>
				<p>Si vous décidez d'implémenter un constructeur, exporter votre classe comme ceci:</p>
				<pre><code class="language-ts">module.exports = new Component();</code></pre>
			</li>
		</ol>
		<hr>
		<p>Détaillons-plus le code du Composant qui nous intéresse.</p>
		<ol>
			<li>On appel la fonction précédemment créée pour pouvoir obtenir les données correspond à la langue sélectionnée par l'utilisateur</li>
			<li>Notre instance de MCtemplate va être utilisée pour rendre votre template dans l'élément sélectionné.
				<p>L'affichage d'un plugin est <b>obligatoirement</b> rendu dans l'élément de la page identifié par l'ID <code>content</code></p>
				<img style="width: 50%;" src="./srcs/img/data/getting_started/content.png"/>
				<p>Explication des arguments:</p>
				<ul>
					<li>Element de la page dans lequel le template sera rendu, sachant qu'il s'agit du template principal, on le rendra dans la <b>div.#content</b></li>
					<li>Fichier de template choisi, avec l'extension</li>
					<li>Donné JSON valide pour remplacer les variables du template par celle indiquée. Ici on utilise directement les variables contenues dans LANG</li>
				</ul>
			</li>
		</ol>
	</div>
	<pre><code class="language-ts">
class Component
{
	static main()
	{
a.		updateLANG();
b.		TEMPLATE.render(document.getElementById('content'), 'hello_world.tp', LANG.Data);
	}
}
	</code></pre>
</div>

---
### Shell
<div class="gs-flex">
	<div>
		<p>La partie Shell n'est pas obligatoire. Si vous souhaitez savoir comment fonctionne la création d'une nouvelle commande, dirigez-vous sur la page dédiée au module <a href="api/mcshell">MCshell</a></p>
		<p>Pour pouvoir détecter si un utilisateur à rentrer dans le chat du jeu une commande liée à votre plugin, vous allez devoir "écouter" en permanence le canal <code>Shell:execute-command</code>. Ce canal envoie une donnée à partir du moment où n'importe quelle commande a été détectée, à vous de savoir s'il s'agit bien de la vôtre</p>
		<ol>
			<li>Pour éviter de refaire à chaque fois un "if", on récupère le nom de la commande défini dans le <code>package.json</code> de votre plugin. Si ce champ n'est pas défini, par défaut c'est le nom de votre plugin qui sera pris</li>
			<li>On écoute sur le canal si une commande est reçue via la fonction receive (<a href="https://nodejs.org/api/fs.html#callback-example">Callback</a>)</li>
			<li>On vérifie que la commande est bien celle qui nous intéresse en vérifiant son nom via <code>[name_of_args].Command</code>, si oui on continue</li>
			<li>On crée une alerte de succès, en affichant sous forme de String le retour de la fonction</li>
		</ol>
	</div>
	<pre><code class="language-ts">
a.	const COMMAND = (MANIFEST.bin.command) ? MANIFEST.bin.command : MANIFEST.name;
b.	MCipc.receive('Shell:execute-command', (command) =>
	{
c.		if (command.Command !== COMMAND)
			return;
d.		MCutilities.createAlert('success', document.getElementById('custom-alert'), `Receive command ${JSON.stringify(command)}`);
	});
	</code></pre>
</div>

---
## Fichier de template

Le format des templates est décrit entièrement dans la page correspondante de la documentation au module [MCtemplate, à la rubrique Détails](api/mctemplate#détails)

---
## Distribuer votre plugin

Ça y est, vous avez fini votre plugin et êtes prêt à le rendre disponible au monde. Pour ce faire, vous allez devoir le compresser dans un format spécifique, le `.mapcraft`.

<div style="display: inline-flex; align-items: center;">
	<div style="margin-right: .7em">
		<p>Pour ce faire, ouvrer votre application, et aller dans le module <b>Option</b>. Dans l'onglet <b>Général</b>, aller à la partie <b>Outil de développement</b> et cliquer sur le bouton <code>Empaquetage d'un plugin</code></p>
		<p>Après avoir sélectionné votre plugin dans la liste, celui-ci sera converti. Selon sa taille cela peut prendre quelques secondes comme quelques minutes</p>
	</div>
	<img style="max-width: 30%" src="./srcs/img/data/getting_started/package_plugin.png"/>
</div>

L'archive sera disponible dans le dossier de votre plugin:
- Windows: `%appdata%/Mapcraft/plugins/[nom de votre plugin]/[nom de votre plugin].mapcraft` 
- Mac: `~/.config/Mapcraft/plugins/[nom de votre plugin]/[nom de votre plugin].mapcraft` ou `$XDG_CONFIG_HOME/Mapcraft/plugins/[nom de votre plugin]/[nom de votre plugin].mapcraft`
- Linux: `~/Library/Application Support/Mapcraft/plugins/[nom de votre plugin]/[nom de votre plugin].mapcraft`

Fournissez tout simplement cette archive aux utilisateurs pour qu'ils puissent facilement l'installer chez eux.

---
### Félicitation, vous êtes enfin prêt à faire vos premières armes, votre seule limite est votre imagination !
