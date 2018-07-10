#!/bin/sh

mkdir -p include/tikz/core
mkdir -p include/tikz/ui

CORE_FILES=$(grep -r -l TIKZCORE_EXPORT core)
for i in $CORE_FILES ; do
  echo $i
  cp --update "$i" ./include/tikz/core
done

UI_FILES=$(grep -r -l TIKZUI_EXPORT ui)
for i in $UI_FILES ; do
  echo $i
  cp --update "$i" ./include/tikz/ui
done
