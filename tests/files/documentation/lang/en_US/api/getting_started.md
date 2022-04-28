# Getting started

Getting started creating a plugin may seem like a daunting task. But don't worry, by the end of this guide you will have your first fully functional plugin that you can share with all other users.

- [Prerequisites](#prerequisites)
- [Create your plugin](#create-your-plugin)
  - [Structure of a plugin](#structure-of-a-plugin)
- [Start development](#start-development)
- [Explanation](#explanation)
  - [Initialization](#initialization)
  - [Component](#component)
  - [Shell](#shell)
- [Template file](#template-file)
- [Distribute your plugin](#distribute-your-plugin)
---

## Prerequisites

To develop your plugin, you will need to install `Node.js` (16.14.0) and `NPM`:
  - Via a version manager, [Windows](https://github.com/coreybutler/nvm-windows/releases), [MacOS / Linux](https://github.com/nvm-sh/nvm/releases/latest) (recommended)
  - Via the direct installer [Node.js](https://nodejs.org)

To verify that Node.js has been installed correctly, type the following commands in your terminal:

```bash
node -v
npm -v
```

The commands should print the versions of Node.js and NPM respectively

## Create your plugin

The plugins follow the same structure as the other projects using Node.js. However, to make your life much easier, Mapcraft contains a `CLI` (command line interface).
<div style="display: inline-flex; align-items: center;">
	<div style="margin-right: .7em; min-width: 70%;">
		<p>To access it, open your application, and go to the <b>Option</b> module. In the <b>General</b> tab, go to the <b>Development tool</b> part and click on the <code>Create a new plugin button</code></p>
	</div>
	<img style="max-width: 30%" src="./srcs/img/data/getting_started/create_plugin.png"/>
</div>

---

<div style="display: inline-flex; align-items: center;">
	<div style="margin-right: .7em; min-width: 70%;">
		<p>A new window will open. Follow the different steps by filling in the requested information:</p>
		<ol>
			<li>Name of your plugin</li>
			<li>Name displayed in the software</li>
			<li>Version of your plugin, it is recommended to follow <a href="https://docs.npmjs.com/about-semantic-versioning">the logic of NPM</a></li>
			<li>Name of the author of the plugin, yours in all logic</li>
			<li>Description of your plugin, a short description explaining basically what your plugin does</li>
			<li>Choice of license, if you have no idea, take the default license</li>
			<li>Name of the main file, this is the file that should contain the access point to your plugin</li>
			<li>Name of the folder where the language files will be available</li>
			<li>Will your plugin display a notification if it detects an order ?</li>
		</ol>
	</div>
	<img style="max-width: 30%" src="./srcs/img/data/getting_started/cli_create.png"/>
</div>

---
Your new plugin will be in the following folder:
- Windows: `%appdata%/Mapcraft/plugins/[name of your plugin]` 
- Mac: `~/.config/Mapcraft/plugins/[name of your plugin]` ou `$XDG_CONFIG_HOME/Mapcraft/plugins/[name of your plugin]`
- Linux: `~/Library/Application Support/Mapcraft/plugins/[name of your plugin]`

### Structure of a plugin
A basic plugin contains at least the following tree structure:

```text
[plugin_name]
  │
  ├─ 🗀 lang
  │   ╰─ *.json
  ├─ 🗀 node_modules
  │   ╰─ **/*
  ├─ index.js
  ├─ LICENSE
  ├─ [plugin_name].tp
  ├─ package.json
  ├─ package-lock.json
  ╰─ shell.js
```

- `lang`: contains the language files of your application, by default this one must have `en_US.json`.
- `node_modules`: contains the dependencies of your plugin
- `index.js` or the name of your input file: the input file of your plugin
- `LICENSE`: the license of your plugin
- `[plugin_name].tp`: the template file of your application, see [MCtemplate](api/mctemplate)
- `package.json`: contains all the information about your plugin. This one is fully compatible with package managers
- `package-lock.json`: file that allows npm to reproduce your exact dependency installation
- `shell.js`: file allowing you to add your own commands to the Mapcraft shell, see [MCshell](api/mcshell)

Your `package.json` file should look like this:
```json
{
    "name": "hello_world",
    "title": "Hello World",
    "version": "0.0.1",
    "author": "John Doe",
    "description": "My wonderful plugin",
    "license": "gpl-3.0",
    "uuid": "e942b9fd-e6b2-4711-960d-7f3211a4a486",
    "icon": "icon.png",
    "isNotification": true,
    "bin": {
        "component": "index.js",
        "command": "hello_world",
        "lang": "lang"
    },
    "dependencies": {
        "mapcraft-api": "^1.8.1"
    }
}
```
As you can see, the Mapcraft api is already installed and directly available. And like any Node.js project, you can install all the packages you need for its development. Nevertheless the only mandatory dependency is `mapcraft-api`.

However don't forget that `devDependencies` will not be included for the user.

---
## Start development

To begin with, you need to understand how Mapcraft will use your plugin.

It will first read your `package.json` file to extract all the information, and initialize your plugin as a module (via a `require`).
To do this, the entry point must be a ++class++ with a function called `main`, with no arguments; then it must be exported.

```ts
class Component
{
	static main()
	{
		...
	}
}

module.exports = Component;
```

The Mapcraft interface has an [HTMLDivElement](https://developer.mozilla.org/en-US/docs/Web/API/HTMLDivElement) with the id `content`, in which all installed plugins are displayed.
This means to start displaying your plugin, you should at least insert the base of it in this div, for example :
|||
|---|---|
| ✓ | <pre><code class="language-ts">TEMPLATE.render(document.getElementById('content'), 'hello_world.tp', LANG.Data);</code></pre> |
| ✗ | <pre><code class="language-ts">TEMPLATE.render(document.getElementById('random-id'), 'hello_world.tp', LANG.Data);</code></pre> |

After that, you'll be able to access all the Elements you create, which means that if in your template you have a div with the id `hello`, you'll be able to do the following code without any problem :
<pre><code class="language-ts">document.getElementById('hello')</code></pre>

You will also have access to absolutely [all APIs](https://developer.mozilla.org/en-US/docs/Web/API) available in a regular browser, if you know how to code a website, you know how to make a plugin.
However, to avoid any conflicts (let's say you create an element with an identifier that already exists in the software interface), it is strongly recommended to work only on the `div content`, like this:
```ts
const CONTENT = document.getElementById('content');

CONTENT.getElementById('my-div').addEventListener('click', () => console.log('click in content'), false);
CONTENT.querySelectorAll('a.my-class').forEach((el) => ...);
...
```

This way you only modify your own code without any risk of interfering with the software interface

## Explanation

To go further, open the input file of your application (here `index.js`). We will step by step understand the written code and its usefulness.

### Initialization
<div class="gs-flex">
	<div>
		<ol>
			<li>Import the following modules:
				<ul>
					<li><b>Mapcraft</b> Access all the information of the software</li>
					<li><b>MCipc</b> Send and/or receive information from the main process</li>
					<li><b>MCtemplate</b> Generate, display and modify the interface</li>
					<li><b>MCutilities</b> Contains a set of useful functions</li>
				</ul>
			</li>
			<li>Retrieve the entire contents of the <code>package.json</code> file. This will allow access to all the information necessary for its proper functioning
				<p>Explanation of the arguments:</p>
				<ul>
					<li>Location where the file is located, here at the root of the plugin, that is <code>__dirname</code></li>
				</ul>
			</li>
			<li>Initialization of a new instance of MCtemplate.
				Explanation of the arguments:
				<ul>
					<li>Location of all our template files <code>*.tp</code>, here at the root of the plugin, that is <code>__dirname</code></li>
					<li>UUID of the plugin to allow our instance to manage only these elements (this allows several instances to coexist without problem)</li>
				</ul>
			</li>
			<li>Creation of a variable that will contain the data of the language files</li>
			<li>Create a function that will simply update the data in the <code>LANG</code> variable according to the language chosen by the user
				<p>The called function returns the pre-formatted data in the correct language format</p>
				Explanation of the arguments:
				<ul>
					<li>Location of the plugin, here <code>__dirname</code></li>
					<li>User-selected language, retrievable via <code>Mapcraft.config.Env.Lang</code></li>
					<li>Folder in which this is the language files, this one being defined in the <b>package.json</b>, we can access it via <code>PACKAGE.bin.lang</code></li>
				</ul>
			</li>
		</ol>
	</div>
	<pre><code class="language-ts">
a. const { Mapcraft, MCipc, MCtemplate, MCutilities } = require('mapcraft-api');
</br>
b. const PACKAGE = MCutilities.getPackage(__dirname);
c. const TEMPLATE = new MCtemplate(__dirname, PACKAGE.uuid);
d. let LANG;
e. const updateLANG = () =>
{
	LANG = MCutilities.getLang(__dirname, Mapcraft.config.Env.Lang, PACKAGE.bin.lang);
};
	</code></pre>
</div>

---
### Component
<div class="gs-flex">
	<div>
		<p>The main component of your plugin is the one that will be exported and used by Mapcraft</p>
		<p>This one must respect several points to work:</p>
		<ol style="list-style-type: number">
			<li>It must be a class</li>
			<li>The input function is and will always be named <code>main</code>. This one must have no arguments</li>
			<li>The class can contain a constructor, but nevertheless Mapcraft never initializes a class with <code>new</code>
				<p>If you decide to implement a constructor, export your class like this:</p>
				<pre><code class="language-ts">module.exports = new Component();</code></pre>
			</li>
		</ol>
		<hr>
		<p>Let's detail the code of the component we are interested in.</p>
		<ol>
			<li>We call the function previously created to obtain the data corresponding to the language selected by the user</li>
			<li>Our MCtemplate instance will be used to render your template in the selected element.
				<p>The display of a plugin is <b>mandatorily</b> rendered in the page element identified by the ID <code>content</code></p>
				<img style="width: 50%;" src="./srcs/img/data/getting_started/content.png"/>
				<p>Explanation of the arguments:</p>
				<ul>
					<li>Element of the page in which the template will be rendered, knowing that this is the main template, we will render it in the <b>div.#content</b></li>
					<li>Selected template file, with the extension</li>
					<li>Valid JSON data to replace the variables of the template with the one indicated. Here we use directly the variables contained in LANG</li>
				</ul>
			</li>
		</ol>
	</div>
	<pre><code class="language-ts">
class Component
{
	static main()
	{
a.		updateLANG();
b.		TEMPLATE.render(document.getElementById('content'), 'hello_world.tp', LANG.Data);
	}
}
	</code></pre>
</div>

---
### Shell
<div class="gs-flex">
	<div>
		<p>The Shell part is not mandatory. If you want to know how the creation of a new command works, go to the page dedicated to the MCshell module</p>
		<p>To be able to detect if a user to enter in the game chat a command related to your plugin, you will have to "listen" permanently to the channel <code>Shell:execute-command</code>. This channel sends a data from the moment any command has been detected, it's up to you to know if it's yours</p>
		<ol>
			<li>To avoid redoing an "if" each time, we get the name of the command defined in the <code>package.json</code> of your plugin. If this field is not defined, by default the name of your plugin will be taken</li>
			<li>We listen on the channel if a command is received via the receive function (<a href="https://nodejs.org/api/fs.html#callback-example">Callback</a>)</li>
			<li>We check that the command is the one we are interested in by checking its name via <code>[name_of_args].Command</code>, if yes we continue</li>
			<li>We create a success alert, by displaying the return of the function as a String</li>
		</ol>
	</div>
	<pre><code class="language-ts">
a.	const COMMAND = (MANIFEST.bin.command) ? MANIFEST.bin.command : MANIFEST.name;
b.	MCipc.receive('Shell:execute-command', (command) =>
	{
c.		if (command.Command !== COMMAND)
			return;
d.		MCutilities.createAlert('success', document.getElementById('custom-alert'), `Receive command ${JSON.stringify(command)}`);
	});
	</code></pre>
</div>

---
## Template file

The format of the templates is fully described in the corresponding page of the [MCtemplate module documentation, in the Details section](api/mctemplate#details)

---
## Distribute your plugin

That's it, you have finished your plugin and are ready to make it available to the world. To do so, you will have to compress it into a specific format, the `.mapcraft`.

<div style="display: inline-flex; align-items: center;">
	<div style="margin-right: .7em">
		<p>To do this, open your application, and go to the <b>Option</b> module. In the <b>General</b> tab, go to the <b>Development tool</b> part and click on the <code>Plugin packaging</code> button</p>
		<p>After selecting your plugin from the list, it will be converted. Depending on the size of the plugin, this may take a few seconds or a few minutes</p>
	</div>
	<img style="max-width: 30%" src="./srcs/img/data/getting_started/package_plugin.png"/>
</div>

The archive will be available in the folder of your plugin:
- Windows: `%appdata%/Mapcraft/plugins/[name of your plugin]/[name of your plugin].mapcraft` 
- Mac: `~/.config/Mapcraft/plugins/[name of your plugin]/[name of your plugin].mapcraft` ou `$XDG_CONFIG_HOME/Mapcraft/plugins/[name of your plugin]/[name of your plugin].mapcraft`
- Linux: `~/Library/Application Support/Mapcraft/plugins/[name of your plugin]/[name of your plugin].mapcraft`

Simply provide this archive to users so that they can easily install it.

---
### Congratulations, you are finally ready to make your first weapons, your only limit is your imagination !
