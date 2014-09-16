#!/bin/sh


if [ -f setEnv.sh ];
then
	rm setEnv.sh;
fi
mkdir lib
echo "#!/bin/sh\n" >> setEnv.sh
echo "export gossip="$(pwd) >> setEnv.sh
echo "export LD_LIBRARY_PATH=\$gossip/lib/:\$LD_LIBRARY_PATH" >> setEnv.sh
echo "export PATH=\$gossip:\$PATH" >> setEnv.sh

. ./setEnv.sh

make
