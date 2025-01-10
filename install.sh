#!/bin/bash

cp bin/libignition.so /usr/lib/
cp bin/libig-imgui.a /usr/lib/
cp bin/libpreglsl.a /usr/lib/
cp bin/implosion-hub /usr/bin/
cp bin/implosion /usr/bin/
cp bin/ignition-runtime /usr/bin

if [ ! -d "/usr/share/ignition" ]; then
   mkdir "/usr/share/ignition"
fi

cp icon.png /usr/share/ignition/icon.png
