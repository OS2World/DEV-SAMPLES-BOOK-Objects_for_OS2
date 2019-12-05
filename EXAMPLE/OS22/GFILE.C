
#define GenericFile_Class_Source
#include <gfile.ih>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 *Method from the IDL attribute statement:
 *"attribute string path"
 */

SOM_Scope void  SOMLINK _set_path(GenericFile somSelf,  Environment *ev,
                                  string path)
{
    GenericFileData *somThis = GenericFileGetData(somSelf);
    GenericFileMethodDebug("GenericFile","_set_path");
    if(_path)
        free(_path);
    _path = (string)malloc(strlen(path)+1);
    strcpy(_path, path);
}

/*
 *Method from the IDL attribute statement:
 *"attribute string name"
 */

SOM_Scope void  SOMLINK _set_name(GenericFile somSelf,  Environment *ev,
                                  string name)
{
    GenericFileData *somThis = GenericFileGetData(somSelf);
    GenericFileMethodDebug("GenericFile","_set_name");
    if(_name)
        free(_name);
    _name = (string)malloc(strlen(name)+1);
    strcpy(_name, name);

}

SOM_Scope void  SOMLINK somInit(GenericFile somSelf)
{
    GenericFileData *somThis = GenericFileGetData(somSelf);
    GenericFileMethodDebug("GenericFile","somInit");
    _name = NULL;
    _path = NULL;
    GenericFile_parent_SOMObject_somInit(somSelf);
}

SOM_Scope void  SOMLINK somUninit(GenericFile somSelf)
{
    GenericFileData *somThis = GenericFileGetData(somSelf);
    GenericFileMethodDebug("GenericFile","somUninit");
    if(_name)
        free(_name);
    if(_path)
        free(_path);

    GenericFile_parent_SOMObject_somUninit(somSelf);
}
