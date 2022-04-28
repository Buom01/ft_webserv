# MCfs

> Modifier facilement un fichier, cette classe est très utile pour modifier les fichiers `.mcfunction` du datapack par exemple.

- [Class: MCfs](#class%3A-mcfs)
  - [addLine](#addline)
  - [modifyLine](#modifyline)
  - [deleteLine](#deleteline)
---

## Class: MCfs

### addLine
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCfs.addLine(File: string, Line: string): void;
```
Ajouter une ligne à la fin du fichier

- `File` <span class="required">obligatoire</span> <span class="type">\<string\></span> Chemin du fichier
- `Line` <span class="required">obligatoire</span> <span class="type">\<string\></span> Ligne ajoutée au fichier

---
### modifyLine
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCfs.modifyLine(File: string, Occurence: string, NewLine?: string, AddIfNotExit?: boolean): Promise<void>;
```
Modifie la ligne à la première occurrence trouvée, ajoute une nouvelle ligne à la fin du fichier si elle n'existe pas si `AddIsNotExit` vaut `true`.

- `File` <span class="required">obligatoire</span> <span class="type">\<string\></span> Chemin du fichier
- `Occurence` <span class="required">obligatoire</span> <span class="type">\<string\></span> Chaîne de recherche
- `NewLine` <span class="optional">optionnel</span> <span class="type">\<string\></span> Ligne à réenregistrer, efface la ligne si non définie
- `AddIfNotExit` <span class="optional">optionnel</span> <span class="type">\<boolean\></span> Mettre à `true` si la ligne doit être ajoutée à la fin du fichier si elle n'existe pas.
- ++Retourne++: <span class="type">\<promise\></span>

---
### deleteLine
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCfs.deleteLine(File: string, Occurence: string): void;
```
Supprimer la ligne sur laquelle se trouve la première occurrence.

- `File` <span class="required">obligatoire</span> <span class="type">\<string\></span> Chemin du fichier
- `Occurence` <span class="required">obligatoire</span> <span class="type">\<string\></span> Chaîne de recherche
