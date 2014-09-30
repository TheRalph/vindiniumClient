#!/bin/bash

## default is SSITeam
vindinium_key=uviqc5an
navigator=""

if [ $# -eq 0 ]
then
    ### just use the default !
    echo "use SSITeam key!"
elif [ $# -eq 1 ]
then
    vindinium_key=$1
elif [ $# -eq 2 ]
then
    vindinium_key=$1
    navigator=$2
else
    echo "Syntax: $0 vindinium_key [navigator]"
    exit 0
fi

rm -rf build
mkdir -p build
cd build

echo "#!/bin/bash" > testIA.sh
echo "../bin/vindiniumLauncher $vindinium_key random $navigator" >> testIA.sh
chmod +x testIA.sh

echo "#!/bin/bash" > mitchIA.sh
echo "../bin/vindiniumLauncher $vindinium_key botMitch $navigator" >> mitchIA.sh
chmod +x mitchIA.sh

echo "#!/bin/bash" > TheRalphBot.sh
echo "../bin/vindiniumLauncher $vindinium_key TheRalphBot $navigator" >> TheRalphBot.sh
chmod +x TheRalphBot.sh

ln -s ../bin/vindiniumLauncher vindiniumLauncher

cmake ..
cd ..
