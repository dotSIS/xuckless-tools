#!/bin/sh

# merge xresources
xrdb merge ~/.Xresources &

# run lxpolkit to manage authorizations on dwm
lxpolkit &

# set keyboard rate
#xset r rate 200 50 &

# run picom compositor in bg
picom &

# set default brightness and disable screendim
xbacklight -set 10 &
xset dpms 0 0 0 &
xset s off &

# run eww status bar in bg
eww open eww &

# run chadwm
~/Configs/xuckless-tools/chadwm/scripts/bar.sh &
while type chadwm >/dev/null; do chadwm && continue || break; done
