# MCfs

> Easily modify a file, this class is very useful to modify the `.mcfunction` files of the datapack for example.

- [Class: MCfs](#class%3A-mcfs)
  - [addLine](#addline)
  - [modifyLine](#modifyline)
  - [deleteLine](#deleteline)
---

## Class: MCfs

### addLine
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCfs.addLine(File: string, Line: string): void;
```
Add a line at the end of the file

- `File` <span class="required">required</span> <span class="type">\<string\></span> File path
- `Line` <span class="required">required</span> <span class="type">\<string\></span> Line appended to file

---
### modifyLine
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCfs.modifyLine(File: string, Occurence: string, NewLine?: string, AddIfNotExit?: boolean): Promise<void>;
```
Modify the line at the first occurrence find, add new line at the end of file if not exist if `AddIsNotExit` set to `true`

- `File` <span class="required">required</span> <span class="type">\<string\></span> File path
- `Occurence` <span class="required">required</span> <span class="type">\<string\></span> Search string
- `NewLine` <span class="optional">optional</span> <span class="type">\<string\></span> Line to record, delete line if not defined
- `AddIfNotExit` <span class="optional">optional</span> <span class="type">\<boolean\></span> Set to `true` if the line must be added at the end of the file if it does not exist
- ++Return++: <span class="type">\<promise\></span>

---
### deleteLine
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCfs.deleteLine(File: string, Occurence: string): void;
```
Delete the line on which the first occurrence is found

- `File` <span class="required">required</span> <span class="type">\<string\></span> File path
- `Occurence` <span class="required">required</span> <span class="type">\<string\></span> Search string
