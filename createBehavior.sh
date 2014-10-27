#!/bin/bash

if [ ! $# -eq 1 ]
then
    echo "synthax $0 newBehaviorName"
    exit 0
fi

newBehaviorName=$1
newBehaviorPath="behaviorModulesSrc/$newBehaviorName"

if [ -e $newBehaviorPath ]
then
    echo "Error: the behavior '$newBehaviorName' already exists"
    exit 1
fi

randomBehaviorPath="behaviorModulesSrc/random"
if [ ! -d $randomBehaviorPath ]
then
    echo "Error: the behavior 'random' can not be found"
    exit 1
fi

cp -r $randomBehaviorPath $newBehaviorPath

NewBehaviorName=$(tr '[:lower:]' '[:upper:]' <<< ${newBehaviorName:0:1})${newBehaviorName:1}
className="C${NewBehaviorName}BehaviorModule"
classFilePath=$newBehaviorPath/$className

mv $newBehaviorPath/CRandomBehaviorModule.cpp $classFilePath.cpp
mv $newBehaviorPath/CRandomBehaviorModule.h $classFilePath.h

sed -i "s/CRandomBehaviorModule/$className/g" $classFilePath.cpp
sed -i "s/CRandomBehaviorModule/$className/g" $classFilePath.h
sed -i "s/Random Behavior/$NewBehaviorName Behavior/g" $classFilePath.h
sed -i "s/Raphaël La Greca <raphael.la.greca+vindinium@gmail.com>/$USER/g" $classFilePath.h

sed -i "s/\"random\"/\"$newBehaviorName\"/g" $classFilePath.cpp
sed -i "s/a random displacement/a new random displacement/g" $classFilePath.cpp

isBehaviorExistInMakeLists=$(grep "### $NewBehaviorName" CMakeLists.txt | wc -l)

if [ $isBehaviorExistInMakeLists -eq 0 ]
then
cat >> CMakeLists.txt << EOF

### $NewBehaviorName
add_behavior_module( $newBehaviorName $className )
EOF
fi

