# MCplugin

> Initialize a set of plugins to be able to use them in the application

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

This class will allow you to instantiate your modules and to access them within the same list. In the classical case, you will have no reason to use it, everything being done automatically.

However, if you want to create several modules in a single ecosystem and access them easily, this class will be useful.

### Initialize
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
const { MCplugin } = require('mapcraft-api');
const PluginsList = new MCplugin(directory?: string);
```
Initialize the class

- `directory` <span class="optional">optional</span> <xmp class="type"><string></xmp> Folder where the plugins are located, `path_to_builtin_plugins`

---
### instance
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
instance(Name: string): any;
```
Get instance of component
- `Name` <span class="required">required</span> <xmp class="type"><string></xmp> Name of component
- ++Return:++ Instance <xmp class="type"><function></xmp> of component, or `undefined` if error

---
### component
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
component(Name: string): any;
```
Get component
- `Name` <span class="required">required</span> <xmp class="type"><string></xmp> Name of component
- ++Return:++ <xmp class="type"><Function></xmp> if exist, or `undefined`

---
### active
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
active(Name: string): any;
```
Check if component is active
- `Name` <span class="required">required</span> <xmp class="type"><string></xmp> Name of component
- ++Return:++ <xmp class="type"><boolean></xmp> `true` if active, `undefined` if not exist

---
### toogle
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
toggle(Name: string, forceValue?: boolean): void;
```
Activate / Deactivate component
- `Name` <span class="required">required</span> <xmp class="type"><string></xmp> Name of component
- `forceValue` <span class="optional">optional</span> <xmp class="type"><boolean></xmp> Set to `true` / `false` if you want to force the activation / deactivation of the plugin

---
### lang
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
lang(Name: string): JSON;
```
Get the language data of the component selected by the user
- `Name` <span class="required">required</span> <xmp class="type"><string></xmp> Name of component
- ++Return:++ <xmp class="type"><JSON></xmp> formatted language data

---
### default
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
default(): JSON;
```
Get the language data of the default component
- ++Return:++ <xmp class="type"><JSON></xmp> formatted language data

---
### listComponents
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
listComponents(): any;
```
Get the complete list of components
- ++Return:++ <span class="type">\<any[]\></span> complete list of components
