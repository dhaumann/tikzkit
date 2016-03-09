#!/bin/sh

mkdir -p include/EntitySystem
mkdir -p include/tikz/core
mkdir -p include/tikz/ui

ES_FILES=$(grep -r -l ES_EXPORT entitysystem)
for i in $ES_FILES ; do
  echo $i
  cp --update "$i" ./include/EntitySystem
done

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
