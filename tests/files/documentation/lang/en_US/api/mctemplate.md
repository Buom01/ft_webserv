# MCtemplate

> Générer, afficher et modifier l'interface

- [Class: MCtemplate](#class%3A-mctemplate)
  - [constructor](#constructor)
  - [render](#render)
  - [getRaw](#getraw)
  - [renderRaw](#renderraw)
  - [parseRaw](#parseraw)
  - [updateLang](#updatelang)
  - [cleanNode](#cleannode)
- [Details](#details)
  - [Format](#format)
  - [Variables](#variables)
  - [Language](#language)
  - [A concrete example](#a-concrete-example)
    - [Step 1](#step-1)
	- [Step 2](#step-2)
	- [Step 3](#step-3)
	- [Step 4](#step-4)
	- [Step 5](#step-5)


---

## Class: MCtemplate
Mapcraft uses an approach strongly inspired by several other templating systems.

This system revolves around two axes:
1. Your templates work around `component`, represented by files with the extension `.tp`.
2. You can generate within [element](https://developer.mozilla.org/en-US/docs/Web/API/Element) your `component`, the class will automatically take care of inserting the necessary codes to run.

---
### constructor
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
constructor(directory: string, preGenerateID?: string);
```
Create a new instance

- `directory` <span class="required">required</span> <xmp class="type"><string></xmp> Path to the folder where the `components` are located
- `preGenerateID` <span class="optional">optional</span> <xmp class="type"><string></xmp> Force the use of its own template identifier. Be careful if this function is badly used it can generate many strange problems

---
### render
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
render(DOMelement: Element, template: string, args: JSON): void;
```
Render template in Element

- `DOMelement` <span class="required">required</span> <xmp class="type"><Element></xmp> Element on which the elements will be added
- `template` <span class="required">required</span> <xmp class="type"><string></xmp> Name of template file with extension
- `args` <span class="required">required</span> <xmp class="type"><JSON></xmp> Valid JSON for the replacement variable(s), or `null`

---
### getRaw
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
getRaw(template: string): string;
```
Get HTML raw of template

- `template` <span class="required">required</span> <xmp class="type"><string></xmp> Name of template file with extension
- ++Return:++ <xmp class="type"><string></xmp> Raw of HTML template

---
### renderRaw
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
renderRaw(DOMelement: Element, rawHTML: string, template: string, args: JSON): void;
```
Render generated elements in HTML raw formats; data must be valid HTML

- `DOMelement` <span class="required">required</span> <xmp class="type"><Element></xmp> Element on which the elements will be added
- `rawHTML` <span class="required">required</span> <xmp class="type"><string></xmp> HTML raw
- `template` <span class="required">required</span> <xmp class="type"><string></xmp> Name of template file with extension
- `args` <span class="required">required</span> <xmp class="type"><JSON></xmp> Valid JSON for the replacement variable(s), or `null`

---
### parseRaw
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
parseRaw(HTML: string, args: JSON): string;
```
Parse HTML raw with variable(s)

- `HTML` <span class="required">required</span> <xmp class="type"><string></xmp> HTML raw
- `args` <span class="required">required</span> <xmp class="type"><JSON></xmp> Valid JSON for the replacement variable(s), or `null`
- ++Return:++ <xmp class="type"><string></xmp> Raw HTML with variables replaced

---
### updateLang
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
updateLang(DOMelement: Element, args: JSON): void;
```
Update the language of a specific `component`

- `DOMelement` <span class="required">required</span> <xmp class="type"><Element></xmp> Element where the template to be updated is located
- `args` <span class="required">required</span> <xmp class="type"><JSON></xmp> Valid JSON for the replacement variable(s), or `null`

---
### cleanNode
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
cleanNode(node: Element, RemoveParent?: boolean): void;
```
Correctly clean Node (https://developer.mozilla.org/en-US/docs/Web/API/Node)

- `node` <span class="required">required</span> <xmp class="type"><Element></xmp> Node to be removed
- `RemoveParent` <span class="optional">optional</span> <xmp class="type"><boolean></xmp> If `true`, function remove node after delete child. `false` by default

---
## Details

### Format
The template format has three tags, in which you must write your codes:
- [HTML]
- [CSS]
- [JS]

The tags work exactly like the ones on HTML (opening and closing), here is an example to better understand:

```html
[HTML]
<div class="random">
	<h1>My title</h1>
	<p></p>
</div>
[/HTML]

[CSS]
.random {
	color: aqua;
}
[/CSS]

[JS]
document.querySelector('div.random p').innerText = 'Hello world';
[/JS]
```

Each tag is completely optional, meaning that you can only write HTML and CSS without JS.

The text outside these three tags will not be taken into account when reading them, so you can write comments easily.

---
### Variables
Templates can contain variables, which you will pass as arguments when generating them in JSON format.

The format of the variables is very simple: **{**_var_name_**}**

`mytemplate.tp`:
```html
[HTML]
<div class="{my_var_1}">
	<h1>{my_var_2}</h1>
</div>
[/HTML]
```

This will give:

```js
const args = {
	my_var_1: 'variable-css-class',
	my_var_2: 'My wonderful text'
};

MCtemplate.render(document.getElementById('hello'), 'mytemplate.tp', args);
```

---
### Language
MCtemplate natively supports multilingual modules. By default, the language defined is `en_US`, nevertheless it is possible in a simple way to add one.

In the folder of your plugin inside the `lang` folder, add the files respecting the following supported nomenclatures:
<div style="display:flex; justify-content:space-evenly;">
	<div class="channel">
		<div class="send">en_US</div>
		<div class="receive">English</div>
	</div>
	<div class="channel">
		<div class="send">fr_FR</div>
		<div class="receive">Français</div>
	</div>
</div>

A language file has a structure:
```json
{
	"Title": "My plugin",
	"Icon": "cog",
	"Notification" : "My plugin generate a notification",
	"Data" : {
		"One": "One",
		"Deep": {
			"MoreDeep": {
				"Two": "Two",
			},
			"Three": "Three",
		},
		...
	},
}
```
Let's go into more detail:
- `Title` Title of your plugin that will appear in the dashboard and main menu.
- `Icon`: Icon that will complement your title. The list of icons can be found [here](https://getuikit.com/docs/icon#library).
- `Notification`: Text that will appear when your plugin is notified.
- `Data`: Contains all the texts you want to translate into several languages.

In your templates, you just have to add the `lang` tag and its path as argument, to your html tags to be able to use it.<br/>
Let's take the example above, and apply it to a typical template:
```html
<h1 lang="One"></h1>
<h2 lang="Deep.MoreDeep.Two"></h2>
<h3 lang="Deep.Three"></h3>
```

---
### A concrete example
To better understand how the class works, we will create several components and use them concretely. In our example, we will create a To-Do-List.

#### Step 1
To begin, we will need to create our `components`.

`main.tp`
```html
[HTML]
<div class="main">
	<div class="header">
		<img src="./img/logo.png" />
		<h4 lang="MainTitle"></h4>
	</div>
	<div id="form"></div>
	<button id="add-to-do" lang="AddButton"></button>
</div>
[/HTML]
[CSS]
.main {
	display: flex;
	flex-direction: column;
}
[/CSS]
```

`input.tp`
```html
[HTML]
<input type="text" value="{ID}"/>
<span class="close">X</span>
[/HTML]
[CSS]
.input {
	display: flex;
	flex-direction: row;
	flex-wrap: nowrap;
}
.close {
	color: red;
}
[/CSS]
```

#### Step 2
Let's create a module available in two languages, `en_US` and `fr_FR`. In the `lang` folder, we will create two files:

`en_US.json`
```json
{
	"Title": "To-do list",
	"Icon": "list",
	"Notification" : "",
	"Data" : {
		"MainTitle": "My to-do list",
	},
}
```

`fr_FR.json`
```json
{
	"Title": "Liste des tâches",
	"Icon": "list",
	"Notification" : "",
	"Data" : {
		"MainTitle": "Ma liste des tâches",
	},
}
```

#### Step 3
Initialize the class. To do this, simply build a new instance of the class by indicating the folder in which your `components` are placed.

1. If your `components` are placed in the same folder as your main file, there is a handy global variable that indicates the current folder (`__dirname`). So we'll use it:
```js
const { MCtemplate } = require('mapcraft-api');

const TEMPLATE = new MCtemplate(__dirname);
```

2. On the other hand, if they are placed in another folder, it is strongly recommended to use [path](https://nodejs.org/api/path.html#pathjoinpaths):
```js
const path = require('path');
const { MCtemplate } = require('mapcraft-api');

const TEMPLATE = new MCtemplate(path.join(__dirname, 'template'));
```

#### Step 4
We will display the main `components` in the **content** div.

At the same time we will update the `lang` tags present with the data we retrieved via the `getLang` function of MCutilities.

```js
let LANG = MCutilities.getLang(__dirname, Mapcraft.config.Env.Lang);
TEMPLATE.render(document.getElementById('content'), 'main.tp', null);
TEMPLATE.updateLang(document.getElementById('content'), LANG.Data);
```

Next, we'll add an Event Listener to the button.
When clicking on it, we will add a new line to our list.

We will take advantage of this to make sure that if the user clicks on the cross, the line will be deleted.

```js
let lineID = Number(0);
const List = document.getElementById('form');
document.getElementById('add-to-do').addEventListener('click', (e) =>
{
	e.preventDefault();
	const NewLine = document.createElement('div');
	NewLine.classList.add('input');
	TEMPLATE.render(NewLine, 'input.tp', { ID: lineID++ });
	List.appendChild(NewLine);
	Newline.querySelector('span').addEventListener('click', () => TEMPLATE.cleanNode(NewLine, true));
});
```

#### Step 5
Your module is ready to use. Here is the complete code of your hand:

```js
const { MCtemplate } = require('mapcraft-api');
const TEMPLATE = new MCtemplate(__dirname);
let LANG = MCutilities.getLang(__dirname, Mapcraft.config.Env.Lang);

TEMPLATE.render(document.getElementById('content'), 'main.tp', null);
TEMPLATE.updateLang(document.getElementById('content'), LANG.Data);

let lineID = Number(0);
const List = document.getElementById('form');
document.getElementById('add-to-do').addEventListener('click', (e) =>
{
	e.preventDefault();
	const NewLine = document.createElement('div');
	NewLine.classList.add('input');
	TEMPLATE.render(NewLine, 'input.tp', { ID: lineID++ });
	List.appendChild(NewLine);
	Newline.querySelector('span').addEventListener('click', () => TEMPLATE.cleanNode(NewLine, true));
});
```
