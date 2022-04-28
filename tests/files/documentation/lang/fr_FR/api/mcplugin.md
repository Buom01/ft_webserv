# MCplugin

> Initialiser un ensemble de plugin pour pouvoir les utiliser dans l'application

- [Class: MCplugin](#class%3A-mcplugin)
  - [Initialize](#initialize)
  - [instance](#instance)
  - [component](#component)
  - [active](#active)
  - [toogle](#toogle)
  - [lang](#lang)
  - [default](#default)
  - [listComponents](#listcomponents)
---

## Class: MCplugin

Cette classe va vous permettre d'instancier vos modules et d'y accéder au sein d'une même liste. Dans le cas classique, vous n'aurez aucune raison de l'utiliser, tout étant fait automatiquement.

Néanmoins imaginons que vous souhaitez créer plusieurs modules en un seul écosystème et y accéder facilement, cette classe vous sera utile.

### Initialize
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
const { MCplugin } = require('mapcraft-api');
const PluginsList = new MCplugin(directory?: string);
```
Initialiser la classe

- `directory` <span class="optional">optionel</span> <xmp class="type"><string></xmp> Dossier dans lequel se trouvent les plugins, par défaut `path_to_builtin_plugins`

---
### instance
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
instance(Name: string): any;
```
Obtenir l'instance d'un composant
- `Name` <span class="required">obligatoire</span> <xmp class="type"><string></xmp> Nom du composant
- ++Return:++ Instance <xmp class="type"><function></xmp> du composant, ou `undefined` en cas d'erreur

---
### component
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
component(Name: string): any;
```
Obtenir un composant
- `Name` <span class="required">obligatoire</span> <xmp class="type"><string></xmp> Nom du composant
- ++Return:++ <xmp class="type"><Function></xmp> s'il existe, ou `undefined`

---
### active
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
active(Name: string): any;
```
Vérifier si le composant est actif
- `Name` <span class="required">obligatoire</span> <xmp class="type"><string></xmp> Nom du composant
- ++Return:++ <xmp class="type"><boolean></xmp> `true` si actif, `undefined` s'il n'existe pas

---
### toogle
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
toggle(Name: string, forceValue?: boolean): void;
```
Activer/Désactiver le composant
- `Name` <span class="required">obligatoire</span> <xmp class="type"><string></xmp> Nom du composant
- `forceValue` <span class="optional">optionel</span> <xmp class="type"><boolean></xmp> Définissez à `true` / `false` si vous voulez forcer l'activation / désactivation du plugin

---
### lang
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
lang(Name: string): JSON;
```
Obtenir les données de la langue du composant sélectionné par l'utilisateur
- `Name` <span class="required">obligatoire</span> <xmp class="type"><string></xmp> Nom du composant
- ++Return:++ <xmp class="type"><JSON></xmp> données de la langue formatées

---
### default
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
default(): JSON;
```
Obtenir les données de la langue du composant par défaut
- ++Return:++ <xmp class="type"><JSON></xmp> données de la langue formatées

---
### listComponents
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
listComponents(): any;
```
Obtenir la liste complète des composants
- ++Return:++ <span class="type">\<any[]\></span> liste complète des composants
