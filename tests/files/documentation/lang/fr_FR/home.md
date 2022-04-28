# Bienvenue dans la documentation Mapcraft

- [Qu'est-ce que Mapcraft ?](#qu%E2%80%99est-ce-que-mapcraft-%3F)
- [Comment ça marche ?](#comment-%C3%A7a-marche-%3F)
- [Installation](#installation)
- [Obtenir de l'aide](#obtenir-de-l%E2%80%99aide)
---

## Qu'est-ce que Mapcraft ?

Mapcraft est un ensemble comprenant un logiciel, un resourcepack et un datapack. Agissant comme une boîte à outils géante, Mapcraft augmente sans configuration les possibilités des créateurs de cartes. Son grand avantage est sa simplicité, toutes les modifications faites par le logiciel sont directement appliqué à votre monde, et vice-versa.

<div class="stab experimental">
	<p style="text-align: center;">Mapcraft étant en cours de développement et toujours en alpha, il est toujours possible de rencontrer des bugs. Néanmoins la majorité du temps, celui-ci fonctionnera sans réel problème</p>
</div>

## Comment ça marche ?

<div style="display: inline-flex; align-items: center;">
	<div style="margin-right: 1em">
		<p>Le diagramme va vous permettre de mieux visualiser le fonctionnement et ces étapes :</p>
		<ol>
			<li>Mapcraft va ce lier à votre monde</li>
			<li>Installation du datapack et du resource pack officiel dans votre monde, rendant accessible les outils de développement dans le jeu</li>
			<li>Création du resourcepack et du datapack contenant tous le contenu qui sera créé</li>
			<li>Lecture en direct du fichier de log du jeu, permettant à Mapcraft de lire le chat et de détecter les commandes shell envoyées dans celui-ci</li>
		</ol>
	</div>
	<img src="./srcs/img/data/diagram.png"/>
</div>

## Installation

Mapcraft est disponible sur les trois principaux systèmes d'exploitation. Rendez-vous sur la [page de téléchargement](https://mapcraft.app/download.html) pour obtenir le fichier correspondant à votre système.

## Obtenir de l'aide

Êtes-vous coincé quelque part ? Voici quelques liens vers des endroits où chercher :

- Si vous avez besoin d'aide pour développer votre plugin ou sur une fonctionnalité, [le serveur Discord](https://discord.gg/PMEFsW3ac9) est un excellent endroit pour obtenir des conseils.
- Si vous soupçonnez que vous rencontrez un bug avec Mapcraft, veuillez consulter [le suivi de tickets](https://gitlab.com/cbertran/mapcraft/-/issues) pour voir si des problèmes existants correspondent au vôtre. Si ce n'est pas le cas, n'hésitez pas à remplir notre modèle de rapport de bug et à nous le soumettre.
