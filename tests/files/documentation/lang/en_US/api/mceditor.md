# MCeditor

> Retrieve information allowing the edition of a file by class instance

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
Open file and return formated data

- `link` <span class="required">required</span> <span class="type">\<string\></span> Path of file
- ++Retourne++: <span class="type">\<JSON\></span> `fileName`, `extension` and `data` in JSON format

---
### saveFile
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCeditor.saveFile(data: string): void;
```
Overwrite the previously opened file and save the file with new data

- `data` <span class="required">required</span> <span class="type">\<string\></span> Data to be saved on the file

---
### closeFile
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCeditor.closeFile(): void;
```
Close file from edition
