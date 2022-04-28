# MCutilities

> Contains a set of functions that are very useful in your life as a developer

- [Class: MCutilities](#class%3A-mcutilities)
  - [checkIsLegalString](#checkislegalstring)
  - [nextLetter](#getnextcharacterinalphabet)
  - [generateENV](#generateenv)
  - [download](#download)
  - [isEmptyDir](#isemptydir)
  - [getLang](#getlang)
  - [getPackage](#getmanifest)
  - [getDataGameElement](#getdatagameelement)
  - [createAlert](#createalert)
---

## Class: MCutilities

### checkIsLegalString
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCutilities.checkIsLegalString(string: string): boolean;
```
Returns a Boolean value that indicates if string is valid in Minecraft format

- `string` <span class="required">required</span> <xmp class="type"><string></xmp> String to check
- ++Return++: <xmp class="type"><boolean></xmp> `false` if the string contains an unauthorized character

---
### nextLetter
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCutilities.nextLetter(char: string): string;
```
Get next character in alphabet

- `char` <span class="required">required</span> <xmp class="type"><string></xmp> Current character
- ++Return++: <xmp class="type"><boolean></xmp> Next character

```ts
for (let char = 'a'; char != 'z'; char = MCutilities.nextLetter(char))
	console.log(`${char}-`);
/* a-b-c-d-e-f-g-[...] */
```

---
### generateENV
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCutilities.generateENV(void): void;
```
Generate ENV of system for application

---
### download
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCutilities.download(url: string, destination: string, callback: Function): void;
```
Download file from web, accept http and https url

This function does not check the content of the downloaded file, it is up to you to perform the necessary checks so as not to compromise the user's security

- `url` <span class="required">required</span> <xmp class="type"><string></xmp> Url of download file
- `destination` <span class="required">required</span> <xmp class="type"><string></xmp> Path of file destination
- `callback` <span class="required">required</span> <xmp class="type"><Function></xmp> Callback function with (error)

```js
MCutilities.download('https://hello.com/my_file.png', 'my_new_file', (err) => {
	if (err)
		throw new Error(err.code, err.message);
	// rest of callback
});
```

---
### isEmptyDir
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCutilities.isEmptyDir(path: any): boolean;
```
Check if directory is empty

- `path` <span class="required">required</span> <xmp class="type"><string></xmp> Path to directory
- ++Return++: <xmp class="type"><boolean></xmp> `true` if is empty

---
### getLang
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCutilities.getLang(dirname: string, langPath: string, defaultDir?: string): JSON;
```
Get lang of component

- `dirname` <span class="required">required</span> <xmp class="type"><string></xmp> Component directory path (`__dirname` recommended)
- `langPath` <span class="required">required</span> <xmp class="type"><string></xmp> Name of the selected language, or `MC.config.Env.Lang` for a predefined value
- `defaultDir` <span class="optional">optional</span> <xmp class="type"><string></xmp> Default directory of the `lang` folder
- ++Return++: <xmp class="type"><JSON></xmp>  JSON data of lang file, or `undefined` if error

---
### getPackage
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCutilities.getPackage(dirname: string): JSON;
```
Retrieved data from `package.json`

- `dirname` <span class="required">required</span> <xmp class="type"><string></xmp> Folder in which you want to search
- ++Return++: <xmp class="type"><JSON></xmp> JSON data of `package.json`, or `undefined` if error

---
### getDataGameElement
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCutilities.getDataGameElement(type: string, minecraftVersion?: any): JSON;
```
Retrieved data on game elements

- `type` <span class="required">required</span> <xmp class="type"><string></xmp> Type of data to be retrieved (`biomes`, `blocks`, `effects`, `enchantements`, `entities`, `items`, `potions`, `structures`, `tags`, `triggers`)
- `minecraftVersion` <span class="required">required</span> <xmp class="type"><string></xmp> The version of minecraft desired, by default at the highest version supported by Mapcraft
- ++Return++: <xmp class="type"><JSON></xmp> JSON data, or `undefined` if error

---
### createAlert
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCutilities.createAlert(type: string, DOMelement: Element, str: string): void;
```
Print alert in HTMLelement

- `type` <span class="required">required</span> <xmp class="type"><string></xmp> Type of error (`primary`, `success`, `warning`, `danger`)
- `DOMelement` <span class="required">required</span> <xmp class="type"><Element></xmp> Element in which alert will be displayed
- `str` <span class="required">required</span> <xmp class="type"><string></xmp> Error message

```js
MCutilities.createAlert('success', document.getElementById('custom-alert'), 'Command is undefined');
```
