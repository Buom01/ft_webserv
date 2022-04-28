## Advancement

![Alt image](./srcs/img/data/plugins/advancement/main.png)

> Create your own advancements

- [Select a advancement](#select-a-advancement)
- [Create a new advancement](#create-a-new-advancement)
- [Modify a advancement](#modify-a-advancement)
  - [Root](#root)
  - [Display](#display)
  - [Criteria](#criteria)
  - [Requirements](#requirements)
  - [Rewards](#rewards)
- [Generate a advancement](#delete-a-advancement)
- [Delete a advancement](#delete-a-advancement)
---
### Select a advancement

<img src="./srcs/img/data/plugins/advancement/list.png" style="width: 250px;"/>

To select a advancement, click on the one you want in the list. The selected advancement will have an arrow displayed on its left side.

---
### Create a new advancement

<img src="./srcs/img/data/plugins/advancement/list.png" style="width: 250px;"/>

At the top of the list, simply click on the ``New Advancement`` button.

### Modify a advancement
![modify_advancement](./srcs/img/data/plugins/advancement/graph.png)

When a advancement is selected, a graph composed of nodes and form will appear on the right:
1. Name of the current advancement (will not be displayed in the game)
2. Generate advancement
3. Delete advancement
4. Root of advancement, this node cannot be deleted
5. Selected node
6. Add a parent to the selected node
7. Add a child to the selected node
8. Save the selected node

The form below this area will allow you to modify the selected node

#### Root
![root_advancement](./srcs/img/data/plugins/advancement/root.png)

> Change the display information of your current progress. This data is applied to all nodes
- **Namespace**: In-game namespace 
- **Background image**: Background texture applied in the corresponding window

#### Display
![display_advancement](./srcs/img/data/plugins/advancement/display.png)

> Change the display of the selected node in the game
1. Node title :
  - **Title**: Node title
  - **Title color**: Title color
  - **Bold / Italic / Underlined / Strikethrough / Obfuscated**: Formatting of the title
2. Node description :
  - **Description**: Node description
  - **Description color**: Description color
  - **Bold / Italic / Underlined / Strikethrough / Obfuscated**: Formatting of the description
3. Node icon, can be a block or an element
4. Type of frame :
   
   <img src="./srcs/img/data/plugins/advancement/frame.png" style="image-rendering: crisp-edges;image-rendering: pixelated;-ms-interpolation-mode: nearest-neighbor;"/>
   
   From left to right: Normal / Goal / Challenge
5. NBT data
6. Displays a notification on the player's screen if a player gets the progress
7. Announcement in the chat if a player gets the progress
8. No notification will be shown

#### Criteria
![criteria_advancement](./srcs/img/data/plugins/advancement/criteria.png)

> Add a criteria to the current node 

To add a new criteria, click on the button at the bottom of the area ``Add a trigger``.
A new block will then appear, with a pre-filled randomly generated name. You can change it, but there must not be two equivalent names on the same node.
In the search bar, you can select one of the many existing triggers, and customize them completely. Once selected, a form will appear at the bottom.
To delete a criteria, click on the cross at the top right of the block.

#### Requirements
![criteria_advancement](./srcs/img/data/plugins/advancement/requirements.png)

> Define the ways in which progress will be validated

The idea behind the requirements is simple, it is to know with which criteria the progress will be validated. It is quite possible to create several ways. For example, let's imagine having the criteria A, B and C. I can decide that the progress will be achieved if the player passes A and B OR B and C.

#### Rewards
![criteria_advancement](./srcs/img/data/plugins/advancement/rewards.png)

> Define the rewards and actions to be performed in case of success

- **Recipes**: Add recipes that the player will receive as a reward (he will receive the result of the recipes but will not unlock them)
- **Loot tables**: Add loot tables that the player will receive as a reward
- **Experiences**:  Indicate what the player will receive in experience
- **Function**: Write a valid Minecraft function that will be executed

---
### Generate a advancement

![criteria_advancement](./srcs/img/data/plugins/advancement/butons.png)

Select a advancement [(see here)](#select-a-advancement); then click on the button ``Generate complete advancement`` and validate your request. Depending on the complexity of the progress, it may take a few seconds

---
### Delete a advancement

![criteria_advancement](./srcs/img/data/plugins/advancement/butons.png)

Select a advancement [(see here)](#select-a-advancement); then click on the button ``Delete complete advancement`` and validate your request. Beware that this will also delete the files in the corresponding datapack if it has been generated
