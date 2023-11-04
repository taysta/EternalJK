# game/app specific values
export APP_VERSION="1.0"
export ICONSDIR="codemp/macosx"
export ICONSFILENAME="OpenJK"
export PRODUCT_NAME="EternalJK"
export EXECUTABLE_NAME="EternalJK"
export PKGINFO="APPLEEJK"
export COPYRIGHT_TEXT="Jedi Knight Copyright © 2002 Lucasarts. All rights reserved."
export SDKROOT="$(xcrun --sdk macosx --show-sdk-path)"

#constants

source constants.sh

rm -rf "${BUILT_PRODUCTS_DIR}"

#create makefiles with cmake, perform builds with make
rm -rf "${ARM64_BUILD_FOLDER}"
mkdir "${ARM64_BUILD_FOLDER}"
cd "${ARM64_BUILD_FOLDER}" || exit
cmake -DCMAKE_INSTALL_PREFIX=./install -DCMAKE_SYSTEM_PROCESSOR=arm64 -DCMAKE_SYSTEM_NAME=Darwin -DCMAKE_OSX_ARCHITECTURES=arm64 -DUseInternalLibs=ON -DBuildDiscordRichPresence=OFF -DCMAKE_BUILD_TYPE=Release ..
make -j${NCPU}
make install
cd ..

rm -rf "${X86_64_BUILD_FOLDER}"
mkdir "${X86_64_BUILD_FOLDER}"
cd "${X86_64_BUILD_FOLDER}" || exit
cmake -DCMAKE_INSTALL_PREFIX=./install -DCMAKE_SYSTEM_PROCESSOR=x86_64 -DCMAKE_SYSTEM_NAME=Darwin -DCMAKE_OSX_ARCHITECTURES=x86_64 -DUseInternalLibs=ON -DBuildDiscordRichPresence=OFF -DCMAKE_BUILD_TYPE=Release ..
make -j${NCPU}
make install
cd ..

# EternalJK
export PRODUCT_NAME="EternalJK"

# have to re-do all the variables cascading down from PRODUCT_NAME
export WRAPPER_NAME="${PRODUCT_NAME}.app"
export CONTENTS_FOLDER_PATH="${WRAPPER_NAME}/Contents"
export EXECUTABLE_FOLDER_PATH="${CONTENTS_FOLDER_PATH}/MacOS"
export BUNDLE_ID="com.taysta.${PRODUCT_NAME}"
export EXECUTABLE_NAME="eternaljk"
export UNLOCALIZED_RESOURCES_FOLDER_PATH="${CONTENTS_FOLDER_PATH}/Resources"
export FRAMEWORKS_FOLDER_PATH="${CONTENTS_FOLDER_PATH}/Frameworks"

rm -rf "${X86_64_BUILD_FOLDER}/eternaljk.app"
mv "${X86_64_BUILD_FOLDER}/install/JediAcademy/eternaljk.x86_64.app" "${X86_64_BUILD_FOLDER}/${WRAPPER_NAME}"
mv "${X86_64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}/eternaljk.app" "${X86_64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}/${EXECUTABLE_NAME}"
echo mv "${X86_64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}/eternaljk.x86_64" "${X86_64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}/${EXECUTABLE_NAME}"

rm -rf "${ARM64_BUILD_FOLDER}/eternaljk.app"
mv "${ARM64_BUILD_FOLDER}/install/JediAcademy/eternaljk.arm64.app" "${ARM64_BUILD_FOLDER}/${WRAPPER_NAME}"
mv "${ARM64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}/eternaljk.app" "${ARM64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}/${EXECUTABLE_NAME}"
echo mv "${ARM64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}/eternaljk.arm64" "${ARM64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}/${EXECUTABLE_NAME}"

# create the app bundle
# since the one reference in the executable is covered we can skip lipo and dylibbundler in this script
"./build_app_bundle.sh" "skiplipo"

#lipo the executable and libs
lipo "${X86_64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}/${EXECUTABLE_NAME}" "${ARM64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}/${EXECUTABLE_NAME}" -output "${BUILT_PRODUCTS_DIR}/${EXECUTABLE_FOLDER_PATH}/${EXECUTABLE_NAME}" -create
mkdir -p "${BUILT_PRODUCTS_DIR}/${FRAMEWORKS_FOLDER_PATH}" || exit 1;
lipo "${X86_64_BUILD_FOLDER}/${FRAMEWORKS_FOLDER_PATH}/libSDL2-2.0.0.dylib" "${ARM64_BUILD_FOLDER}/${FRAMEWORKS_FOLDER_PATH}/libSDL2-2.0.0.dylib" -output "${BUILT_PRODUCTS_DIR}/${FRAMEWORKS_FOLDER_PATH}/libSDL2-2.0.0.dylib" -create

mkdir "${BUILT_PRODUCTS_DIR}/${EXECUTABLE_FOLDER_PATH}/EternalJK"

