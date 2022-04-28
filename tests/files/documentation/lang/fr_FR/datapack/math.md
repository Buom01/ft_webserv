## Mathématique
> Ensemble de fonctions permettant de calculer des opérations et des transformations mathématiques courantes

- [Paramètres](#paramètres)
- [Fonctions](#fonctions)
  - [absolute](#absolute)
  - [cos](#cos)
  - [pow](#pow)
  - [rand](#rand)
  - [reset](#reset)
  - [sin](#sin)
  - [sqrt](#sqrt)
  - [tan](#tan)
  - [wrap_angle](#wrap_angle)
- [Exemple](#exemple)

### Paramètres
| MC_Calc | |
| --- | --- |
| **Input**| Entrée principale |
| **Input2**| Entrée secondaire |
| **Input3**| Entrée tertiaire |
| **Output** | Sortie |

### Fonctions
- #### absolute
  >  Renvoie la valeur absolue d'un nombre
  ```js
  function mapcraft:built_in/math/absolute
  ```
  | Paramètres |  |
  | --- | --- |
  | **Input** | Entrée |
  | **Output** | Sortie |
- #### cos
  >  Renvoie le cosinus d'un angle en degrés (mise à l'échelle par 100)
  ```js
  function mapcraft:built_in/math/cos
  ```
  | Paramètres |  |
  | --- | --- |
  | **Input** | Angle en degrés |
  | **Output** | Cosinus de l'angle |
- #### pow
  >  Renvoie la base élevée à la puissance de l'exposant
  ```js
  function mapcraft:built_in/math/pow
  ```
  | Paramètres |  |
  | --- | --- |
  | **Input** | Base |
  | **Input2** | Exposant |
  | **Output** | Base élevée à la puissance de l'exposant |
- #### rand
  >  Renvoie un nombre aléatoire
  ```js
  function mapcraft:built_in/math/rand
  ```
  | Paramètres |  |
  | --- | --- |
  | **Output** | Nombre aléatoire négatif ou positif |
- #### reset
  >  Remets à zéro toutes les variables de l'objet
  ```js
  function mapcraft:built_in/math/reset
  ```
  Peut-être utile en cas de problème étrange sur vos calculs, mais il n'est aucunement nécessaire de l'utiliser tout le temps
- #### sin
  >  Renvoie le sinus d'un angle en degrés (mise à l'échelle par 100)
  ```js
  function mapcraft:built_in/math/sin
  ```
  | Paramètres |  |
  | --- | --- |
  | **Input** | Angle en degrés |
  | **Output** | Sinus de l'angle |
- #### sqrt
  >  Renvoie la racine carrée d'un nombre
  ```js
  function mapcraft:built_in/math/sqrt
  ```
  | Paramètres |  |
  | --- | --- |
  | **Input** | Nombre |
  | **Output** | Racine carrée du nombre |
- #### tan
  >  Renvoie la tangente d'un angle en degrés 
  ```js
  function mapcraft:built_in/math/tan
  ```
  | Paramètres |  |
  | --- | --- |
  | **Input** | Angle en degrés |
  | **Output** | Tangente de l'angle |
- #### wrap_angle
  >  Renvoie un angle situé entre -180° et 180°
  ```js
  function mapcraft:built_in/math/wrap_angle
  ```
  | Paramètres |  |
  | --- | --- |
  | **Input** | Angle en degrés |
  | **Output** | Angle située entre -180° et 180° |

### Exemple

1. On souhaiter obtenir un nombre alétoire ``>= 0``
   ```js
   function mapcraft:built_in/math/rand
   scoreboard players operation Input MC_Calc = Output MC_Calc
   function mapcraft:built_in/math/absolute
   ```
2. On souhaite obtenir la racine carée du nombre ``75086142``
   ```js
   scoreboard players set Input MC_Calc 75086142
   function mapcraft:built_in/math/sqrt
   ```
