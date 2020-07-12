#!/bin/bash

while true
do

echo
echo RESTAURANTE EL GRAN BOCADO
echo
echo "1) Mostrar el código del programa"
echo "2) Compilar el Programa"
echo "3) Ejecutar el Programa"
echo "4) Salir"
echo

echo Por favor, introduzca una opcion:
read opcion



case $opcion in

1) 
echo Se ha seleccionado la opción 1
echo Mostrando código del programa
echo
cat PIntermedia.c
;;



2) 
echo Se ha seleccionado la opcion 2
echo
gcc -o Ejecutable PIntermedia.c
echo Compilación Terminada
;;





3) 
echo Se ha seleccionado la opcion 3
echo
#COMPROBACIÓN DE LA EXISTENCIA DEL EJECUTABLE
echo COMPROBANDO LA EXISTENCIA DEL EJECUTABLE:
echo
ls -l|grep Ejecutable
echo
Ejecutable=./Ejecutable
if test -f "$Ejecutable"
then
	echo "El ejecutable de la aplicación existe"

else
	echo "El ejecutable de la aplicación no existe, por favor, ejecute la opción 2 primero"
	exit
fi
echo
echo
echo


#VERIFICACION DE PERMISOS DE EJECUCION
echo COMPROBANDO LOS PERMISOS DE EJECUCIÓN:
echo
ls -l|grep Ejecutable
echo
echo

Ejecutable=./Ejecutable
if test -x "$Ejecutable"
then
	echo El Ejecutable ya tiene Permisos de Ejecución
else
	echo El Ejecutable no tiene Permisos de Ejecución, ¿Quiere dárselos?[S/N]:
read permisos

	if test $permisos = "S"
	then 
		chmod 777 Ejecutable
		ls -l|grep Ejecutable|grep ^-..x..x..x
	else
		exit
	fi 
echo
echo operación completada
fi

echo
echo
echo

#SE PIDE EL NUMERO DE PINCHES
echo Por favor, intrduzca el número de pinches que desea en la cocina de su restaurante:
read pinches
echo
clear
#SE EJECUTA EL PROGRAMA

echo Ejecutando programa...
echo
sleep 2
./Ejecutable $pinches
exit

#se hace un gep y un if test y si no los tiene se los da 
#COMPLETE
;;






4) 
echo se ha seleccionado la opcion 4
echo Saliendo
echo
exit;;
*) 
echo Se ha introducido una opcion incorrecta
echo



esac
done
