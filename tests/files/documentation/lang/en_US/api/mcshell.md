# MCshell

> Add your own commands to the shell system

- [Class: MCshell](#class%3A-mcshell)
  - [Format of a command](#format-of-a-command)
  - [add](#add)
  - [parse](#parse)
  - [Example](#example)
---

## Class: MCshell

This class will allow you to add your own commands to the shell system, and thus make them available in the game.

---
### Format of a command

The arguments passed to your function are in array form, corresponding respectively to :
```js
args[0]: String 	<= "Command name"
args[1]: String 	<= "Username of the player who executed the command"
args[2...]: any[] 	<= "he other arguments in the order they were sent"
```

The format of a command is standardized under the `JSON` format, with the :
```js
exports.command = {
	name: 'program_name',
	function: (args) =>
	{
		const num = Number(42);
		return ({
			Command: args[0],
			Player: args[1],
			Number: num,
		});
	},
};
```
You can absolutely do everything that is necessary inside the function, or you can send the raw data directly.
It is mandatory to have the following keys for your command to work :
- `Command`: must contain the name of your command (`args[0]`)
- `Player`: must contain the user's nickname (`args[1]`)
Without these keys, your command will not work properly.

---
### add

```ts
MCshell.add(json: any): void;
```
Add new command to shell. There are two possible formats :

- Directly via the above format  :
```js
MCshell.add({
	name: 'my_program',
	function: (args) =>
	{
		return({
			Command: args[0],
			Player: args[1],
			Args: args[2]
		});
	},
});
```

- Or automatically by creating a `shell.js` file in the root folder of your plugin, like the following (this method is recommended for better readability):
```js
exports.command = {
	name: 'my_program',
	function: (args) =>
	{
		return({
			Command: args[0],
			Player: args[1],
			Args: args[2]
		});
	},
};
```
Additionally, this method has the huge advantage of being able to create several commands in the same file:
```js
exports.command = [
	{
		name: 'one',
		...
	},
	{
		name: 'two',
		...
	},
	{
		name: 'three',
		...
	},
]

```

---
### parse

```ts
MCshell.parse(line: string): JSON;
```
Parse a line and return the data if the program exists

- `line` <span class="required">obligatoire</span> <xmp class="type"><string></xmp> Line to be analyzed
- ++Return:++ <xmp class="type"><JSON></xmp> Preformed data, or `undefined` if error

---
### Example

Let's take an example, let's say you want to add an input to a form according to what the user requests. We decide that :
- `/mapcraft my_plugin Vex345 add_form number` will add a number input field
- `/mapcraft my_plugin Vex345 add_form string` will add a string input field

The skeleton of the command is as follows:
```js
const myCommand = {
	name: 'my_plugin',
	function: (args) =>
	{
		return ({
			Command: args[0],
			Player: args[1],
			Option: args[2],
			Format: args[3],
		});
	},
}
```

So we can do the following code:
```js
const { MCshell, MCipc } = require('mapcraft_api');

MCshell.add(myCommand);

MCipc.receive('Shell:execute-command', (command) =>
{
	if (command.Command !== 'my_plugin')
		return;
	if (command.Option === 'add_form')
	{
		switch (command.Format)
		{
			case 'number':
				console.log('Add number input');
			case 'string':
			default:
				console.log('Add string input');
		}
	}
});
```