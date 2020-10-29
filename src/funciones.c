#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utn.h"
#include "menu.h"
#include "funciones.h"

static int capitalizeStrings(char *string);
static int lowerString(char *string);

int setStructName(char *name, int *fieldsQty)
{
	int retornar = -1;
	int bufferInt;
	if(name!=NULL && fieldsQty!=NULL)
	{
		if(!utn_getString("Ingrese el nombre de la estructura: ", "\nERROR! Ingrese un nombre valido: ", name, 2, NAME_SIZE) &&
		   !utn_getInt("\nCuantos campos tiene la entidad?: ", "\nERROR! Ingrese un numero valido: ", &bufferInt, 2, 1, 10))
		{
			capitalizeStrings(name);
			*fieldsQty = bufferInt;
			retornar = 0;
			printf("\nNombre para la entidad: %s y tiene %d campos", name, bufferInt);
		}
	}
	return retornar;
}

int setFieldTypeAndName(Struct *list, int fieldsQty)
{
	int retornar=-1;
	int menuOption;
	if(list!=NULL && fieldsQty>0)
	{
		for(int i=0; i<fieldsQty; i++)
		{
			if(!utn_getMenu("\nElija el tipo del campo:\n1)Int\n2)Float\n3)Char*\n>OPCION: ", &menuOption, 2, 1, 4))
			{
				switch(menuOption)
				{
				case 1:
					if(!setFieldName(list[i].fieldName))
					{
						strncpy(list[i].fieldType, "int", FIELDTYPE_SIZE);
					}
				break;
				case 2:
					if(!setFieldName(list[i].fieldName))
					{
						strncpy(list[i].fieldType, "float", FIELDTYPE_SIZE);
					}
				break;
				case 3:
					if(!setFieldName(list[i].fieldName))
					{
						strncpy(list[i].fieldType, "char*", FIELDTYPE_SIZE);
					}
				break;
				}
			}
		}
		retornar=0;
	}
	return retornar;
}

int setFieldName(char *name)
{
	int retornar=-1;
	char fieldName[NAME_SIZE];
	if(name!=NULL)
	{
		if(!utn_getString("Ingrese el nombre del campo: ", "\nERROR! Ingrese un nombre valido: ", fieldName, 2, NAME_SIZE))
		{
			strncpy(name, fieldName, NAME_SIZE);
			retornar=0;
		}
	}
	return retornar;
}


int createNewWithParameters(Struct *list, char *structName, int fieldsQty)
{
	int retornar=-1;
	char lcStructName[NAME_SIZE];
	if(list!=NULL && structName!=NULL && fieldsQty>0)
	{
		strncpy(lcStructName, structName, NAME_SIZE);
		lowerString(lcStructName);
		printf("\n%s* %s_newWithParameters(", structName, lcStructName);
		for(int i=0; i<fieldsQty; i++)
		{
			if(i==0)
			{
				printf("%s %s", list[i].fieldType, list[i].fieldName);
				i++;
			}
			printf(", %s %s",list[i].fieldType, list[i].fieldName);
		}
		printf(")\n{\n\t%s* this = %s_new();\n\tif(this!=NULL)\n\t{\n\t\tif(",structName, lcStructName);
		for(int i=0; i<fieldsQty; i++)
		{
			if(i==0)
			{
				printf("  %s_set%s(this, %s)!=-1",lcStructName, list[i].fieldName, list[i].fieldName);
				i++;
			}
			printf("\n\t\t  && %s_set%s(this, %s)!=-1",lcStructName, list[i].fieldName, list[i].fieldName);
		}
		printf(")\n\t\t{\n\t\t\treturn this;\n\t\t}\n\t\t%s_delete(this);\n\t\treturn NULL;\n\t}\n}\n",lcStructName);
	}
	return retornar;
}


