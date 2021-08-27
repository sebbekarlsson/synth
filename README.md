# synth
> Midi synth software.  
> (Work in progress)

## Building it
> First make sure you have these:

* SDL2
* cmake
* C compiler (gcc)


> Then run:

``` bash
mkdir build && cd build && cmake .. && make
```

> Then to start it:

```
    ./synth
```

> Use a midi-keyboard to play notes.

## Note
> You might want to go to `src/midi_input.c`
> And change this line:

``` C
#define MIDI_INPUT "/dev/midi2"
```

> To a midi device on your system.  
> You can find your devices by running:

``` bash
find /dev | grep midi
```

> Try one that works.
