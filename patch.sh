#!/bin/bash

echo src/rp3d
mv src rp3d
mkdir src
mv rp3d src/

files=()
while IFS=  read -r -d $'\0'; do
    files+=("$REPLY")
done < <(find src \( -name '*.h' -or -name '*.cpp' \) -type f -print0)

for i in "${files[@]}"; do
    echo $i
	sed -i 's/reactphysics3d *::/rp3d::/g' $i
	sed -i 's/namespace \+reactphysics3d/namespace rp3d/g' $i

	sed -i 's/#include "configuration.h"/#include <rp3d\/configuration.h>/g' $i
	sed -i 's/#include "decimal.h"/#include <rp3d\/decimal.h>/g' $i
	sed -i 's/#include "reactphysics3d.h"/#include <rp3d\/reactphysics3d.h>/g' $i

	sed -i 's/#include "body\/\(.*\).h"/#include <rp3d\/body\/\1.h>/g' $i
	sed -i 's/#include "collision\/\(.*\).h"/#include <rp3d\/collision\/\1.h>/g' $i
	sed -i 's/#include "constraint\/\(.*\).h"/#include <rp3d\/constraint\/\1.h>/g' $i
	sed -i 's/#include "containers\/\(.*\).h"/#include <rp3d\/containers\/\1.h>/g' $i
	sed -i 's/#include "engine\/\(.*\).h"/#include <rp3d\/engine\/\1.h>/g' $i
	sed -i 's/#include "mathematics\/\(.*\).h"/#include <rp3d\/mathematics\/\1.h>/g' $i
	sed -i 's/#include "memory\/\(.*\).h"/#include <rp3d\/memory\/\1.h>/g' $i
	sed -i 's/#include "utils\/\(.*\).h"/#include <rp3d\/utils\/\1.h>/g' $i
done

echo CMakeLists.txt
sed -i 's/    "src\//    "src\/rp3d\//g' CMakeLists.txt
sed -i 's/PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}\/reactphysics3d/#/g' CMakeLists.txt
echo 'INSTALL(' >> CMakeLists.txt
echo '        DIRECTORY ${CMAKE_SOURCE_DIR}/src/' >> CMakeLists.txt
echo '        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}' >> CMakeLists.txt
echo '        FILES_MATCHING PATTERN "*.h*"' >> CMakeLists.txt
echo ')' >> CMakeLists.txt
