# GNUitar (LV2 Port)
## What
This is the LV2 port of the **legendary** [GNUitar](https://sourceforge.net/projects/gnuitar/) guitar effects processor. 

From the GNUitar official website:
> GNUitar is guitar effects software that allows you to use your PC as guitar processor. It includes the following effects: wah-wah, sustain, distortion, reverberator, echo, delay, tremolo, vibrato, and chorus/flanger.

When I started dabbling in Audio Production on Linux almost two decades ago, GNUitar was my go to application for guitar effects. The Tube Amplifier effect was one of my favorite effects, clearly audible on many of my older songs:
- [Tum na Mile](https://music.shaji.in/media/Abstractions%20Inconclusive%20(2009)/Tum%20na%20Mile.mp3)
- [Junoon](https://music.shaji.in/media/Abstractions%20Inconclusive%20(2009)/Junoon.mp3)
- [Rehnuma](https://music.shaji.in/media/Give%20Time%20Its%20Time%20(2011)/Rehnuma.mp3)

and some of the recent ones too:
- [Sound of Freedom](https://music.shaji.in/media/ACE%20Riff%20Project%20(With%20Rohan)/The%20Sound%20of%20Freedom.mp3)

I had a x86 compiled binary with jack support which I used to use, but after the world shifted to x86_64 and pipewire, the old binary wouldn't work anymore. I tried to find newer updated source with jack support, but couldn't ([until recently](https://github.com/anarsoul/gnuitar)). Even the official source on Sourceforge.net hadn't been updated, and I was unable to get in touch with the original developer Max.

So, after I found the new updated code thanks to [anarsoul](https://github.com/anarsoul), I started porting the effects to LV2, to preserve them and hopefully make them easily usable to others in the future. 

I have a great sentimental attachment to GNUitar, and hopefully my little  contribution might help sustain the app for the next generation.

## Building
Simply run make in each folder and copy the folder to ~/.lv2

## License
GPL v2 as per the original source. 
## Thanks
Thanks to the original developers for making this great program.