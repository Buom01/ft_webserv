# Mapcraft

> Accéder à toutes les informations du logiciel

- [Class: MC](#class%3A-mc)
  - [resetConfigFile](#resetconfigfile)
  - [updateConfig](#updateconfig)
  - [setSelectedVersion](#setselectedversion)
  - [Getter](#getter)
    - [config](#config)
    - [lang](#lang)
    - [manifest](#manifest)
---

## Class: MC

### resetConfigFile
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MC.resetConfigFile(void): void
```
Réinitialiser le fichier de configuration avec les valeurs par défaut.

---
### updateConfig
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MC.updateConfig(data: string, save: string, temp?: string, lang?: string, resourcepack?: string, datapack?: string, apiVersion?: string): void;
```
Met à jour le fichier de configuration avec les nouvelles valeurs.

- `data` <span class="required">obligatoire</span> <span class="type">\<string\></span> Chemin du dossier du jeu (requis)
- `save` <span class="required">obligatoire</span> <span class="type">\<string\></span> Chemin du dossier de sauvegarde du jeu (requis)
- `temp` <span class="required">obligatoire</span> <span class="type">\<string\></span> Chemin du dossier temporaire, par défaut os.tempdir()
- `lang` <span class="optional">optionnel</span> <span class="type">\<string\></span> Langue de l'application, par défaut `default_lang` du manifest
- `resourcepack` <span class="optional">optionnel</span> <span class="type">\<string\></span> Préfixe du resource pack, par défaut `Mapcraft-resource`
- `datapack` <span class="optional">optionnel</span> <span class="type">\<string\></span> Préfixe du data pack, par défaut `Mapcraft-data`
- `apiVersion` <span class="optional">optionnel</span> <span class="type">\<string\></span> Version de l'API

---
### setSelectedVersion
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
setSelectedVersion(version?: string): void;
```
Définir la version de Minecraft sélectionnée par l'utilisateur

- `version` <span class="optional">optionnel</span> <span class="type">\<string\></span> Version sélectionnée, par défaut la plus récente

---
## Getter

### config
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
get config(void): JSON;
```
Obtenir des données de configuration
- ++Retourne++: <span class="type">\<JSON\></span>
---
### lang
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
get lang(void): JSON;
```
Obtenir la langue actuelle de l'application
- ++Retourne++: <span class="type">\<string\></span>
---
### manifest
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
get manifest(void): JSON;
```
Obtenir les données du manifest
- ++Retourne++: <span class="type">\<JSON\></span>
