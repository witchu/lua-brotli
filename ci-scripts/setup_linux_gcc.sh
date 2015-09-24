#! /bin/bash

source ci-scripts/platform.sh

if [ "$PLATFORM" == "linux" ]; then
  sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
  sudo apt-get update -qq
  CXX_PATH=$(which g++)
  sudo apt-get install -qq g++-4.8
  sudo rm $CXX_PATH
  sudo ln -s g++-4.8 $CXX_PATH
fi
