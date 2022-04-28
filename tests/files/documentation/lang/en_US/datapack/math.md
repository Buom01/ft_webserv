## Mathematics
> Set of functions to compute common mathematical operations and transformations

- [Parameters](#parameters)
- [Functions](#functions)
  - [absolute](#absolute)
  - [cos](#cos)
  - [pow](#pow)
  - [rand](#rand)
  - [reset](#reset)
  - [sin](#sin)
  - [sqrt](#sqrt)
  - [tan](#tan)
  - [wrap_angle](#wrap_angle)
- [Example](#example)

### Parameters
| MC_Calc | |
| --- | --- |
| **Input**| Main input |
| **Input2**| Secondary input |
| **Input3**| Tertiary Input |
| **Output** | Output |

### Functions
- #### absolute
  >  Returns the absolute value of a number
  ```js
  function mapcraft:built_in/math/absolute
  ```
  | Parameters |  |
  | --- | --- |
  | **Input** | Input |
  | **Output** | Output |
- #### cos
  >  Returns the cosine of an angle in degrees (scaled up by 100)
  ```js
  function mapcraft:built_in/math/cos
  ```
  | Parameters |  |
  | --- | --- |
  | **Input** | Angle in degrees |
  | **Output** | Cosine of the angle |
- #### pow
  >  Returns the base raised to the power of the exponent
  ```js
  function mapcraft:built_in/math/pow
  ```
  | Parameters |  |
  | --- | --- |
  | **Input** | Base |
  | **Input2** | Exponent |
  | **Output** | Base raised to the power of the exponent |
- #### rand
  >  Returns a random number
  ```js
  function mapcraft:built_in/math/rand
  ```
  | Parameters |  |
  | --- | --- |
  | **Output** | Negative or positive random number |
- #### reset
  >  Resets all variables of the object
  ```js
  function mapcraft:built_in/math/reset
  ```
  Can be useful in case of strange problems with your calculations, but it is not necessary to use it all the time
- #### sin
  >  Returns the sine of an angle in degrees (scaled up by 100)
  ```js
  function mapcraft:built_in/math/sin
  ```
  | Parameters |  |
  | --- | --- |
  | **Input** | Angle in degrees |
  | **Output** | Sine of the angle |
- #### sqrt
  >  Returns the square root of a number
  ```js
  function mapcraft:built_in/math/sqrt
  ```
  | Parameters |  |
  | --- | --- |
  | **Input** | Number |
  | **Output** | Square root of the number |
- #### tan
  >  Returns the tangent of an angle in degrees 
  ```js
  function mapcraft:built_in/math/tan
  ```
  | Parameters |  |
  | --- | --- |
  | **Input** | Angle in degrees |
  | **Output** | Tangent of the angle |
- #### wrap_angle
  >  Returns an angle between -180° and 180°
  ```js
  function mapcraft:built_in/math/wrap_angle
  ```
  | Parameters |  |
  | --- | --- |
  | **Input** | Angle in degrees |
  | **Output** | Angle between -180° and 180° |

### Example

1. We wish to obtain a set number ``>= 0``
   ```js
   function mapcraft:built_in/math/rand
   scoreboard players operation Input MC_Calc = Output MC_Calc
   function mapcraft:built_in/math/absolute
   ```
2. We wish to obtain the square root of the number ``75086142``
   ```js
   scoreboard players set Input MC_Calc 75086142
   function mapcraft:built_in/math/sqrt
   ```
