# MCipc

> Envoyer et/ou réceptionner des informations du processus principal

- [Class: MCipc](#class%3A-mcipc)
  - [Liste des canaux](#liste-des-canaux)
  - [Send](#send)
  - [Receive](#receive)
---

## Class: MCipc

Mapcraft fonctionne autour de deux processus, le parent et le rendu. Les deux fonctionnant de façon isolée et ne communiquent pas entre eux.

Cette façon de faire à plusieurs avantages notamment au niveau de la sécurité de l'application, mais à bien sûr comme désavantage de rendre leur communication complexe.

Les plugins fonctionnent dans le processus de rendu, il est parfois nécessaire d'effectuer des actions que seul le parent est autorisé à faire. Vous allez donc devoir utiliser cette class pour envoyer des données sur un canal spécifiques aux parents de façon asynchrone, et le parent vous retournera sur un canal de réponse des données si nécessaires.

### Liste des canaux

Format d'utilisation des fonctions :
```js
MCipc.send("[Type]:[Canal d'envoi]", ...args);
MCipc.receive("[Type]:[Canal de réception]", (...args) => {});
```

++Schéma :++
<div class="channel">
	<div class="send"><span>📤</span><b>Envoi</b></div>
	<div class="receive"><span>📥</span><b>Réception</b></div>
</div>
<table class="channel-tab">
	<thead>
		<tr>
			<th>Type</th>
			<th>Canaux</th>
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
						<p>Ouvrir une boîte de dialogue permettant de sélectionner un dossier</p>
						<ul>
							<li><code>element</code> <xmp class="type"><string></xmp> ID de l'évènement</li>
							<li><code>path</code> <xmp class="type"><string></xmp> Chemin du répertoire par défaut</li>
							<li><ins>Retourne:</ins> <xmp class="type"><string></xmp> Chemin du répertoire sélectionné</li>
							<li><ins>Error:</ins> Imprime l'erreur dans <code>console.error()</code> et ne retourne rien</li>
						</ul>
					</div>
				</div>
				<div class="channel">
					<div class="send"><span>📤</span>open-file</div>
					<div class="receive"><span>📥</span>selected-file</div>
					<div class="text">
						<p>Ouvrir une boîte de dialogue permettant de sélectionner un fichier</p>
						<ul>
							<li><code>element</code> <xmp class="type"><string></xmp> ID de l'évènement</li>
							<li><code>path</code> <xmp class="type"><string></xmp> Chemin du répertoire par défaut</li>
							<li><ins>Retourne:</ins> <xmp class="type"><string></xmp> Chemin du fichier sélectionné</li>
							<li><ins>Error:</ins> Imprime l'erreur dans <code>console.error()</code> et ne retourne rien</li>
						</ul>
					</div>
				</div>
				<div class="channel">
					<div class="send"><span>📤</span>open-global</div>
					<div class="receive"><span>📥</span>selected-global</div>
					<div class="text">
						<p>Ouvrir une boîte de dialogue permettant de sélectionner un objet selon la configuration passé en paramètre</p>
						<ul>
							<li><code>element</code> <xmp class="type"><string></xmp> ID de l'évènement</li>
							<li><code>options</code> <xmp class="type"><JSON></xmp> <a href="https://www.electronjs.org/docs/latest/api/dialog#dialogshowopendialogbrowserwindow-options">Options</a></li>
							<li><ins>Retourne:</ins> <xmp class="type"><Object></xmp>
								<ul>
									<li><code>canceled</code> <xmp class="type"><boolean></xmp> Si le dialogue a été annulé ou non</li>
									<li><code>filePaths</code> <xmp class="type"><string[]></xmp> Un tableau de chemins de fichiers choisis par l'utilisateur. Si la boîte de dialogue est annulée, ce sera un tableau vide</li>
								</ul>
							</li>
							<li><ins>Error:</ins> Imprime l'erreur dans <code>console.error()</code> et ne retourne rien</li>
						</ul>
					</div>
				</div>
				<div class="channel">
					<div class="send"><span>📤</span>question</div>
					<div class="text">
						<p>Ouvre une fenêtre avec une question</p>
						<ul>
							<li><code>options</code> <xmp class="type"><JSON></xmp> <a href="https://www.electronjs.org/docs/latest/api/dialog#dialogshowmessageboxbrowserwindow-options">Options</a></li>
						</ul>
					</div>
				</div>
				<div class="channel">
					<div class="send"><span>📤</span>error</div>
					<div class="text">
						<p>Ouvre une fenêtre pour afficher une erreur</p>
						<ul>
							<li><code>title</code> <xmp class="type"><string></xmp> Le titre à afficher dans la boîte d'erreur</li>
							<li><code>content</code> <xmp class="type"><string></xmp> Le contenu à afficher dans la boîte d'erreur</li>
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
						<p>Ouvre l'éditeur de texte</p>
						<ul>
							<li><code>path</code> <xmp class="type"><string></xmp> Chemin du fichier</li>
						</ul>
					</div>
				</div>
				<div class="channel">
					<div class="send"><span>📤</span>close</div>
					<div class="text">
						<p>Ferme l'éditeur de texte sans sauvegarder les modifications</p>
					</div>
				</div>
				<div class="channel">
					<div class="send"><span>📤</span>save-file</div>
					<div class="text">
						<p>Sauvegarde les modifications faites au fichier, et ferment l'éditeur</p>
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
						<p>Une nouvelle commande vient d'être exécuté dans le jeu</p>
						<ul>
							<li><ins>Retourne:</ins> <xmp class="type"><JSON></xmp> Commande formatée en JSON (voir <code>MCshell</code> pour plus d'informations)</li>
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
Envoyer des données à un canal spécifique

- `channel` <span class="required">obligatoire</span> <xmp class="type"><string></xmp> Nom du canal
- `args` <span class="required">obligatoire</span> <span class="type"><any[]></span> Liste d'argument(s)

---
### Receive
<div class="stab stable">
	<h3><span>⛉</span>Stable</h3>
</div>

``` ts
MCipc.receive(channel: string, func: Function): void;
```
Recevoir la réponse d'un canal spécifique et transmettre les arguments de retour à une fonction callback.

- `channel` <span class="required">obligatoire</span> <xmp class="type"><string></xmp> Nom du canal
- `funct` <span class="required">obligatoire</span> <xmp class="type"><Function></xmp> Fonction de callback
