# MCeditor

> Récupérer des informations permettant l'édition d'un fichier par instance de classe

- [Class: MCeditor](#class%3A-mceditor)
  - [openFile](#openfile)
  - [saveFile](#savefile)
  - [closeFile](#closefile)
---

## Class: MCeditor

Cette classe peut-être utilisée seule, mais il est fortement recommandé de l'appeler via `MCipc` si vous souhaitez faire appel à l'éditeur de code intégré.
``` ts
MCipc.send('Editor:open', pathToFile);
```

Néanmoins si vous souhaitez créer votre propre éditeur, cette classe vous mâchera grandement le travail.

### openFile
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCeditor.openFile(link: string): JSON;
```
Ouvrir le fichier et retourner les données formatées

- `link` <span class="required">obligatoire</span> <span class="type">\<string\></span> Chemin du fichier
- ++Retourne++: <span class="type">\<JSON\></span> les clés `fileName`, `extension` et `data`

---
### saveFile
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCeditor.saveFile(data: string): void;
```
Écraser le fichier précédemment ouvert et enregistrer le fichier avec de nouvelles données.

- `data` <span class="required">obligatoire</span> <span class="type">\<string\></span> Données à sauvegarder dans le fichier

---
### closeFile
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCeditor.closeFile(): void;
```
Fermer le fichier en édition
