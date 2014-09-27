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
echo "#!/bin/bash" > build/testIA.sh
echo "../bin/vindiniumLauncher $vindinium_key random $navigator" >> build/testIA.sh
chmod +x build/testIA.sh
echo "#!/bin/bash" > build/michIA.sh
echo "../bin/vindiniumLauncher $vindinium_key botMitch $navigator" >> build/mitchIA.sh
chmod +x build/mitchIA.sh
cd build
cmake ..
cd ..