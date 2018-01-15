#!/bin/bash
cmake -DCMAKE_INSTALL_PREFIX=/home/ecb/Falaise-local -DCMAKE_PREFIX_PATH="$(brew --prefix);$(brew --prefix qt5-base)" $@
