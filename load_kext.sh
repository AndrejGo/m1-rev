#!/bin/bash

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi

cp -R ./PMCKext2.kext /tmp
kextutil /tmp/PMCKext2.kext