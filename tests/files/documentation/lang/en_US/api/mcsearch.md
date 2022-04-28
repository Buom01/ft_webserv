# MCsearch

> Implement a search bar on game elements

- [Class: MCsearch](#class%3A-mcsearch)
  - [biomes](#biomes)
  - [blocks](#blocks)
  - [blocksItems](#blocksitems)
  - [effects](#effects)
  - [enchantements](#enchantements)
  - [entities](#entities)
  - [items](#items)
  - [potions](#potions)
  - [structures](#structures)
  - [triggers](#triggers)
  - [getValue](#getvalue)
  - [setValue](#setvalue)
---

## Class: MCsearch

This class will allow you to add within your interface a search bar on game elements. This one will automatically adapt to the game version the user will have chosen, or if not, it will be generated with the latest version of Minecraft.

### biomes
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCsearch.biomes(DOM: Element, MinecraftVersion?: string): string;
```
Implements a biome search system via a drop-down menu
- `DOM` <span class="required">required</span> <xmp class="type"><Element></xmp> The Element object in which the search will be inserted
- `MinecraftVersion` <span class="optional">optional</span> <xmp class="type"><string></xmp> The version of minecraft desired, by default at the highest version supported by Mapcraft
- ++Return:++ <xmp class="type"><string></xmp> Identifier of the inserted element. Be careful, this identifier cannot be retrieved later

---

### blocks
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCsearch.blocks(DOM: Element, MinecraftVersion?: string): string;
```
Implements a block search system via a drop-down menu
- `DOM` <span class="required">required</span> <xmp class="type"><Element></xmp> The Element object in which the search will be inserted
- `MinecraftVersion` <span class="optional">optional</span> <xmp class="type"><string></xmp> The version of minecraft desired, by default at the highest version supported by Mapcraft
- ++Return:++ <xmp class="type"><string></xmp> Identifier of the inserted element. Be careful, this identifier cannot be retrieved later

---

### blocksItems
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCsearch.blocksItems(DOM: Element, MinecraftVersion?: string): string;
```
Implements a block and item search system via a drop-down menu
- `DOM` <span class="required">required</span> <xmp class="type"><Element></xmp> The Element object in which the search will be inserted
- `MinecraftVersion` <span class="optional">optional</span> <xmp class="type"><string></xmp> The version of minecraft desired, by default at the highest version supported by Mapcraft
- ++Return:++ <xmp class="type"><string></xmp> Identifier of the inserted element. Be careful, this identifier cannot be retrieved later

---

### effects
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCsearch.effects(DOM: Element, MinecraftVersion?: string): string;
```
Implements a effect search system via a drop-down menu
- `DOM` <span class="required">required</span> <xmp class="type"><Element></xmp> The Element object in which the search will be inserted
- `MinecraftVersion` <span class="optional">optional</span> <xmp class="type"><string></xmp> The version of minecraft desired, by default at the highest version supported by Mapcraft
- ++Return:++ <xmp class="type"><string></xmp> Identifier of the inserted element. Be careful, this identifier cannot be retrieved later

---

### enchantements
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCsearch.enchantements(DOM: Element, MinecraftVersion?: string): string;
```
Implements a enchantement search system via a drop-down menu
- `DOM` <span class="required">required</span> <xmp class="type"><Element></xmp> The Element object in which the search will be inserted
- `MinecraftVersion` <span class="optional">optional</span> <xmp class="type"><string></xmp> The version of minecraft desired, by default at the highest version supported by Mapcraft
- ++Return:++ <xmp class="type"><string></xmp> Identifier of the inserted element. Be careful, this identifier cannot be retrieved later

---

### entities
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCsearch.entities(DOM: Element, MinecraftVersion?: string): string;
```
Implements a entitie search system via a drop-down menu
- `DOM` <span class="required">required</span> <xmp class="type"><Element></xmp> The Element object in which the search will be inserted
- `MinecraftVersion` <span class="optional">optional</span> <xmp class="type"><string></xmp> The version of minecraft desired, by default at the highest version supported by Mapcraft
- ++Return:++ <xmp class="type"><string></xmp> Identifier of the inserted element. Be careful, this identifier cannot be retrieved later

---

### items
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCsearch.items(DOM: Element, MinecraftVersion?: string): string;
```
Implements a item search system via a drop-down menu
- `DOM` <span class="required">required</span> <xmp class="type"><Element></xmp> The Element object in which the search will be inserted
- `MinecraftVersion` <span class="optional">optional</span> <xmp class="type"><string></xmp> The version of minecraft desired, by default at the highest version supported by Mapcraft
- ++Return:++ <xmp class="type"><string></xmp> Identifier of the inserted element. Be careful, this identifier cannot be retrieved later

---

### potions
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCsearch.potions(DOM: Element, MinecraftVersion?: string): string;
```
Implements a potion search system via a drop-down menu
- `DOM` <span class="required">required</span> <xmp class="type"><Element></xmp> The Element object in which the search will be inserted
- `MinecraftVersion` <span class="optional">optional</span> <xmp class="type"><string></xmp> The version of minecraft desired, by default at the highest version supported by Mapcraft
- ++Return:++ <xmp class="type"><string></xmp> Identifier of the inserted element. Be careful, this identifier cannot be retrieved later

---

### structures
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCsearch.structures(DOM: Element, MinecraftVersion?: string): string;
```
Implements a structure search system via a drop-down menu
- `DOM` <span class="required">required</span> <xmp class="type"><Element></xmp> The Element object in which the search will be inserted
- `MinecraftVersion` <span class="optional">optional</span> <xmp class="type"><string></xmp> The version of minecraft desired, by default at the highest version supported by Mapcraft
- ++Return:++ <xmp class="type"><string></xmp> Identifier of the inserted element. Be careful, this identifier cannot be retrieved later

---

### triggers
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCsearch.triggers(DOM: Element, MinecraftVersion?: string): string;
```
Implements a trigger search system via a drop-down menu
- `DOM` <span class="required">required</span> <xmp class="type"><Element></xmp> The Element object in which the search will be inserted
- `MinecraftVersion` <span class="optional">optional</span> <xmp class="type"><string></xmp> The version of minecraft desired, by default at the highest version supported by Mapcraft
- ++Return:++ <xmp class="type"><string></xmp> Identifier of the inserted element. Be careful, this identifier cannot be retrieved later

---

### getValue
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCsearch.getValue(DOM: Element): any;
```
Get value of search system
- `DOM` <span class="required">required</span> <xmp class="type"><Element></xmp> Element in which the search system is located
- ++Return:++ <xmp class="type"><string></xmp> Value of search

---

### setValue
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCsearch.setValue(DOM: Element, value: string): void;
```
Set value of search system
- `DOM` <span class="required">required</span> <xmp class="type"><Element></xmp> Element in which the search system is located
- `value` <span class="required">required</span> <xmp class="type"><string></xmp> New value of element
