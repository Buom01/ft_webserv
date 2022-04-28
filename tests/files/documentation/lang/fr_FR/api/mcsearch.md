# MCsearch

> Implémenter une barre de recherche sur des éléments du jeu

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

Cette classe va vous permettre d'ajouter au sein de votre interface une barre de recherche sur des éléments du jeu. Celle-ci s'adaptera automatiquement à la version du jeu que l'utilisateur aura choisie, ou à défaut sera généré avec la dernière version de Minecraft.

### biomes
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCsearch.biomes(DOM: Element, MinecraftVersion?: string): string;
```
Implémente un système de recherche de biome via un champ de recherche
- `DOM` <span class="required">obligatoire</span> <xmp class="type"><Element></xmp> L'objet Élément dans lequel le champ de recherche sera insérée
- `MinecraftVersion` <span class="optional">optionel</span> <xmp class="type"><string></xmp> La version de Minecraft souhaitée, par défaut à la plus haute version supportée par Mapcraft
- ++Retourne:++ <xmp class="type"><string></xmp> Identifiant de l'élément inséré. Attention, cet identifiant ne peut pas être récupéré ultérieurement

---

### blocks
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCsearch.blocks(DOM: Element, MinecraftVersion?: string): string;
```
Implémente un système de recherche de blocs via un champ de recherche
- `DOM` <span class="required">obligatoire</span> <xmp class="type"><Element></xmp> L'objet Élément dans lequel le champ de recherche sera insérée
- `MinecraftVersion` <span class="optional">optionel</span> <xmp class="type"><string></xmp> La version de Minecraft souhaitée, par défaut à la plus haute version supportée par Mapcraft
- ++Retourne:++ <xmp class="type"><string></xmp> Identifiant de l'élément inséré. Attention, cet identifiant ne peut pas être récupéré ultérieurement

---

### blocksItems
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCsearch.blocksItems(DOM: Element, MinecraftVersion?: string): string;
```
Implémente un système de recherche de blocs et d'éléments via un champ de recherche
- `DOM` <span class="required">obligatoire</span> <xmp class="type"><Element></xmp> L'objet Élément dans lequel le champ de recherche sera insérée
- `MinecraftVersion` <span class="optional">optionel</span> <xmp class="type"><string></xmp> La version de Minecraft souhaitée, par défaut à la plus haute version supportée par Mapcraft
- ++Retourne:++ <xmp class="type"><string></xmp> Identifiant de l'élément inséré. Attention, cet identifiant ne peut pas être récupéré ultérieurement

---

### effects
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCsearch.effects(DOM: Element, MinecraftVersion?: string): string;
```
Implémente un système de recherche d'effects via un champ de recherche
- `DOM` <span class="required">obligatoire</span> <xmp class="type"><Element></xmp> L'objet Élément dans lequel le champ de recherche sera insérée
- `MinecraftVersion` <span class="optional">optionel</span> <xmp class="type"><string></xmp> La version de Minecraft souhaitée, par défaut à la plus haute version supportée par Mapcraft
- ++Retourne:++ <xmp class="type"><string></xmp> Identifiant de l'élément inséré. Attention, cet identifiant ne peut pas être récupéré ultérieurement

---

### enchantements
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCsearch.enchantements(DOM: Element, MinecraftVersion?: string): string;
```
Implémente un système de recherche d'enchantements via un champ de recherche
- `DOM` <span class="required">obligatoire</span> <xmp class="type"><Element></xmp> L'objet Élément dans lequel le champ de recherche sera insérée
- `MinecraftVersion` <span class="optional">optionel</span> <xmp class="type"><string></xmp> La version de Minecraft souhaitée, par défaut à la plus haute version supportée par Mapcraft
- ++Retourne:++ <xmp class="type"><string></xmp> Identifiant de l'élément inséré. Attention, cet identifiant ne peut pas être récupéré ultérieurement

---

### entities
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCsearch.entities(DOM: Element, MinecraftVersion?: string): string;
```
Implémente un système de recherche d'entités via un champ de recherche
- `DOM` <span class="required">obligatoire</span> <xmp class="type"><Element></xmp> L'objet Élément dans lequel le champ de recherche sera insérée
- `MinecraftVersion` <span class="optional">optionel</span> <xmp class="type"><string></xmp> La version de Minecraft souhaitée, par défaut à la plus haute version supportée par Mapcraft
- ++Retourne:++ <xmp class="type"><string></xmp> Identifiant de l'élément inséré. Attention, cet identifiant ne peut pas être récupéré ultérieurement

