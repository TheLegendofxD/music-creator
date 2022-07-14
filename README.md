# Paul's Music Creator
![GitHub](https://img.shields.io/github/license/TheLegendofxD/music-creator)
![GitHub top language](https://img.shields.io/github/languages/top/TheLegendofxD/music-creator)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/TheLegendofxD/music-creator)

A simple way for creating music from text files.

## Usage
```
pm-creator.exe -i input.pmcf -o out.wav
```
## The Basics of a pmcf-file
### Data-Types
Name | Example | Description
--- | --- | ---
bool | true | A value that can only be **true**/**on** or **false**/**off**
float_num | 0.4 | A *floating point number*
note_name | c4 | Even though it does contain characters it **CAN'T** be within *quotaion marks*
wave_type | sine | It can be either **square** or **sine**
### Commands
Command | Example | Description
--- | --- | ---
freq <float_num> | **freq** 345.56 | *Plays the frequency* given as parameter
note <note_name> | **note** f#3 | *Plays the note* given as parameter
pause | **pause** | Creates a short silence
set_duration <float_num> | **set_duration** 0.4 | *Sets the duration* of any **note**, **pause** or **freq**uency after it
set_fadeout <bool: optional> | **set_fadeout** false | *Switches the fadeout* of notes either on or off. If no paramter is given it sets it to the opposite state
set_volume <float_num> | **set_volume** 0.7 | Changes the volume of any **note** or **freq**uency after it
set_wave <wave_type> | **set_wave** square | Changes the waveform of any **note** or **freq**uency after it
