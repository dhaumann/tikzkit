#!/bin/sh

mkdir -p include/tikz/core
mkdir -p include/tikz/ui

CORE_FILES=$(grep -R -l TIKZCORE_EXPORT core)
for i in $CORE_FILES ; do
  echo $i
  cp "$i" ./include/tikz/core
done

UI_FILES=$(grep -R -l TIKZUI_EXPORT ui)
for i in $UI_FILES ; do
  echo $i
  cp "$i" ./include/tikz/ui
done
