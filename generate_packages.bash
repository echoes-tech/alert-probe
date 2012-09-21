#!/bin/bash

for i in {1..20}
do 
	sed -i -e "s/\(probe_id=\).*/\1$i/" conf/probe.conf

	echo "Création du paquet deb pour Debian 32 bits"
	bash nbproject/Package-Deb_32bits_Release.bash
	echo -e "\nCréation du paquet deb pour Debian 64 bits"
	bash nbproject/Package-Deb_64bits_Release.bash
	echo -e "\nCréation du paquet deb pour Debian Squeeze 32 bits"
	bash nbproject/Package-Debian6_32bits_Release.bash
	echo -e "\nCréation du paquet deb pour Debian Squeeze 64 bits"
	bash nbproject/Package-Debian6_64bits_Release.bash
	echo -e "\nCréation du paquet deb pour Redhat 32 bits"
	bash nbproject/Package-RPM_32bits_Release.bash
	echo -e "\nCréation du paquet deb pour Redhat 64 bits"
	bash nbproject/Package-RPM_64bits_Release.bash

	echo -e "\nCréation du dossier $i"
        if [ -d $i ]
	then
		rm -rf $i
	fi
	mkdir $i
	echo -e "\nCopie de tous les paquets dans $i"
	cp dist/Deb_32bits_Release/GNU-Linux-x86/package/echoes-alert-probe_0.1.0.alpha-1_i386.deb $i
	cp dist/Deb_64bits_Release/GNU-Linux-x86/package/echoes-alert-probe_0.1.0.alpha-1_amd64.deb $i
	cp dist/Debian6_32bits_Release/GNU-Linux-x86/package/echoes-alert-probe_0.1.0.alpha-1_squeeze_i386.deb $i
	cp dist/Debian6_64bits_Release/GNU-Linux-x86/package/echoes-alert-probe_0.1.0.alpha-1_squeeze_amd64.deb $i
	cp dist/RPM_32bits_Release/GNU-Linux-x86/package/ea-probe-0.1.0.alpha-1.i386.rpm $i
	cp dist/RPM_64bits_Release/GNU-Linux-x86/package/ea-probe-0.1.0.alpha-1.x86_64.rpm $i
done