---

### items
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCsearch.items(DOM: Element, MinecraftVersion?: string): string;
```
Implémente un système de recherche d'éléments via un champ de recherche
- `DOM` <span class="required">obligatoire</span> <xmp class="type"><Element></xmp> L'objet Élément dans lequel le champ de recherche sera insérée
- `MinecraftVersion` <span class="optional">optionel</span> <xmp class="type"><string></xmp> La version de Minecraft souhaitée, par défaut à la plus haute version supportée par Mapcraft
- ++Retourne:++ <xmp class="type"><string></xmp> Identifiant de l'élément inséré. Attention, cet identifiant ne peut pas être récupéré ultérieurement

---

### potions
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCsearch.potions(DOM: Element, MinecraftVersion?: string): string;
```
Implémente un système de recherche de potions via un champ de recherche
- `DOM` <span class="required">obligatoire</span> <xmp class="type"><Element></xmp> L'objet Élément dans lequel le champ de recherche sera insérée
- `MinecraftVersion` <span class="optional">optionel</span> <xmp class="type"><string></xmp> La version de Minecraft souhaitée, par défaut à la plus haute version supportée par Mapcraft
- ++Retourne:++ <xmp class="type"><string></xmp> Identifiant de l'élément inséré. Attention, cet identifiant ne peut pas être récupéré ultérieurement

---

### structures
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCsearch.structures(DOM: Element, MinecraftVersion?: string): string;
```
Implémente un système de recherche de structures via un champ de recherche
- `DOM` <span class="required">obligatoire</span> <xmp class="type"><Element></xmp> L'objet Élément dans lequel le champ de recherche sera insérée
- `MinecraftVersion` <span class="optional">optionel</span> <xmp class="type"><string></xmp> La version de Minecraft souhaitée, par défaut à la plus haute version supportée par Mapcraft
- ++Retourne:++ <xmp class="type"><string></xmp> Identifiant de l'élément inséré. Attention, cet identifiant ne peut pas être récupéré ultérieurement

---

### triggers
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCsearch.triggers(DOM: Element, MinecraftVersion?: string): string;
```
Implémente un système de recherche de déclencheurs via un champ de recherche
- `DOM` <span class="required">obligatoire</span> <xmp class="type"><Element></xmp> L'objet Élément dans lequel le champ de recherche sera insérée
- `MinecraftVersion` <span class="optional">optionel</span> <xmp class="type"><string></xmp> La version de Minecraft souhaitée, par défaut à la plus haute version supportée par Mapcraft
- ++Retourne:++ <xmp class="type"><string></xmp> Identifiant de l'élément inséré. Attention, cet identifiant ne peut pas être récupéré ultérieurement

---

### getValue
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCsearch.getValue(DOM: Element): any;
```
Obtenir la valeur du champ de recherche
- `DOM` <span class="required">obligatoire</span> <xmp class="type"><Element></xmp> Élément dans lequel se trouve le système de recherche
- ++Retourne:++ <xmp class="type"><string></xmp> Valeur du champ

---

### setValue
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCsearch.setValue(DOM: Element, value: string): void;
```
Définir la valeur du champ de recherche
- `DOM` <span class="required">obligatoire</span> <xmp class="type"><Element></xmp> Élément dans lequel se trouve le système de recherche
- `value` <span class="required">obligatoire</span> <xmp class="type"><string></xmp> New value of element
