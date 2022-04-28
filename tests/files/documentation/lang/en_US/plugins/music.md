## Music
![music_main](./srcs/img/data/plugins/music/main.png)
> Manage music, sound effects or dialogues

- [Add](#add)
- [Modify](#modify)
- [Delete](#delete)
---
### Add

![music_main](./srcs/img/data/plugins/music/add.png)

Click on ``Add music``, then on the new window drag an audio file in ``.ogg`` format [^1]. No other format is allowed (Minecraft only allows this format).
Add a name and the folder where the music will be saved.

---

### Modify
![music_main](./srcs/img/data/plugins/music/form.png)

You will find below an exhaustive list of the available options:
- **Sound Name** : Name of the music
- **category** : the option is deprecated since 1.10, but will allow you to better categorize your music
- **replace** : if active, the music will replace the default music with the same name, ***not active*** by default
- **subtitle** : subtitle of the music if the option ``Show subtitles`` is active in the game, ***Optional***
- **attenuation_distance** : change the attenuation distance of the music, ***16*** by default
- **pitch** : defines the pitch of the sound, ***1.0*** by default
- **preload** : Indicates that the music should be preloaded when the resource pack is loaded, ***not active*** by default
- **stream** : allows to play music in stream mode. Recommended for music longer than 4 seconds. <br/> In the game a maximum of 4 instances of the same music is possible with this option, against a much higher number without, ***not active*** by default
- **type** : two values are available, ***sound*** by default :

  | Sound | Event |
  | --- | --- |
  | The value of **name** will be interpreted as the name of the file | The value of ***name*** will be interpreted as the name of an event already defined in the game. |

- **volume** : the volume of the sound in the game, defaulted to ***1.0***
- **weight** : the probability that the music is selected if the music is in event, default ***1.0***
---
### Delete

Select your music from the list, scroll down to the bottom of the edit area and click on ``Delete this music``

[^1]: https://en.wikipedia.org/wiki/Ogg
