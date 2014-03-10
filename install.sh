DIR="./import"
PDIR="../"

if [ ! -d "$DIR" ]
then
    mkdir $DIR
fi
cd $DIR
wget http://interactivebrokers.github.io/downloads/twsapi_unixmac_969.02.jar
jar xf twsapi_unixmac_969.02.jar
cp ./IBJts/source/PosixClient/shared/* .
cp ./IBJts/source/PosixClient/src/* .
sed -i "" -e '4i#define IB_USE_STD_STRING' EClientSocketBase.cpp
sed -i "" -e '4i#define IB_USE_STD_STRING' EPosixClientSocket.cpp
sed -i "" -e 's_// LINUX_#include <unistd.h>_g' EPosixClientSocketPlatform.h
cd $PDIR
node-gyp rebuild
