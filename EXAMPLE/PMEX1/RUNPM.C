/*************/
/** runpm.c **/
/*************/

#include <pmapp.h>
void main(int argc, char *argv[])
{
    PMApp obj = PMAppNew();
    _somFree(obj);
}