int createGetters(Struct *list, int len, char *structName)
{
	int retornar=-1;
	char lcStructName[NAME_SIZE];
	char ucFieldName[NAME_SIZE];
	if(list!=NULL && len>0)
	{
		strncpy(lcStructName, structName, NAME_SIZE);
		lcStructName[0] = tolower(lcStructName[0]);
		for(int i=0;i<len;i++)
		{
			strncpy(ucFieldName, list[i].fieldName, FIELDNAME_SIZE);
			capitalizeStrings(ucFieldName);
			if(strncmp(list[i].fieldType, "char*", NAME_SIZE)==0)
			{
				printf("\n%s %s_get%s(%s* this)\n{\n\tif(this!=NULL)\n\t{\n\t\treturn this->%s;\n\t}\n\treturn NULL;\n}\n"
						,list[i].fieldType, lcStructName, ucFieldName, structName, list[i].fieldName);
			}
			else
			{
				printf("\n%s %s_get%s(%s* this)\n{\n\tif(this!=NULL)\n\t{\n\t\treturn this->%s;\n\t}\n\treturn NULL;\n}\n"
						,list[i].fieldType, lcStructName, ucFieldName, structName, list[i].fieldName);
			}
		}
	}
	return retornar;
}

int createSetters(Struct *list, int len, char *structName)
{
	int retornar=-1;
	char lcStructName[NAME_SIZE];
	char ucFieldName[FIELDNAME_SIZE];
	char ucFieldType[FIELDTYPE_SIZE];
	if(list!=NULL && len>0)
	{
		strncpy(lcStructName, structName, NAME_SIZE);
		lcStructName[0] = tolower(lcStructName[0]);
		for(int i=0;i<len;i++)
		{
			strncpy(ucFieldName, list[i].fieldName, FIELDNAME_SIZE);
			strncpy(ucFieldType, list[i].fieldType, FIELDTYPE_SIZE);
			capitalizeStrings(ucFieldName);
			capitalizeStrings(ucFieldType);
			if(strncmp(list[i].fieldType, "char*", NAME_SIZE)==0)
			{
				printf("\nint %s_set%s(%s* this, %s %s)\n{\n\tint retorno=-1;\n\tif(this!=NULL && isValidString(%s)==1)\n\t{\n\t\tthis->%s = %s;\n\t\tretorno=0;\n\t}\n\treturn retorno;\n}\n",
						lcStructName, ucFieldName, structName, list[i].fieldType, list[i].fieldName,
						list[i].fieldName, list[i].fieldName, list[i].fieldName);
			}
			else
			{
				printf("\nint %s_set%s(%s* this, %s %s)\n{\n\tint retorno=-1;\n\tif(this!=NULL && isValid%s(%s)==1)\n\t{\n\t\tthis->%s = %s;\n\t\tretorno=0;\n\t}\n\treturn retorno;\n}\n",
						lcStructName, ucFieldName, structName, list[i].fieldType, list[i].fieldName,
						ucFieldType, list[i].fieldName, list[i].fieldName, list[i].fieldName);
			}
		}
		retornar=0;
	}
	return retornar;
}

