#!/bin/bash

cp bin/libignition.a /usr/lib/libignition.a
cp bin/implosion /usr/bin/implosion
cp bin/implosion-hub /usr/bin/implosion-hub

if [ ! -d "/usr/share/ignition" ]; then
   mkdir "/usr/share/ignition"
fi

cp icon.png /usr/share/ignition/icon.png
