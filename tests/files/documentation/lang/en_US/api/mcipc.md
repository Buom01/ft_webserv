# MCipc

> Send and/or receive information from the main process

- [Class: MCipc](#class%3A-mcipc)
  - [List of channels](#list-of-channels)
  - [Send](#send)
  - [Receive](#receive)
---

## Class: MCipc

Mapcraft works around two processes, the parent and the renderer. The two processes work in isolation and do not communicate with each other.

This way of doing things has several advantages, especially in terms of the security of the application, but of course has the disadvantage of making their communication complex.

The plugins work in the rendering process, it is sometimes necessary to perform actions that only the parent is authorized to do. So you will have to use this class to send data on a specific channel to the parent asynchronously, and the parent will return data to you on a response channel if necessary.

### List of channels

Usage format of the functions :
```js
MCipc.send("[Type]:[Sending channel]", ...args);
MCipc.receive("[Type]:[Receiving channel]", (...args) => {});
```

++Scheme :++
<div class="channel">
	<div class="send"><span>📤</span><b>Sending</b></div>
	<div class="receive"><span>📥</span><b>Receiving</b></div>
</div>
<table class="channel-tab">
	<thead>
		<tr>
			<th>Type</th>
			<th>Channels</th>
		</tr>
	</thead>
	<tbody>
		<tr>
			<td>Dialog</td>
			<td>
				<div class="channel">
					<div class="send"><span>📤</span>open-directory</div>
					<div class="receive"><span>📥</span>selected-directory</div>
					<div class="text">
						<p>Open a dialog box to select a folder</p>
						<ul>
							<li><code>element</code> <xmp class="type"><string></xmp> Event ID</li>
							<li><code>path</code> <xmp class="type"><string></xmp> Default directory path</li>
							<li><ins>Retourne:</ins> <xmp class="type"><string></xmp> Path of the selected directory</li>
							<li><ins>Error:</ins> Prints the error in <code>console.error()</code> and returns nothing</li>
						</ul>
					</div>
				</div>
				<div class="channel">
					<div class="send"><span>📤</span>open-file</div>
					<div class="receive"><span>📥</span>selected-file</div>
					<div class="text">
						<p>Open a dialog box to select a file</p>
						<ul>
							<li><code>element</code> <xmp class="type"><string></xmp> Event ID</li>
							<li><code>path</code> <xmp class="type"><string></xmp> Default directory path</li>
							<li><ins>Retourne:</ins> <xmp class="type"><string></xmp> Path of the selected file</li>
							<li><ins>Error:</ins> Prints the error in <code>console.error()</code> and returns nothing</li>
						</ul>
					</div>
				</div>
				<div class="channel">
					<div class="send"><span>📤</span>open-global</div>
					<div class="receive"><span>📥</span>selected-global</div>
					<div class="text">
						<p>Open a dialog box allowing to select an object according to the configuration passed in parameter</p>
						<ul>
							<li><code>element</code> <xmp class="type"><string></xmp> Event ID</li>
							<li><code>options</code> <xmp class="type"><JSON></xmp> <a href="https://www.electronjs.org/docs/latest/api/dialog#dialogshowopendialogbrowserwindow-options">Options</a></li>
							<li><ins>Retourne:</ins> <xmp class="type"><Object></xmp>
								<ul>
									<li><code>canceled</code> <xmp class="type"><boolean></xmp> If the dialogue has been cancelled or not</li>
									<li><code>filePaths</code> <xmp class="type"><string[]></xmp> An array of file paths chosen by the user. If the dialog box is cancelled, it will be an empty array</li>
								</ul>
							</li>
							<li><ins>Error:</ins> Prints the error in <code>console.error()</code> and returns nothing</li>
						</ul>
					</div>
				</div>
				<div class="channel">
					<div class="send"><span>📤</span>question</div>
					<div class="text">
						<p>Open a window with a question</p>
						<ul>
							<li><code>options</code> <xmp class="type"><JSON></xmp> <a href="https://www.electronjs.org/docs/latest/api/dialog#dialogshowmessageboxbrowserwindow-options">Options</a></li>
						</ul>
					</div>
				</div>
				<div class="channel">
					<div class="send"><span>📤</span>error</div>
					<div class="text">
						<p>Open a window to display an error</p>
						<ul>
							<li><code>title</code> <xmp class="type"><string></xmp> The title to be displayed in the error box</li>
							<li><code>content</code> <xmp class="type"><string></xmp> The content to be displayed in the error box</li>
						</ul>
					</div>
				</div>
			</td>
		</tr>
		<tr>
			<td>Editor</td>
			<td>
				<div class="channel">
					<div class="send"><span>📤</span>open</div>
					<div class="text">
						<p>Opens the text editor</p>
						<ul>
							<li><code>path</code> <xmp class="type"><string></xmp> File path</li>
						</ul>
					</div>
				</div>
				<div class="channel">
					<div class="send"><span>📤</span>close</div>
					<div class="text">
						<p>Closes the text editor without saving changes</p>
					</div>
				</div>
				<div class="channel">
					<div class="send"><span>📤</span>save-file</div>
					<div class="text">
						<p>Saves the changes made to the file, and closes the editor</p>
					</div>
				</div>
			</td>
		</tr>
		<tr>
			<td>Shell</td>
			<td>
				<div class="channel">
					<div class="receive"><span>📥</span>execute-command</div>
					<div class="text">
						<p>A new command has just been executed in the game</p>
						<ul>
							<li><ins>Return:</ins> <xmp class="type"><JSON></xmp> JSON formatted command (see <a href="http://localhost/documentation/?en_US&api/mcshell">MCshell</a> for more information)</li>
						</ul>
					</div>
				</div>
			</td>
		</tr>
	</tbody>
</table>

### Send
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCipc.send(channel: string, ...args: any[]): void;
```
Send data to a specific channel

- `channel` <span class="required">required</span> <xmp class="type"><string></xmp> Channel name
- `args` <span class="required">required</span> <span class="type"><any[]></span> List of argument(s)

---
### Receive
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCipc.receive(channel: string, func: Function): void;
```
Receive the response from a specific channel and pass the return arguments to a callback function.

- `channel` <span class="required">required</span> <xmp class="type"><string></xmp> Channel name
- `funct` <span class="required">required</span> <xmp class="type"><Function></xmp> Callback function
