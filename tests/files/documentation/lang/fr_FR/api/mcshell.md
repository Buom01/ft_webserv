# MCshell

> Ajouter vos propres commandes au système shell

- [Class: MCshell](#class%3A-mcshell)
  - [Format d'une commande](#format-d%E2%80%99une-commande)
  - [add](#add)
  - [parse](#parse)
  - [Example](#example)
---

## Class: MCshell

Cette classe va vous permettre d'ajouter vos propres commande au système shell, et donc par la même occasion les rendre accessibles dans le jeu.

---
### Format d'une commande

Les arguments passés à votre fonction sont sous forme de tableau, correspondant respectivement à :
```js
args[0]: String 	<= "Nom de la commande"
args[1]: String 	<= "Pseudonyme du joueur ayant exécuté la commande"
args[2...]: any[] 	<= "Les autres arguments dans l'ordre d'envoi"
```

Le format d'une commande est standardisé sous le format `JSON`, avec comme squelette :
```js
exports.command = {
	name: 'program_name',
	function: (args) =>
	{
		const num = Number(42);
		return ({
			Command: args[0],
			Player: args[1],
			Number: num,
		});
	},
};
```
Vous pouvez absolument faire tout ce qui est nécessaire à l'intérieur de la fonction, ou alors envoyer les données brutes directement.
Il est obligatoire d'avoir les clés suivantes pour que votre commande fonctionne:
- `Command`: doit contenir le nom de votre commande (soit `args[0]`)
- `Player`: doit contenir le pseudonyme de l'utilisateur (soit `args[1]`)
Sans ces clés, votre commande ne fonctionnera pas correctement.

---
### add

```ts
MCshell.add(json: any): void;
```
Ajouter une nouvelle commande au shell. Il existe deux formats possibles :

- Directement via le format ci-dessus :
```js
MCshell.add({
	name: 'my_program',
	function: (args) =>
	{
		return({
			Command: args[0],
			Player: args[1],
			Args: args[2]
		});
	},
});
```

- Ou alors de façon automatisée en créant un fichier `shell.js` dans le dossier racine de votre plugin, comme ce qui suit (cette méthode est recommandée pour plus de lisibilité):
```js
exports.command = {
	name: 'my_program',
	function: (args) =>
	{
		return({
			Command: args[0],
			Player: args[1],
			Args: args[2]
		});
	},
};
```
De plus cette méthode a l'énorme avantage de pouvoir dans le même fichier créer plusieurs commandes:
```js
exports.command = [
	{
		name: 'one',
		...
	},
	{
		name: 'two',
		...
	},
	{
		name: 'three',
		...
	},
]

```

---
### parse

```ts
MCshell.parse(line: string): JSON;
```
Analyser une ligne et retourner les données si le programme existe

- `line` <span class="required">obligatoire</span> <xmp class="type"><string></xmp> Ligne à analyser
- ++Retourne:++ <xmp class="type"><JSON></xmp> Données préformées, ou `undefined` si erreur

---
### Example

Prenons un exemple, imaginons que vous souhaitez ajouter une entrée de saisie dans un formulaire selon ce que l'utilisateur demande. On décide que :
- `/mapcraft my_plugin Vex345 add_form number` ajoutera un champ d'entrée de nombre
- `/mapcraft my_plugin Vex345 add_form string` ajoutera un champ d'entrée de chaine de caractères

Le squelette de la commande est le suivant:
```js
const myCommand = {
	name: 'my_plugin',
	function: (args) =>
	{
		return ({
			Command: args[0],
			Player: args[1],
			Option: args[2],
			Format: args[3],
		});
	},
}
```

On peut donc faire le code suivant :
```js
const { MCshell, MCipc } = require('mapcraft_api');

MCshell.add(myCommand);

MCipc.receive('Shell:execute-command', (command) =>
{
	if (command.Command !== 'my_plugin')
		return;
	if (command.Option === 'add_form')
	{
		switch (command.Format)
		{
			case 'number':
				console.log('Add number input');
			case 'string':
			default:
				console.log('Add string input');
		}
	}
});
```