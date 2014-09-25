#!/bin/bash

## default is SSITeam
vindinium_key=uviqc5an

if [ $# -eq 0 ]
then
    ### just use the default !
    echo "use SSITeam key!"
elif [ $# -eq 1 ]
then
    vindinium_key=$1
else
    echo "Syntax: $0 vindinium_key"
    exit 0
fi

rm -rf build
mkdir -p build
echo "#!/bin/bash" > build/myIA.sh
echo "../bin/vindiniumTest $vindinium_key" >> build/myIA.sh
chmod +x build/myIA.sh
echo "#!/bin/bash" > build/michIA.sh
echo "../bin/botMitch $vindinium_key" >> build/mitchIA.sh
chmod +x build/mitchIA.sh
cd build
cmake ..
cd ..