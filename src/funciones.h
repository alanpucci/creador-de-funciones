#ifndef FUNCIONES_H_
#define FUNCIONES_H_
#define STRUCTNAME_SIZE 51
#define FIELDNAME_SIZE 30
#define FIELDTYPE_SIZE 10
#define NAME_SIZE 51
#define FIELD_SIZE 7

typedef struct{
	char fieldName[FIELDNAME_SIZE];
	char fieldType[FIELDTYPE_SIZE];
	int isEmpty;
}Struct;

int setStructName(char *name, int *fieldsQty);
int setFieldName(char *name);
int setFieldTypeAndName(Struct *list, int fieldsQty);
int printAll(char *structName, Struct *list, int len);
int structInit(Struct *list, int len);
int searchForEmpty(Struct *list, int len, int *index);
int createGetters(Struct *list, int len, char *structName);
int createSetters(Struct *list, int len, char *structName);
int createNewWithParameters(Struct *list, char *structName, int fieldsQty);
void createIsValid(void);

#endif
