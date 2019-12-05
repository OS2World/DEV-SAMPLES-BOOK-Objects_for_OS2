// A simple interpreter for SOM object calls. 
// icc interp.cpp som.lib

#include <somcls.xh>
#include <somcm.xh>
#include <stdio.h>
#include <stdarg.h>

/*  REGISTERS  */
SOMObject *   o[10]; /* objects */
somId         i[10]; /* somIds */
long          l[10]; /* integers */
char *        s[10]; /* strings */
boolean       b[10]; /* booleans */
struct {char bank; int  num;} result, reg;
#define get_reg(r) { r.bank = *(cmd_ptr); sscanf(cmd_ptr+1,"%d",&(r.num)); }

/*  COMMANDS  -- see Example Use, at end.  */
char *cmd_ptr, cmd[100];
va_list args;
int cmd_is_call;
void assign_literal(), call_method();
int get_cmd(), push_args();
#define get_token(s1,s2) cmd_ptr = strtok(s1,s2)

/* INTERPRETER */
main()
{
    o[0] = somEnvironmentNew(); args = (va_list) SOMMalloc(1000);
    somPrintf("-- o0 = "); o[0]->somPrintSelf();
    while (get_cmd())
	if (cmd_is_call) call_method();
	else assign_literal();
}

int get_cmd()
{
    somPrintf(":  "); fgets(cmd,100,stdin);
    cmd_is_call = (int) strchr(cmd,'(');
    get_token(cmd," "); get_reg(result); /* determine result register */
    get_token(0," ");                    /* ignore the "=" */
    get_token(0," (\n");                 /* rhs of assignment */
    return (int) cmd_ptr;
}

void call_method() /* cmd_ptr points to rhs of a method call assignment */
{
    va_list arg_ptr = args;
    somId mid = somIdFromString(cmd_ptr); get_token(0,",)"); 
    get_reg(reg);
    SOMObject *receiver = o[reg.num];
    va_arg(arg_ptr, SOMObject *) = receiver;
    if (push_args())
	switch (result.bank) {
	  case 'o': receiver->SOMObject_somDispatch((somToken*)&(o[result.num]),mid,args);
	    somPrintf("-- o%d = ",result.num); o[result.num]->somPrintSelf(); break;
	  case 's': receiver->SOMObject_somDispatch((somToken*)&(s[result.num]),mid,args);
	    somPrintf("-- s%d = string <%s>\n",result.num,s[result.num]); break;
	  case 'l': receiver->SOMObject_somDispatch((somToken*)&(l[result.num]),mid,args);
	    somPrintf("-- l%d = long <%d>\n",result.num,l[result.num]); break;
	  case 'b': receiver->SOMObject_somDispatch((somToken*)&(b[result.num]),mid,args);
	    somPrintf("-- b%d = bool <%d>\n",result.num,b[result.num]); break;
	  case 'v': receiver->SOMObject_somDispatch((somToken*)0,mid,args); break;
	  default: somPrintf("method result restricted to o(bject),s(tring),l(ong),b(ool) and v(oid) register banks\n");
	}
}
int push_args()
{
    va_list arg_ptr = args+4;
    get_token(0,",)\n");
    while (cmd_ptr) {
	get_reg(reg);
	switch (reg.bank) {
	  case 'i': va_arg(arg_ptr, char **) = i[reg.num]; break;
	  case 'o': va_arg(arg_ptr, SOMObject *) = o[reg.num]; break;
	  case 's': va_arg(arg_ptr, char *) = s[reg.num]; break;
	  case 'l': va_arg(arg_ptr, long) = l[reg.num]; break;
	  case 'b': va_arg(arg_ptr, boolean) = b[reg.num]; break;
	  default: somPrintf("arguments restricted to i, s, o, l, and b register banks\n"); return 0;
	}
	get_token(0,",)\n");
    }
    return 1;
}
void assign_literal() /* cmd_ptr points to rhs of a literal assignment */
{
    switch (result.bank) {
      case 'i': i[result.num] = somIdFromString(cmd_ptr);
	somPrintf("-- i%d = somId <%s>\n",result.num, somStringFromId(i[result.num])); break;
      case 's': s[result.num] = (char*)SOMMalloc(strlen(cmd_ptr)+1); strcpy(s[result.num],cmd_ptr);
	somPrintf("-- s%d = string <%s>\n",result.num,s[result.num]); break;
      case 'l': sscanf(cmd_ptr,"%d",&(l[result.num]));
	somPrintf("-- l%d = long <%d>\n",result.num, l[result.num]); break;
      default: somPrintf("literal assignment restricted to i,s, and l register banks\n");
    }
}


/********************************
  
Example Use:

%interp
-- o0 = {An instance of class SOMClassMgr at address 20059598}
:  i0 = somFindClass
-- i0 = somId <somFindClass>
:  b1 = somRespondsTo(o0,i0)
-- b1 = boolean <1>
:  i1 = SOMObject
-- i1 = somId <SOMObject>
:  l1 = 0
-- l1 = long <0>
:  o1 = somFindClass(o0,i1,l1,l1)
-- o1 = {An instance of class SOMClass at address 200567A8}
:  s1 = somGetName(o1)
-- s1 = string <SOMObject>
:  l1 = somGetNumMethods(o1)
-- l1 = long <30>

********************************/
