#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "funciones.h"
#define ARRAY_SIZE 20

int main(void) {
	setbuf(stdout,NULL);
	Struct arrayStruct[ARRAY_SIZE];
	char structName[NAME_SIZE];
	int menuOption;
	int fieldsQty;
	int entityFlag=0;
	int fieldsFlag=0;

	structInit(arrayStruct, ARRAY_SIZE);
	do
	{
		utn_getMenu("\nElija una opcion:\n1)Ingresar el nombre de la entidad\n2)Ingresar el tipo y nombre de cada campo\n3)Imprimir\n5)Salir\n>OPCION:", &menuOption, 2, 1, 5);
		switch(menuOption)
		{
		case 1:
			if(!setStructName(structName, &fieldsQty))
			{
				printf("\nEntidad creada correctamente\n");
				entityFlag=1;
			}
		break;
		case 2:
			if(entityFlag==1 && !setFieldTypeAndName(arrayStruct, fieldsQty))
			{
				printf("\nCampos ingresados correctamente");
				fieldsFlag=1;
			}
			else
			{
				printf("\nPrimero ingrese el nombre de la entidad");
			}
		break;
		case 3:
			if(fieldsFlag==1 && !printAll(structName, arrayStruct, fieldsQty))
			{
				printf("\nFin de la impresion");
			}
			else
			{
				printf("\nPrimero ingrese el nombre de la entidad y sus respectivos campos");
			}
		break;
		}
	}while(menuOption!=5);
	return EXIT_SUCCESS;
}
