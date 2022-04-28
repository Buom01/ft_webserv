# MCutilities

> Contient un ensemble de fonction très utile dans votre vie de développeur

- [Class: MCutilities](#class%3A-mcutilities)
  - [checkIsLegalString](#checkislegalstring)
  - [nextLetter](#getnextcharacterinalphabet)
  - [generateENV](#generateenv)
  - [download](#download)
  - [isEmptyDir](#isemptydir)
  - [getLang](#getlang)
  - [getPackage](#getmanifest)
  - [getDataGameElement](#getdatagameelement)
  - [createAlert](#createalert)
---

## Class: MCutilities

### checkIsLegalString
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCutilities.checkIsLegalString(string: string): boolean;
```
Renvoie une valeur booléenne qui indique si la chaîne de caractères est valide au format Minecraft.

- `string` <span class="required">obligatoire</span> <xmp class="type"><string></xmp> Chaîne à vérifier
- ++Retourne++: <xmp class="type"><boolean></xmp> `false` si la chaîne contient un caractère non autorisé.

---
### nextLetter
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCutilities.nextLetter(char: string): string;
```
Obtenir le caractère suivant dans l'alphabet

- `char` <span class="required">obligatoire</span> <xmp class="type"><string></xmp> Caractère courant
- ++Retourne++: <xmp class="type"><boolean></xmp> Caractère suivant

```ts
for (let char = 'a'; char != 'z'; char = MCutilities.nextLetter(char))
	console.log(`${char}-`);
/* a-b-c-d-e-f-g-[...] */
```

---
### generateENV
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCutilities.generateENV(void): void;
```
Générer l'environnement du système pour l'application

---
### download
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCutilities.download(url: string, destination: string, callback: Function): void;
```
Téléchargement asynchrone de fichier depuis le web, accepte les url `http` et `https`.

Cette fonction ne vérifie pas le contenu du fichier téléchargé, c'est à vous d'effectuer les vérifications nécessaires afin de ne pas compromettre la sécurité de l'utilisateur.

- `url` <span class="required">obligatoire</span> <xmp class="type"><string></xmp> url du fichier à télécharger
- `destination` <span class="required">obligatoire</span> <xmp class="type"><string></xmp> chemin de la destination du fichier
- `callback` <span class="required">obligatoire</span> <xmp class="type"><Function></xmp> fonction de rappel avec (error)

```js
MCutilities.download('https://hello.com/my_file.png', 'my_new_file', (err) => {
	if (err)
		throw new Error(err.code, err.message);
	// rest of callback
});
```

---
### isEmptyDir
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCutilities.isEmptyDir(path: any): boolean;
```
Vérifier si le dossier est vide

- `path` <span class="required">obligatoire</span> <xmp class="type"><string></xmp> chemin vers le dossier
- ++Retourne++: <xmp class="type"><boolean></xmp> `true` s'il est vide

---
### getLang
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCutilities.getLang(dirname: string, langPath: string, defaultDir?: string): JSON;
```
Obtenir la langue du composant

- `dirname` <span class="required">obligatoire</span> <xmp class="type"><string></xmp> Chemin du répertoire du composant (`__dirname` recommandé)
- `langPath` <span class="required">obligatoire</span> <xmp class="type"><string></xmp> Nom de la langue sélectionnée, ou `MC.config.Env.Lang` pour une valeur prédéfinie.
- `defaultDir` <span class="optional">optionel</span> <xmp class="type"><string></xmp> Répertoire par défaut du dossier `lang`
- ++Retourne++: <xmp class="type"><JSON></xmp> Données JSON du fichier de langue, ou `undefined` en cas d'erreur

---
### getPackage
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCutilities.getPackage(dirname: string): JSON;
```
Récupération des données de `package.json`

- `dirname` <span class="required">obligatoire</span> <xmp class="type"><string></xmp> Dossier dans lequel vous souhaitez effectuer la recherche
- ++Retourne++: <xmp class="type"><JSON></xmp> Données JSON de `package.json`, ou `undefined` en cas d'erreur

---
### getDataGameElement
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCutilities.getDataGameElement(type: string, minecraftVersion?: any): JSON;
```
Récupération de données sur les éléments du jeu

- `type` <span class="required">obligatoire</span> <xmp class="type"><string></xmp> Type de données à récupérer (`biomes`, `blocks`, `effects`, `enchantements`, `entities`, `items`, `potions`, `structures`, `tags`, `triggers`)
- `minecraftVersion` <span class="required">obligatoire</span> <xmp class="type"><string></xmp> La version de Minecraft souhaitée, par défaut à la plus haute version supportée par Mapcraft
- ++Retourne++: <xmp class="type"><JSON></xmp> Données JSON des éléments, ou `undefined` en cas d'erreur

---
### createAlert
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCutilities.createAlert(type: string, DOMelement: Element, str: string): void;
```
Imprimer une alerte préformatée dans un élément HTML

- `type` <span class="required">obligatoire</span> <xmp class="type"><string></xmp> Type d'erreur (`primary`, `success`, `warning`, `danger`)
- `DOMelement` <span class="required">obligatoire</span> <xmp class="type"><Element></xmp> Élément dans lequel l'alerte sera affichée
- `str` <span class="required">obligatoire</span> <xmp class="type"><string></xmp> Message d'erreur

```js
MCutilities.createAlert('success', document.getElementById('custom-alert'), 'Command is undefined');
```