cd "${X86_64_BUILD_FOLDER}" || exit
dylibbundler -od -b -x "./${EXECUTABLE_FOLDER_PATH}/rd-eternaljk_x86_64.dylib" -d "./${EXECUTABLE_FOLDER_PATH}/${X86_64_LIBS_FOLDER}/" -p "@executable_path/${X86_64_LIBS_FOLDER}/"
dylibbundler -od -b -x "./${EXECUTABLE_FOLDER_PATH}/rd-rend2e_x86_64.dylib" -d "./${EXECUTABLE_FOLDER_PATH}/${X86_64_LIBS_FOLDER}/" -p "@executable_path/${X86_64_LIBS_FOLDER}/"
dylibbundler -od -b -x "./${EXECUTABLE_FOLDER_PATH}/rd-vulkan_x86_64.dylib" -d "./${EXECUTABLE_FOLDER_PATH}/${X86_64_LIBS_FOLDER}/" -p "@executable_path/${X86_64_LIBS_FOLDER}/"
cd ..
cp "${X86_64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}/rd-eternaljk_x86_64.dylib" "${BUILT_PRODUCTS_DIR}/${EXECUTABLE_FOLDER_PATH}"
cp "${X86_64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}/rd-rend2e_x86_64.dylib" "${BUILT_PRODUCTS_DIR}/${EXECUTABLE_FOLDER_PATH}"
cp "${X86_64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}/rd-vulkan_x86_64.dylib" "${BUILT_PRODUCTS_DIR}/${EXECUTABLE_FOLDER_PATH}"
cp "${X86_64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}/EternalJK/jampgamex86_64.dylib" "${BUILT_PRODUCTS_DIR}/${EXECUTABLE_FOLDER_PATH}/EternalJK"
cp "${X86_64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}/EternalJK/cgamex86_64.dylib" "${BUILT_PRODUCTS_DIR}/${EXECUTABLE_FOLDER_PATH}/EternalJK"
cp "${X86_64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}/EternalJK/uix86_64.dylib" "${BUILT_PRODUCTS_DIR}/${EXECUTABLE_FOLDER_PATH}/EternalJK"

mkdir "${BUILT_PRODUCTS_DIR}/${EXECUTABLE_FOLDER_PATH}/${X86_64_LIBS_FOLDER}"
cp -a "${X86_64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}/${X86_64_LIBS_FOLDER}/." "${BUILT_PRODUCTS_DIR}/${EXECUTABLE_FOLDER_PATH}/${X86_64_LIBS_FOLDER}"

cd "${ARM64_BUILD_FOLDER}" || exit
dylibbundler -od -b -x "./${EXECUTABLE_FOLDER_PATH}/rd-eternaljk_arm64.dylib" -d "./${EXECUTABLE_FOLDER_PATH}/${ARM64_LIBS_FOLDER}/" -p "@executable_path/${ARM64_LIBS_FOLDER}/"
dylibbundler -od -b -x "./${EXECUTABLE_FOLDER_PATH}/rd-rend2e_arm64.dylib" -d "./${EXECUTABLE_FOLDER_PATH}/${ARM64_LIBS_FOLDER}/" -p "@executable_path/${ARM64_LIBS_FOLDER}/"
dylibbundler -od -b -x "./${EXECUTABLE_FOLDER_PATH}/rd-vulkan_arm64.dylib" -d "./${EXECUTABLE_FOLDER_PATH}/${ARM64_LIBS_FOLDER}/" -p"@executable_path/${ARM64_LIBS_FOLDER}/"

cd ..
cp "${ARM64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}/rd-eternaljk_arm64.dylib" "${BUILT_PRODUCTS_DIR}/${EXECUTABLE_FOLDER_PATH}"
cp "${ARM64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}/rd-rend2e_arm64.dylib" "${BUILT_PRODUCTS_DIR}/${EXECUTABLE_FOLDER_PATH}"
cp "${ARM64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}/rd-vulkan_arm64.dylib" "${BUILT_PRODUCTS_DIR}/${EXECUTABLE_FOLDER_PATH}"
cp "${ARM64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}/EternalJK/jampgamearm64.dylib" "${BUILT_PRODUCTS_DIR}/${EXECUTABLE_FOLDER_PATH}/EternalJK"
cp "${ARM64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}/EternalJK/cgamearm64.dylib" "${BUILT_PRODUCTS_DIR}/${EXECUTABLE_FOLDER_PATH}/EternalJK"
cp "${ARM64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}/EternalJK/uiarm64.dylib" "${BUILT_PRODUCTS_DIR}/${EXECUTABLE_FOLDER_PATH}/EternalJK"

mkdir "${BUILT_PRODUCTS_DIR}/${EXECUTABLE_FOLDER_PATH}/${ARM64_LIBS_FOLDER}"
cp -a "${ARM64_BUILD_FOLDER}/${EXECUTABLE_FOLDER_PATH}/${ARM64_LIBS_FOLDER}/." "${BUILT_PRODUCTS_DIR}/${EXECUTABLE_FOLDER_PATH}/${ARM64_LIBS_FOLDER}"

# #sign and notarize
# "../scripts/universal2/sign_and_notarize.sh" "$1"