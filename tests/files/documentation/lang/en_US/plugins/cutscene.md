## Cutscene

![Alt image](./srcs/img/data/plugins/cutscene/main.png)

> Create cutscenes in a simple and professional way 

- [Select a cutscene](#select-a-cutscene)
- [Create a new cutscene](#create-a-new-cutscene)
- [Modify a cutscene](#modify-a-cutscene)
  - [Add a point](#add-a-point)
  - [Delete a point](#delete-a-point)
  - [Add a transition](#add-a-transition)
  - [Change the transition time](#change-the-transition-time)
- [Delete a cutscene](#delete-a-cutscene)
- [Playing a cutscene](#playing-a-cutscene)
---
### Select a cutscene

In order to work on a cutscene, it is necessary to choose one in the drop-down menu located on the top left. Click on it to make them appear in a list below the area.
#### <span>&#x26a0;</span> In order for the add point system to be functional, it is necessary to have a cutscene to select
---
### Create a new cutscene

![Alt image](./srcs/img/data/plugins/cutscene/main.png)

To create a cutscene, you can either do it via the application or directly in the game:
| | |
| --- | --- |
| ![Alt image](./srcs/img/data/plugins/cutscene/create_manual.png) | Click on the button ``Create a cutscene``, enter the name of it and validate the form. |
| ![Alt image](./srcs/img/data/plugins/cutscene/create_auto.png) | Click on the button ``New cutscene``, open the application, enter the name of the application and validate the form. |

### Modify a cutscene
![Alt image](./srcs/img/data/plugins/cutscene/panel.png)

 - #### Add a point
   ![Alt image](./srcs/img/data/plugins/cutscene/add_point.png)
  
   Select the cursor and click on the ``Add Point`` button. Your position and orientation will be used as a point for your cutscene.

- #### Delete a point
  In the cutscene panel, click on the rightmost delete button of the point

- #### Add a transition
  There are 5 types of transitions (based on bezier curves) [^1] :
  | Transition | Explanation |
  | --- | --- |
  | linear | Transition is constant, **default** |
  | ease | Transition starts slowly, accelerates sharply, then gradually slows down towards the end |
  | ease-in | The transition starts slowly, accelerates gradually to the end, where it stops abruptly |
  | ease-out | The transition starts abruptly, then gradually slows down towards the end |
  | ease-in-out | The transition starts slowly, speeds up, then slows down towards the end |

- #### Change the transition time
  The transition duration is the time in seconds between the current point and the next point, which can be changed in the point duration field.
---
### Delete a cutscene

![Alt image](./srcs/img/data/plugins/cutscene/delete.png)

Select a cutscene [(see here)](#select-a-cutscene); then click on the button ``Delete cutscene`` and validate your request

---
### Playing a cutscene

For an entity to follow the path created for a cutscene, it is necessary to have two tags:
- ``Cutscene``, which allows the entity to launch a cutscene
- ``Cutscene_id``, which when added to the entity launches the corresponding cutscene. The cutscene is launched immediately after adding it. This tag is displayed in the application list next to the magnifying glass icon


[^1]: https://en.wikipedia.org/wiki/B%C3%A9zier_curve
