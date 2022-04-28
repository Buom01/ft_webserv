# Mapcraft

> Access all the information of the software

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
Reset the configuration file to the default values

---
### updateConfig
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MC.updateConfig(data: string, save: string, temp?: string, lang?: string, resourcepack?: string, datapack?: string, apiVersion?: string): void;
```
Update the configuration file file with new values

- `data` <span class="required">required</span> <span class="type">\<string\></span> Path to the game folder (required)
- `save` <span class="required">required</span> <span class="type">\<string\></span> Path to the game saves folder (required)
- `temp` <span class="required">required</span> <span class="type">\<string\></span> Path to the temporary folder, default to OS.tempdir()
- `lang` <span class="optional">optional</span> <span class="type">\<string\></span> Lang of application, default `default_lang` key of manifest
- `resourcepack` <span class="optional">optional</span> <span class="type">\<string\></span> Name of resource pack, default `Mapcraft-resource`
- `datapack` <span class="optional">optional</span> <span class="type">\<string\></span> Name of data pack, default `Mapcraft-data`
- `apiVersion` <span class="optional">optional</span> <span class="type">\<string\></span> Version of API

---
### setSelectedVersion
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
setSelectedVersion(version?: string): void;
```
Set selected minecraft version by user

- `version` <span class="optional">optional</span> <span class="type">\<string\></span> Selected version, latest by default

---
## Getter

### config
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
get config(void): JSON;
```
Get configuration data
- ++Return++: <span class="type">\<JSON\></span>
---
### lang
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
get lang(void): JSON;
```
Get the current language of the application
- ++Return++: <span class="type">\<string\></span>
---
### manifest
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
get manifest(void): JSON;
```
Get the data from the manifest
- ++Return++: <span class="type">\<JSON\></span>
