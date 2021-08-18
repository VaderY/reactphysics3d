#!/bin/bash

echo include/reactphysics3d
mv include/reactphysics3d include/rp3d
mv include/rp3d/reactphysics3d.h include/rp3d/rp3d.h

files=()
while IFS=  read -r -d $'\0'; do
    files+=("$REPLY")
done < <(find include src \( -name '*.h' -or -name '*.cpp' \) -type f -print0)

for i in "${files[@]}"; do
    echo $i
	sed -i 's/reactphysics3d *::/rp3d::/g' $i
	sed -i 's/namespace \+reactphysics3d/namespace rp3d/g' $i

	sed -i 's/#include <reactphysics3d\//#include <rp3d\//g' $i
	sed -i 's/#include "reactphysics3d\//#include "rp3d\//g' $i

	sed -i 's/namespace rp3d = reactphysics3d;/\/\/namespace rp3d = reactphysics3d;/g' $i
done

echo CMakeLists.txt
sed -i 's/reactphysics3d/rp3d/g' CMakeLists.txt
sed -i 's/ReactPhysics3D/RP3D/g' CMakeLists.txt
