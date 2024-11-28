echo "["`date +%FT%H:%M:%S.%N`"] starting build..."

CURRDIR=`pwd`
BUILD_TYPE=RelWithDebInfo

if [ $# -eq 0 ]
  then
    echo "["`date +%FT%H:%M:%S.%N`"] No arguments supplied - Release"
fi

echo "["`date +%FT%H:%M:%S.%N`"] creating build dir..."
mkdir -pv build
cd build

echo "["`date +%FT%H:%M:%S.%N`"] running cmake..."
cmake .. -DCMAKE_BUILD_TYPE=$BUILD_TYPE
RETVAL=$?
if [ "$RETVAL" != 0 ]
  then
    echo "["`date +%FT%H:%M:%S.%N`"] cmake failed"
    exit 1
fi

echo "["`date +%FT%H:%M:%S.%N`"] running make..."
make
RETVAL=$?
if [ "$RETVAL" != 0 ]
  then
    echo "["`date +%FT%H:%M:%S.%N`"] make failed"
    exit 2
fi

echo "["`date +%FT%H:%M:%S.%N`"] running test..."
./MultiTypedQueue

echo "["`date +%FT%H:%M:%S.%N`"] cleaning up..."
cd $CURRDIR
rm -fvR build/
RETVAL=$?
if [ "$RETVAL" != 0 ]
  then
    echo "["`date +%FT%H:%M:%S.%N`"] cleanup failed"
    exit 3
fi

echo "["`date +%FT%H:%M:%S.%N`"] DONE!"
exit 0