void createIsValid(void)
{
	printf("\nint isValidString(char* string)\n{\n\tint retorno=1;\n\tif(string!=NULL && strlen(string)>0)\n\t{\n\t\tfor(int i=0;i<strlen(nombre);i++)"
			"\n\t\t{\n\t\t\tif((string[i]<'a' || string[i]>'z') && (string[i]<'A' || string[i]>'Z') && string[i]!=32)\n"
			"\t\t\t{\n\t\t\t\tretorno=0;\n\t\t\t\tbreak;\n\t\t\t}\n\t\t}\n\t}\n\treturn retorno;\n}\n");
	printf("\nint isValidFloat(char string[])\n{\n\tint retorno = 1;\n\tint i = 0;\n\tint commaCounter=0;\n\n\tif(string!=NULL && strlen(string)>0)"
			"\n\t\tif(string[0]=='-')\n\t\t{\n\t\t\ti=1;\n\t\t}\n\t\tfor(;i<strlen(string);i++)\n\t\t{\n\t\t\tif(string[i]=='.')\n\t\t\t{\n\t\t\t\tcommaCounter++;"
			"\n\t\t\t}\n\t\t\tif(commaCounter>1 || ((string[i]>'9' || string[i]<'0') && string[i]!='.'))\n\t\t\t{\n\t\t\t\tretorno=0;\n\t\t\t\tbreak;\n\t\t\t}\n\t\t}"
			"\n\t}\n\treturn retorno;\n}\n");
	printf("\nint isValidInt(char string[])\n{\n\tint retorno = 1;\n\tint i = 0;\n\n\tif(string!=NULL && strlen(string)>0)"
			"\n\t\tif(string[0]=='-')\n\t\t{\n\t\t\ti=1;\n\t\t}\n\t\tfor(;i<strlen(string);i++)\n\t\t{"
			"\n\t\t\tif(string[i]>'9' || string[i]<'0')\n\t\t\t{\n\t\t\t\tretorno=0;\n\t\t\t\tbreak;\n\t\t\t}\n\t\t}"
			"\n\t}\n\treturn retorno;\n}\n");
}

int printAll(char *structName, Struct *list, int len)
{
	int retornar=-1;
	char structNameLowercase[NAME_SIZE];
	if(structName!=NULL && list!=NULL && len>0)
	{
		strncpy(structNameLowercase, structName, NAME_SIZE);
		structNameLowercase[0] = tolower(structName[0]);
		//struct
		printf("\ntypedef struct{");
		for(int i=0;i<len;i++)
		{
			if(strncmp(list[i].fieldType, "char*", NAME_SIZE)==0)
			{
				printf("\n\t%s %s[];",list[i].fieldType, list[i].fieldName);
			}
			else
			{
				printf("\n\t%s %s;",list[i].fieldType, list[i].fieldName);
			}
		}
		printf("\n}%s;\n",structName);
		//new
		printf("\n%s* %s_new()\n{\n\treturn (%s*)malloc(sizeof(%s));\n}\n",structName, structNameLowercase, structName, structName);
		//newWithParameters
		createNewWithParameters(list, structName, len);
		//delete
		printf("\nvoid %s_delete(%s* this)\n{\n\tfree(this);\n}\n",structNameLowercase, structName);
		//getters
		createGetters(list, len, structName);
		//setters
		createSetters(list, len, structName);
		//isValid
		createIsValid();
		retornar=0;
	}
	return retornar;
}

int structInit(Struct *list, int len)
{
	int retornar=-1;
	if(list!=NULL)
	{
		for(int i=0;i<len;i++)
		{
			list[i].isEmpty = 1;
		}
		retornar=0;
	}
	return retornar;
}

int searchForEmpty(Struct *list, int len, int *index)
{
	int retornar=-1;
	if(list!=NULL)
	{
		for(int i=0;i<len;i++)
		{
			if(list[i].isEmpty == 1)
			{
				*index = i;
				retornar = 0;
				break;
			}
		}
	}
	return retornar;
}

static int capitalizeStrings(char *string)
{
	int retornar = -1;
	char bufferName[NAME_SIZE];
	strncpy(bufferName, string, NAME_SIZE);
	bufferName[0] = toupper(bufferName[0]);
	for(int i=1;i<NAME_SIZE;i++)
	{
		bufferName[i] = tolower(bufferName[i]);
	}
	strncpy(string, bufferName, NAME_SIZE);
	retornar = 0;
	return retornar;
}

static int lowerString(char *string)
{
	int retornar =-1;
	char bufferString[NAME_SIZE];
	strncpy(bufferString, string, NAME_SIZE);
	bufferString[0] = tolower(bufferString[0]);
	strncpy(string, bufferString, NAME_SIZE);
	retornar=0;
	return retornar;
}
