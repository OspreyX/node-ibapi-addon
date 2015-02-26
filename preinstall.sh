DIR="./import"
LIBJSON="./libjson"
rm -rf ./import
if [ ! -d "$DIR" ]
then
    mkdir $DIR
fi

if [ "$(expr substr $(node -v) 4 5)" = "10" ]; then
mv NodeIbapiTen.cpp NodeIbapi.cpp
mv NodeIbapiTen.h NodeIbapi.h
mv IbJsonParserTen.cpp IbJsonParser.cpp
fi

cd $DIR
rm twsapi_macunix.971.01.jar
wget http://interactivebrokers.github.io/downloads/twsapi_macunix.971.01.jar
unzip twsapi_macunix.971.01.jar
cp ./IBJts/source/PosixClient/Shared/* .
cp ./IBJts/source/PosixClient/src/* .

if [ "$(uname)" = "Darwin" ]; then
sed -ie $'4i\\\n#define IB_USE_STD_STRING\n' ./EClientSocketBase.cpp
sed -ie $'4i\\\n#define IB_USE_STD_STRING\n' ./EPosixClientSocket.cpp
sed -ie 's_// LINUX_#include <unistd.h>_g' ./EPosixClientSocketPlatform.h
sed -ie 's_#include "StdAfx.h"__g' ./EClientSocketBaseImpl.h
elif [ "$(expr substr $(uname -s) 1 5)" = "Linux" ]; then
sed -i '4i#define IB_USE_STD_STRING' ./EClientSocketBase.cpp
sed -i '4i#define IB_USE_STD_STRING' ./EPosixClientSocket.cpp
sed -i 's_// LINUX_#include <unistd.h>_g' ./EPosixClientSocketPlatform.h
sed -i 's_#include "StdAfx.h"__g' ./EClientSocketBaseImpl.h
elif [ "$(expr substr $(uname -s) 1 10)" = "MINGW32_NT" ]; then
sed -i '4i#define IB_USE_STD_STRING' ./EClientSocketBase.cpp
sed -i '4i#define IB_USE_STD_STRING' ./EPosixClientSocket.cpp
sed -i 's_// LINUX_#include <unistd.h>_g' ./EPosixClientSocketPlatform.h
sed -i 's_"StdAfx.h"_"afx.h"_g' ./EClientSocketBaseImpl.h
sed -i 's/snprintf/_snprintf/g' ./EClientSocketBaseImpl.h
fi

wget -U "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.10; rv:34.0) Gecko/20100101 Firefox/34.0" http://softlayer-dal.dl.sourceforge.net/project/libjson/libjson_7.6.1.zip
unzip libjson_7.6.1.zip

if [ "$(uname)" = "Darwin" ]; then
sed -ie 's@#define JSON_LIBRARY@//#define JSON_LIBRARY@g' ./libjson/JSONOptions.h
elif [ "$(expr substr $(uname -s) 1 5)" = "Linux" ]; then
sed -i 's@#define JSON_LIBRARY@//#define JSON_LIBRARY@g' ./libjson/JSONOptions.h
elif [ "$(expr substr $(uname -s) 1 10)" = "MINGW32_NT" ]; then
sed -i 's/#warning, Release build of libjson, but NDEBUG is not on//g' ./libjson/_internal/Source/JSONDefs.h
sed -i 's/#error, Release build of libjson, but NDEBUG is not on//g' ./libjson/_internal/Source/JSONDefs.h
sed -i 's@#define JSON_LIBRARY@//#define JSON_LIBRARY@g' ./libjson/JSONOptions.h
fi

