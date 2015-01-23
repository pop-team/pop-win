%{
#include <strings.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <ctype.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include <list>
#include <set>

#include "json/json.h"

#include "parser.h"
#include "paroc_utils.h"
#include "debug.h"


 //Declaration in parser.lex:
  
 int PutToken(char *str);
 int PutToken(char chr);
 char *GetToken(int yyval);

 void errormsg(const  char *s);
 void setReturnParam(int pointer, int ref, int const_virtual); // Methode to group code that set C++ specific C++ Param attributes
 void setPOPCMethodeModifier(int settings); // mehtode to group code that set/controlle Methode attributes (sync, conc, ...)
 void errorGlobalMehtode(bool isGlobal);
  
extern "C" {
	int yywrap();
}

void yyerror(const  char *s);
int yylex();

extern int linenumber;
extern char filename[1024];

CArrayCharPtr incl[1000];
CArrayCharPtr sources;
CArrayCharPtr searchpath; 

int indexsource=0;  //the index of source file and the include directive


 extern CArrayChar othercodes;
 extern bool insideClass;
 extern int startPos;


	CodeFile *thisCodeFile;
	Class *currentClass;
	DataType *currenttype;
	DataType *returntype;
 
	std::vector<bool> constPointerPositions; // counter variables who contains order of conts possition in pointers

	TypeClassStruct *seqclass;

	Param *currentparam;

	PackObject *currentPack;
	Structure *structContainer;

	TypeDefinition *typeDefContainer;
	Method *method; // a method
	// Param *param;

 int n,t;
 bool isNamespace = false;
 
 bool isInStruct = false;
 
 bool isParclassDeclared = false;
 
 bool hadParclass = false; 
 
 bool isWarningEnable;
 bool isImplicitPackEnable;
 bool isPOPCPPCompilation; 
 bool isAsyncAllocationDisable;
 char holdnamespace[500];
 char tmp[10240]; /* what is the content of tmp */
 char typetmp[100];
 char tmpProc[10240];
 char tmpSize[10240];
 int parsingclass=0;

 void UpdateMarshalParam(int flags, Param *t);

 int ParseFile(char *infile, char *outfile, bool client, bool broker, bool isWarningEnable, bool isImplicitPackEnable, bool isPOPCPPCompilation, bool isAsyncAllocationDisable);
 char *CheckAndCreateDir(char *fname,char *name);
 bool CheckAndInsert(CArrayCharPtr &source, CArrayCharPtr &searchpath, char *fname);

 int AfterParsing(int status, char *classname, char *stubproc, bool gen_stub);


 paroc_list<TypeClassStruct *>  typestack;
 TypeClassStruct *currentstruct;
 void CleanStack();
 void Push(TypeClassStruct *x);
 TypeClassStruct *Pop();
 TypeClassStruct *Peek();

 int counter=0;
 int errorcode=0;

 AccessType  accessmodifier=PUBLIC;


struct TemplateArgument
{
  DataType *type;
  bool isRef;
};



#define YYMAXDEPTH 100000
#define YYINITDEPTH 2000
%}

%token PARCLASS_KEYWORD CLASS_KEYWORD CLASSID ID ID1 INTEGER REAL STRING PUBLIC_KEYWORD PROTECTED_KEYWORD PRIVATE_KEYWORD VIRTUAL_KEYWORD CONST_KEYWORD STRUCT_KEYWORD TYPEDEF_KEYWORD
%token SYNC_INVOKE ASYNC_INVOKE INPUT OUTPUT  CONCURRENT SEQUENTIAL MUTEX HIDDEN PROC SIZE THIS_KEYWORD
%token INCLUDE DIRECTIVE OD AUTO_KEYWORD REGISTER_KEYWORD VOLATILE_KEYWORD PACK_KEYWORD 
%token AND_OP OR_OP EQUAL_OP NOTEQUAL_OP GREATEREQUAL_OP LESSEQUAL_OP NONSTRICT_OD_OP EOFCODE
%token SCOPE ENUM CLASS NAMESPACE STATIC_KEYWORD

%left '+' '-' '*' '/' '%'
%left '&' '|' '^' '~' '!' '='
%left AND_OP OR_OP EQUAL_OP NOTEQUAL_OP GREATEREQUAL_OP LESSEQUAL_OP '>' '<' 
%left ':' '?' 
%left UMINUS /*supplies precedence for unary minus */
%nonassoc '('

%error-verbose  /* right now vor parser depuging pourpose, gives more detailes at syntax error */

%%


startlist: handle_this startlist
|/*empty*/
{
	if (othercodes.GetSize()) {
		assert(thisCodeFile!=NULL);
		OtherCode *dat=new OtherCode(thisCodeFile);
		dat->AddCode(othercodes);
		thisCodeFile->AddCodeData(dat);
		othercodes.SetSize(0);
	}
}

| namespace_declaration 
| class_declaration  
{ 
	insideClass=false; 
	othercodes.SetSize(0); 
	startPos=-1;
} startlist 
| class_prototype startlist
| DIRECTIVE  startlist
| pack_directive startlist
| type_definition startlist
| not_care_code startlist
{
	CleanStack(); 
}
| handle_eof
;

handle_eof: EOFCODE
{
	


	if(isImplicitPackEnable){     
	 
		/** 
		 * The following code handles the implicit @pack directive if it's not specified by the developer 
		 * Algo for implicit pack
		 * 1. Look for all included .ph file in the current file
		 * 2. For all .ph file find the parclass(s) included in it
		 * 3. Match the found parclass(s) with the code in the current file (ParClassName::). If match, put in pack directive
		 */
		 
		
		/** 
		 * Perform this task only on file with implementation (.C, .cc, .cpp, .cp, .cxx, .c++, .CPP)
		 * Find the extension of the file in the current C++ extension 
		 */
		std::string fname(filename);
				
		std::size_t cc = fname.find(".cc");
		std::size_t cpp = fname.find(".cpp");
		std::size_t cp = fname.find(".cp");		
		std::size_t cxx = fname.find(".cxx");	
		std::size_t cplusplus = fname.find(".c++");	
		std::size_t CPP = fname.find(".CPP");	
		std::size_t C = fname.find(".C");	
			
		if((cc != std::string::npos || cpp != std::string::npos || cxx != std::string::npos || cp != std::string::npos || cplusplus != std::string::npos 
			|| CPP != std::string::npos || C != std::string::npos) && !isParclassDeclared){	
				
			/**
			 * Find all .ph files included in the current file
			 */
			 
			// Create a list to store all .ph files' name
			std::list<std::string> ph_files; 
			 
			std::fstream cc_real_file; 
			cc_real_file.open(fname.c_str(), std::ios_base::out | std::ios_base::in);

			if (cc_real_file.is_open()){
				// Find name of the parclass
				char line[512]; 
				bool notEOF = true;
				while (notEOF){
					cc_real_file.getline(line, 512);
					if(cc_real_file.eof()){
						notEOF = false;
						cc_real_file.close();
					} else {
						// Get the next line
						std::string str_line(line);
						// Find a .ph include in the current file
						std::size_t ph_extension_pos = str_line.find(".ph"); 
						if(ph_extension_pos != std::string::npos){
							std::size_t ph_file_name_start = str_line.rfind('"', ph_extension_pos);
							std::string ph_file_name = str_line.substr(ph_file_name_start+1, (ph_extension_pos-ph_file_name_start)+2);
							// Save the .ph file found
							ph_files.push_back(ph_file_name);
						}
					}
				}
			}	
			
			
			if(!ph_files.empty()){
				/**
				 * Find all the parclass declared in the .ph files found before. 
				 */
				std::list<std::string> parclass_names;
				std::list<std::string>::iterator it;
				for(it=ph_files.begin(); it != ph_files.end(); it++) {
					std::fstream ph_real_file;			
					ph_real_file.open((*it).c_str(), std::ios_base::out | std::ios_base::in);				
					if (ph_real_file.is_open()){
						// Find name of the parclass
						char line[512]; 
						bool notEOF = true;
						while (notEOF){
							ph_real_file.getline(line, 512);
							if(ph_real_file.eof()){
								notEOF = false;
								ph_real_file.close();
							} else {
								std::string str_line(line);
								std::size_t parclass_pos = str_line.find("parclass"); 
								if(parclass_pos != std::string::npos){
									std::size_t parclass_name_start = str_line.find(" ", parclass_pos); 
									std::size_t parclass_name_stop = str_line.find(" ", parclass_name_start+1);
									if(parclass_name_stop == std::string::npos){
										parclass_name_stop = str_line.find(";", parclass_name_start+1); 
									}
									if(parclass_name_stop == std::string::npos){
										parclass_name_stop = str_line.find("{", parclass_name_start+1); 																	
									}
									if(parclass_name_stop == std::string::npos){			
										parclass_name_stop = str_line.find("\n", parclass_name_start+1); 		
									}
									if(parclass_name_stop == std::string::npos){			
										parclass_name_stop = str_line.find("\r", parclass_name_start+1); 		
									}
									if(parclass_name_stop == std::string::npos){			
										parclass_name_stop = str_line.find("\r\n", parclass_name_start+1); 						
									}
									if(parclass_name_stop == std::string::npos){			
										parclass_name_stop = str_line.size();
									}
									if(parclass_name_stop != std::string::npos && parclass_name_start != std::string::npos){
										std::string parclass_name = str_line.substr(parclass_name_start, (parclass_name_stop - parclass_name_start));
										// Removed possible withespaces from the parclass name
										for(int i=0; i < parclass_name.length(); i++){
											if(parclass_name[i] == ' '){
												parclass_name.erase(i, 1);
												i--;
											}
										}
										// Save the parclass's name found
										parclass_names.push_back(parclass_name);
									}
								}
							}	
						}			
					}
				}	
				/**
				 * Find implementation of the found parclass in the current file
				 */
				std::set<std::string> matched_parclasses;			
				cc_real_file.open(fname.c_str(), std::ios_base::out | std::ios_base::in);
				if (cc_real_file.is_open()){
					// Find name of the parclass
					char line[512]; 
					bool notEOF = true;
					while (notEOF){
						cc_real_file.getline(line, 512);
						if(cc_real_file.eof()){
							notEOF = false;
							cc_real_file.close();
						} else {
							// Get the next line
							std::string str_line(line);
							std::list<std::string>::iterator it;
							for(it=parclass_names.begin(); it != parclass_names.end(); it++) {
								std::string prefix = (*it);
								prefix.append("::");
								std::size_t pos = str_line.find(prefix);
								if(pos != std::string::npos){
									// Saved the matched parclass in a set
									matched_parclasses.insert((*it));
								}
							}
						}
					}
				}	
			
				if(!matched_parclasses.empty()){
					// Implict add the pack directive
					if (othercodes.GetSize() && startPos>0) {
						assert(thisCodeFile!=NULL);
						OtherCode *dat=new OtherCode(thisCodeFile);
						dat->AddCode((char *)othercodes,startPos);
						thisCodeFile->AddCodeData(dat);
						othercodes.SetSize(0);
					}  
					startPos=-1;
					currentPack=new PackObject(thisCodeFile);
					currentPack->SetStartLine(linenumber-1);
					thisCodeFile->AddCodeData(currentPack); 
					std::set<std::string>::iterator it;
					for(it=matched_parclasses.begin() ; it != matched_parclasses.end(); it++ ){
						if (currentPack!=NULL) {

							char * objname = new char[(*it).size() + 1];
							std::copy((*it).begin(), (*it).end(), objname);
							objname[(*it).size()] = '\0';
							currentPack->AddObject(objname);
							// Ok file exists, so we will find the parclass name and add the pack directive with it
							if(isWarningEnable){
								std::cout << filename << ":" << linenumber << ": Warning: No @pack directive for class: " << objname;
								std::cout << ", @pack(" << objname << ") is assumed..." << std::endl;
							}							
							delete [] objname;
						}
					}
					isParclassDeclared = true; 
					currentPack->SetEndLine(linenumber-1);
					currentPack=NULL;
					othercodes.SetSize(0);
					startPos=-1;
				}
			}
		}
	}	
}

handle_this: THIS_KEYWORD
{
	othercodes.InsertAt(-1,"\n",strlen("\n"));
}
;


not_care_code: error ';' 
{
  startPos=-1;
  insideClass=false;
  if (othercodes.GetSize())
    {
      assert(thisCodeFile!=NULL);
      OtherCode *dat=new OtherCode(thisCodeFile);
      dat->AddCode((char *)othercodes,othercodes.GetSize());
      thisCodeFile->AddCodeData(dat);
      othercodes.SetSize(0);
    }  
}
| error '}'
{
  startPos=-1;
  insideClass=false;
  if (othercodes.GetSize())
    {
      assert(thisCodeFile!=NULL);
      OtherCode *dat=new OtherCode(thisCodeFile);
      dat->AddCode((char *)othercodes,othercodes.GetSize());
      thisCodeFile->AddCodeData(dat);
      othercodes.SetSize(0);
    }  
}
| error EOFCODE
{
  startPos=-1;
  insideClass=false;
  if (othercodes.GetSize())
    {
      assert(thisCodeFile!=NULL);
      OtherCode *dat=new OtherCode(thisCodeFile);
      dat->AddCode((char *)othercodes,othercodes.GetSize());
      thisCodeFile->AddCodeData(dat);
      othercodes.SetSize(0);
    } 
  YYACCEPT;
}
;

pack_directive: pack_header '(' object_list ')'
{
	
	isParclassDeclared = true; 
	currentPack->SetEndLine(linenumber-1);
  	currentPack=NULL;
  	othercodes.SetSize(0);
  	startPos=-1;
};

pack_header: PACK_KEYWORD
{
  	if (othercodes.GetSize() && startPos>0) {
      assert(thisCodeFile!=NULL);
      OtherCode *dat=new OtherCode(thisCodeFile);
      dat->AddCode((char *)othercodes,startPos);
      thisCodeFile->AddCodeData(dat);
      othercodes.SetSize(0);
	}  
	startPos=-1;
	currentPack=new PackObject(thisCodeFile);
	currentPack->SetStartLine(linenumber-1);
	thisCodeFile->AddCodeData(currentPack);  
};



object_list: ID rest_object_list
{
  		if (currentPack!=NULL) {
			currentPack->AddObject(GetToken($1));
		}
}
;



namespace_declaration: NAMESPACE ID 

{
	
	// Avoid handling of standard namespace used in POP-C++
	
	if(strcmp("__gnu_cxx", GetToken($2)) != 0 &&  strcmp("std", GetToken($2)) != 0 && strcmp("__gnu_debug", GetToken($2)) != 0 && strcmp("rel_ops", GetToken($2)) != 0  && strcmp("__debug", GetToken($2)) != 0){
			
		isNamespace = true;
			
		sprintf(holdnamespace, "%s", GetToken($2));
	
	}

}
'{' startlist '}'

{
	
	isNamespace = false;

};



rest_object_list: /*empty */
{
}
| ',' object_list
{
};

/*
NEW TYPE DEFINITIONS
*/

type_definition: struct_definition
{
    CleanStack();
}
| typedef_definition
| seqclass_definition

/**
 
 * Inner Class delcaration
 
 * Catch inner class definition to raise an error message and abort the compilation
 
 */
 

innerclass_definition: CLASS_KEYWORD
{
	
	sprintf(tmp,"Class declaration inside parclass are not currently supported in POP-C++ !\n");
	
	errormsg(tmp);
	exit(1);

} 



innerclass_static: STATIC_KEYWORD {
	sprintf(tmp,"Use of static member inside parclass is not currently supported in POP-C++!\n");
	
	errormsg(tmp);
	exit(1);

}
/* 
STRUCT TYPES...
*/



struct_definition: struct_head '{' struct_body '}'
{
  	currentstruct=Pop();
  	if (currentstruct!=NULL) currentstruct->SetTypeClass(false);
  	$$=$1;
  	structContainer = NULL;
  	isInStruct = false;
}
| struct_head not_care_code
{
  currentstruct=Pop();
  $$=$1;
}

struct_head: STRUCT_KEYWORD ID
{
	
	if(currentClass!=NULL){
		
		isInStruct = true;
		
		structContainer = new Structure(currentClass, accessmodifier);
		
		structContainer->SetLineInfo(linenumber-1);
		
		currentClass->AddMember(structContainer);
      
		structContainer->setName(GetToken($2));	
	
	}
  	char *tname=GetToken($2);
	DataType *type=thisCodeFile->FindDataType(tname);
  	TypeClassStruct *t;
  	if (type!=NULL){
		t=dynamic_cast<TypeClassStruct *>(type);
		if (t==NULL) {
	  //	  	  thisCodeFile->RemoveDataType(type);
	  //	  	  delete type;
	  t=new TypeClassStruct(tname, false);
	  thisCodeFile->AddDataType(t);
	  //	  sprintf(tmp,"data type \"%s\" has been redefined!\n",tname);
	  //	  errormsg(tmp);
	  //	  exit(1);
		}
	} else { 
      t=new TypeClassStruct(tname,false);
      thisCodeFile->AddDataType(t);	
   }
	Push(t);
	$$=$2;
}
| STRUCT_KEYWORD
{
  TypeClassStruct *t=new TypeClassStruct(NULL,false);
  thisCodeFile->AddDataType(t);
  Push(t);
  $$=-1;
}
;

struct_body: /*empty*/
| struct_element ';' struct_body
;

struct_element: decl_specifier struct_elname_list 
;

struct_elname_list: pointer_specifier ID array_declarator struct_elname_other
{
	// Save the attribute in the Strcuture object to be able to produce code
	if(structContainer!=NULL) {
		std::string attribute;
		attribute.append(typetmp);
		attribute.append(" ");
		attribute.append(GetToken($2));
		attribute.append(";");	
		structContainer->setInnerDecl(attribute);			
	}

	DataType *type1=currenttype;
	if ($1>0) {
		//type1=new TypePtr(NULL,$1, type1);
      type1=new TypePtr(NULL, $1, type1, constPointerPositions);
      thisCodeFile->AddDataType(type1);
   }
  	if ($3!=-1){
      type1=new TypeArray(NULL,GetToken($3), type1);
      thisCodeFile->AddDataType(type1);     
   }  
	TypeClassStruct *t=Peek();
	assert(t!=NULL);
	t->Add(type1, GetToken($2));
}
;

struct_elname_other: /*empty*/
| ',' struct_elname_list
;



/*
TYPEDEF TYPES
*/

typedef_definition: TYPEDEF_KEYWORD ID pointer_specifier ID array_declarator
{
	if(currentClass!=NULL){
			
		typeDefContainer = new TypeDefinition(currentClass, accessmodifier);
		
		typeDefContainer->SetLineInfo(linenumber-1);
		
		currentClass->AddMember(typeDefContainer);
      
		typeDefContainer->setName(GetToken($2));	
	
		typeDefContainer->setBaseType(GetToken($4));	
	
		if ($3>0) {
			typeDefContainer->setAsPtr();	

		}
		if ($5!=-1) {
			typeDefContainer->setAsArray();	

    	}
	}
	char *orgtype=GetToken($2);
	DataType *type=thisCodeFile->FindDataType(orgtype);
	if (type==NULL) {
		type=new DataType(orgtype);
      thisCodeFile->AddDataType(type);
	}
	if ($3>0) {
      //type=new TypePtr(NULL,$3, type);
      type=new TypePtr(NULL,$3, type, constPointerPositions);
      thisCodeFile->AddDataType(type);
	}
  if ($5!=-1)
    {
      type=new TypeArray(NULL,GetToken($5), type);
      thisCodeFile->AddDataType(type); 
    }
  type=new TypeEqual(GetToken($4),type);
  thisCodeFile->AddDataType(type); 
} 
| TYPEDEF_KEYWORD STRUCT_KEYWORD ID pointer_specifier ID array_declarator
{
	if(insideClass){
		sprintf(tmp,"typedef definition with structure inside a parclass is not currently supported !\n");
	
		errormsg(tmp);
		exit(1);
	}
  char *orgtype=GetToken($3);
  DataType *type=thisCodeFile->FindDataType(orgtype);
  if (type==NULL)
    {
      type=new TypeClassStruct(orgtype,false);
      thisCodeFile->AddDataType(type);
    }
  if ($4>0)
    {
      //type=new TypePtr(NULL,$4, type);
      type=new TypePtr(NULL, $4, type, constPointerPositions);
      
      thisCodeFile->AddDataType(type);
    }
  if ($6!=-1)
    {
      type=new TypeArray(NULL,GetToken($6), type);
      thisCodeFile->AddDataType(type); 
    }
  type=new TypeEqual(GetToken($5),type);
  thisCodeFile->AddDataType(type);   
}
| TYPEDEF_KEYWORD CLASS_KEYWORD ID pointer_specifier ID array_declarator
{
  char *orgtype=GetToken($3);
  DataType *type=thisCodeFile->FindDataType(orgtype);
  if (type==NULL)
    {
      type=new TypeClassStruct(orgtype, true);
      thisCodeFile->AddDataType(type);
    }
  if ($4>0)
    {
      //type=new TypePtr(NULL,$4, type);
      type=new TypePtr(NULL, $4, type, constPointerPositions);
      
      thisCodeFile->AddDataType(type);
    }
  if ($6!=-1)
    {
      type=new TypeArray(NULL,GetToken($6), type);
      thisCodeFile->AddDataType(type); 
    }
  type=new TypeEqual(GetToken($5),type);
  thisCodeFile->AddDataType(type);   
}
|  TYPEDEF_KEYWORD struct_definition pointer_specifier ID array_declarator
{
	if(insideClass){
		sprintf(tmp,"typedef definition with structure inside a parclass is not currently supported !\n");
	
		errormsg(tmp);
		exit(1);
	}
  DataType *type=currentstruct;
  assert(type!=NULL);

  if ($3>0)
    {
      //type=new TypePtr(NULL,$3, type);
      type=new TypePtr(NULL, $3, type, constPointerPositions);
      
      thisCodeFile->AddDataType(type);
    }
  if ($5!=-1)
    {
      type=new TypeArray(NULL,GetToken($5), type);
      thisCodeFile->AddDataType(type); 
    }
  type=new TypeEqual(GetToken($4),type);
  thisCodeFile->AddDataType(type); 
}
;

/*
SEQUENTIAL CLASS DEFINITION...
 */
seqclass_definition: seqclass_header seqbase_spec '{' error '}' 
{
	if (seqclass!=NULL) 
		seqclass->SetTypeClass(true);
}
| seqclass_header ';'

seqclass_header: CLASS_KEYWORD ID
{
	DataType *t=thisCodeFile->FindDataType(GetToken($2));
	if (t==NULL) {
		seqclass=new TypeClassStruct(GetToken($2),true);
		thisCodeFile->AddDataType(seqclass);
	} else if (!t->IsStandardType()) {
		seqclass=dynamic_cast<TypeClassStruct *>(t);
      if (seqclass==NULL) {
			char tmp[256];
			sprintf(tmp,"%s has been declared as non-class data type", GetToken($2));
			errormsg(tmp);
			exit(1);
		}
	}
}

seqbase_spec: /*empty*/
| ':' seqbase_list
;


seqbase_list: seqbase_specifier
| seqbase_list ',' seqbase_specifier
;

seqbase_specifier: ID
{
	assert(seqclass!=NULL);
	TypeClassStruct *seqbase;
	DataType *t=thisCodeFile->FindDataType(GetToken($1));
	if (t==NULL) {
		seqbase=new TypeClassStruct(GetToken($1), true);
		seqclass->AddBase(seqbase);  
	} else if (!t->IsStandardType()) {
      seqclass->AddBase(t);
	}
}
| access_specifier ID
{
	assert(seqclass!=NULL);
	TypeClassStruct *seqbase;
	DataType *t=thisCodeFile->FindDataType(GetToken($2));
	if (t==NULL) {
		seqbase=new TypeClassStruct(GetToken($2), true);
      seqclass->AddBase(seqbase);  
	} else  if (!t->IsStandardType()) {
		seqclass->AddBase(t);
	}
}
| access_specifier ID '<' ID '>'
{
	assert(seqclass!=NULL);
	TypeClassStruct *seqbase;
	DataType *t=thisCodeFile->FindDataType(GetToken($2));
	if (t==NULL) {
		seqbase=new TypeClassStruct(GetToken($2), true);
      seqclass->AddBase(seqbase);  
	} else  if (!t->IsStandardType()) {
		seqclass->AddBase(t);
	}
}
| VIRTUAL_KEYWORD access_specifier ID
{
	assert(seqclass!=NULL);
	TypeClassStruct *seqbase;
	DataType *t=thisCodeFile->FindDataType(GetToken($3));
	if (t==NULL) {
		seqbase=new TypeClassStruct(GetToken($3), true);
		seqclass->AddBase(seqbase);  
	} else  if (!t->IsStandardType()) {
      seqclass->AddBase(t);
	}
}
| access_specifier VIRTUAL_KEYWORD ID
{
	assert(seqclass!=NULL);
	TypeClassStruct *seqbase;
	DataType *t=thisCodeFile->FindDataType(GetToken($3));
	if (t==NULL) {
		seqbase=new TypeClassStruct(GetToken($3), true);
		seqclass->AddBase(seqbase);
	} else  if (!t->IsStandardType()) {
      seqclass->AddBase(t);
	}
};


/*
PARALLEL OBJECT CLASS DEFINITION
 */
class_prototype: class_key ';'
{
  assert(currentClass!=NULL);
  sprintf(tmp,"class %s;",currentClass->GetName());

  assert(thisCodeFile!=NULL);
  OtherCode *dat=new OtherCode(thisCodeFile);
  dat->AddCode(tmp);
  thisCodeFile->AddCodeData(dat);

  othercodes.SetSize(0);

  currentClass=NULL;
  insideClass=false;
}
;

/* 
Parallel class declaration
*/

class_declaration:  class_head '{' member_list '}' ';'
{
  currentClass->SetEndLine(linenumber);
  currentClass=NULL;
  insideClass=false;
}
;

class_head: class_key pure_class_decl base_spec 
{
  accessmodifier=PUBLIC;
  if ($2) currentClass->SetPureVirtual(true);

  thisCodeFile->AddCodeData(currentClass);
};


class_key: PARCLASS_KEYWORD ID
{
	hadParclass = true; 
	if (othercodes.GetSize() && startPos>0) {
      assert(thisCodeFile!=NULL);
      OtherCode *dat=new OtherCode(thisCodeFile);
      dat->AddCode((char *)othercodes,startPos);
      thisCodeFile->AddCodeData(dat);
      othercodes.SetSize(0);
	}
	insideClass=true;
	char *clname=GetToken($2);
  
	Class *t;
	if ((t=thisCodeFile->FindClass(clname))==NULL) {
      t=new Class(clname, thisCodeFile);
      thisCodeFile->AddDataType(t);
      
      // Pass compilation options to classes
      if(thisCodeFile->IsCoreCompilation())
      	t->SetAsCoreCompilation();
		if(thisCodeFile->IsAsyncAllocationDisable()) {
			t->DisableAsyncAllocation();
		}
      if(isWarningEnable)
      	t->EnableWarning();  
	}
	t->SetFileInfo(filename);
	t->SetStartLine(linenumber);
	currentClass=t;
	
	if(isNamespace) {
		
		t->SetNamespace(holdnamespace);
	
	}

};

base_spec: /*empty*/
| ':' base_list
;

base_list: base_specifier
| base_list ',' base_specifier
;

base_specifier: ID
{
	assert(currentClass!=NULL);
	Class *cl=thisCodeFile->FindClass(GetToken($1));
	if (cl==NULL) 
	  {
	    char str[1024];
	    sprintf(str,"base class %s not declared",GetToken($1));
	    errormsg(str);
	    exit(1);
	  }
	BaseClass *t=new BaseClass(cl, PUBLIC, false);
	currentClass->baseClass.InsertAt(-1,t);
}
| access_specifier ID
{
	assert(currentClass!=NULL);
	AccessType accessmode=(AccessType)$1;
	Class *cl=thisCodeFile->FindClass(GetToken($2));
	if (cl==NULL) 
	  {
	    char str[1024];
	    sprintf(str,"base class %s not declared",GetToken($2));
	    errormsg(str);
	    exit(1);
	  }
	BaseClass *t=new BaseClass(cl,accessmode,false);
	currentClass->baseClass.InsertAt(-1,t);
}
| VIRTUAL_KEYWORD access_specifier ID
{
	assert(currentClass!=NULL);
	AccessType accessmode=(AccessType)$2;

	Class *cl=thisCodeFile->FindClass(GetToken($3));
	if (cl==NULL) 
	  {
	    char str[1024];
	    sprintf(str,"base class %s not declared",GetToken($3));
	    errormsg(str);
	    exit(1);
	  }

	BaseClass *t=new BaseClass(cl,accessmode,true);
	currentClass->baseClass.InsertAt(-1,t);
}
| access_specifier VIRTUAL_KEYWORD ID
{
	assert(currentClass!=NULL);
	AccessType accessmode=(AccessType)$1;

	Class *cl=thisCodeFile->FindClass(GetToken($3));
	if (cl==NULL) 
	  {
	    char str[1024];
	    sprintf(str,"base class %s not declared",GetToken($3));
	    errormsg(str);
	    exit(1);
	  }

	BaseClass *t=new BaseClass(cl,accessmode,true);
	currentClass->baseClass.InsertAt(-1,t);
}
;

access_specifier: PRIVATE_KEYWORD
{
	$$=PRIVATE;
}
| PUBLIC_KEYWORD
{
	$$=PUBLIC;
}
| PROTECTED_KEYWORD
{
	$$=PROTECTED;
}
;


pure_class_decl: /*empty*/
{
  $$=0;
}
| '=' INTEGER
{
  $$= (paroc_utils::isEqual(GetToken($2), "0")) ? 1 : 0;
}
;
/* 
Parallel class member declaration 
*/

member_list: /*empty*/
| member_declaration member_list
| access_specifier { accessmodifier=(AccessType)$1; } ':' member_list
;


member_declaration:  enum_declaration ';'
| struct_definition ';'
| function_definition ';'
| innerclass_definition ';' | innerclass_static ';' | typedef_definition ';'
{
	assert(method!=NULL);
	int t=method->CheckMarshal();
	if (t!=0) {      
		if (t==-1) {
			sprintf(tmp,"In method %s::%s: unable to marshal the return argument.\n", currentClass->GetName(), method->name);
      } else {
      	
			sprintf(tmp,"In method %s::%s: unable to marshal argument %d.\n", currentClass->GetName(), method->name, t);
		}
      errormsg(tmp);
		exit(1);
	}
	currenttype=returntype=NULL;
}
 
| attribute_definition ';'
{
	if (accessmodifier==PUBLIC) {
      sprintf(tmp,"%s:%d: attributes of a parallel class must be private or protected.\n",thisCodeFile->GetFileName(), linenumber);
      errormsg(tmp);
      exit(1);
	}
	currenttype=returntype=NULL;  
}
| DIRECTIVE
{
  //  Directive *t=new Directive(currentClass, GetToken($1));
  //  currentClass->AddMember(t);
}
| CLASSID '(' INTEGER ')' ';'
{
  currentClass->SetClassID(GetToken($3));
}
;


/**
 * @author : clementval
 * Enum declaration 
 */
enum_declaration: ENUM ID '{' enum_members '}'
{
	assert(currentClass!=NULL);
	Enumeration *t = new Enumeration(currentClass, accessmodifier);
	t->SetLineInfo(linenumber-1);
	currentClass->AddMember(t);
	t->setName(GetToken($2));
	t->setArgs(GetToken($4));
}
;

enum_members: enum_member 
{
	$$ = $1;	
}
| enum_member ',' enum_members
{
	sprintf(tmp,"%s , %s",GetToken($1), GetToken($3));
	$$ = PutToken(tmp);
}
;

enum_member: ID
{
	$$ = $1;
}
| ID '=' INTEGER
{      
	sprintf(tmp,"%s = %s",GetToken($1), GetToken($3));
   $$=PutToken(tmp);
}
;


/*
Attribute declaration....
 */
attribute_definition: decl_specifier attribute_name_list

attribute_name_list: attribute_name
| attribute_name  ','  attribute_name_list
;

attribute_name: pointer_specifier ref_specifier ID array_declarator
{
	assert(currentClass!=NULL);
	Attribute *t=new Attribute(currentClass,accessmodifier);
	t->SetLineInfo(linenumber-1);
	currentClass->AddMember(t);
	Param *tparam = t->NewAttribute();
	DataType *type;
	strcpy(tparam->name,GetToken($3));
	
	if ($1==0) {
		type=currenttype;
	} else {
      type=new TypePtr(NULL, $1 , currenttype, constPointerPositions);
      thisCodeFile->AddDataType(type);
	}

	if ($4!=-1) {
		type=new TypeArray(NULL, GetToken($4) , type);
		thisCodeFile->AddDataType(type);
	}
	tparam->isRef=$2;
	tparam->SetType(type);
};

array_declarator: /*empty*/
{
  $$=-1;
}
| '[' ']'
{
	$$=0;
}
| '[' expr_decl ']' array_declarator
{
	if ($4==-1) {
		sprintf(tmp,"[%s]",GetToken($2));
		$$=PutToken(tmp);
	} else {
      sprintf(tmp,"[%s]%s",GetToken($2),GetToken($4));
      $$=PutToken(tmp);
	}
}
;

decl_specifier: storage_class_specifier type_specifier
{
  $$=$1;
}
| type_specifier
{
	
	
	$$=0;
}
;

storage_class_specifier:  AUTO_KEYWORD
{
  $$=1;  
}
| REGISTER_KEYWORD
{
  $$=2;
}
;

type_specifier: ID
{
	if(isInStruct)
		sprintf(typetmp, "%s", GetToken($1)); // Save the type specifier for struct attribute
	currenttype=thisCodeFile->FindDataType(GetToken($1));
	if (currenttype==NULL) {
	   currenttype=new DataType(GetToken($1));
      thisCodeFile->AddDataType(currenttype);
   }
	$$=(YYSTYPE)currenttype;
}
| ID1 
{
  currenttype=thisCodeFile->FindDataType(GetToken($1));
  if (currenttype==NULL)
    {
      currenttype=new DataType(GetToken($1));
      thisCodeFile->AddDataType(currenttype);

      /*
	sprintf(tmp,"Undeclared type \"%s\"\n",GetToken($1)); 
	errormsg(tmp);
	exit(1);
      */
    }
  $$=(YYSTYPE)currenttype;
}
| ID '<' template_arguments '>'
{

  TypeTemplate *type=new TypeTemplate(GetToken($1));
  paroc_list<TemplateArgument *> *list=(paroc_list<TemplateArgument *> *)$3;
  POSITION pos=list->GetHeadPosition();
  while (pos!=NULL)
    {
      TemplateArgument *el=list->GetNext(pos);
      type->AddTemplate(el->type, el->isRef);
      delete el;
    }
  delete list;
 
  thisCodeFile->AddDataType(type);
  currenttype=type;
  $$=(YYSTYPE)currenttype;

}
| struct_definition  
{
  assert(currenttype!=NULL);
  currenttype=currentstruct;
  $$=(YYSTYPE)currenttype;
}
| STRUCT_KEYWORD ID
{
  DataType *type=thisCodeFile->FindDataType(GetToken($2));
  if (type==NULL)
    {
      type=new TypeClassStruct(GetToken($2), false);
      thisCodeFile->AddDataType(type);
    } 
  currenttype=type;
  $$=(YYSTYPE)currenttype;
}
;

template_arguments: template_arg
{
  paroc_list<TemplateArgument *> *list=new paroc_list<TemplateArgument *>();
  TemplateArgument *v=(TemplateArgument *)$1;
  list->AddHead(v);
  $$=(YYSTYPE)list;
}
| template_arg ',' template_arguments 
{
  paroc_list<TemplateArgument *> *list=(paroc_list<TemplateArgument *> *)$3;
  TemplateArgument *v=(TemplateArgument *)$1;
  list->AddHead(v);
  $$=(YYSTYPE)list;
}
;

template_arg: type_specifier pointer_specifier array_declarator ref_specifier
{
  TemplateArgument *t=new TemplateArgument;	
  t->type=(DataType *)$1;
  if ($2>0)
  {
	//t->type=new TypePtr(NULL,$2,t->type);
	t->type=new TypePtr(NULL, $2, t->type, constPointerPositions);
  }
  if ($3!=-1) 
  {
	t->type=new TypeArray(NULL,GetToken($3), t->type);
  }
  t->isRef=($4!=0);
  $$=(YYSTYPE)t;
  
}
;
/*
template_arg: ID pointer_specifier array_declarator ref_specifier
{
  if ($2==0 && $3==-1 && $4==0)
    {
      $$=$1;
    }
  else
    {
      strcpy(tmp, GetToken($1));
      for (int i=0;i<$2; i++) strcat(tmp, "*"); 
      if ($3!=-1) strcat(tmp,GetToken($3));
      if ($4) strcat(tmp,"&");
      $$=PutToken(tmp);
    }
}
| ID1 pointer_specifier array_declarator ref_specifier
{
  if ($2==0 && $3==-1 && $4==0)
    {
      $$=$1;
    }
  else
    {
      strcpy(tmp, GetToken($1));
      for (int i=0;i<$2; i++) strcat(tmp, "*"); 
      if ($3!=-1) strcat(tmp,GetToken($3));
      if ($4) strcat(tmp,"&");
      $$=PutToken(tmp);
    }
}
|  ID '<' template_arguments '>'
{
  sprintf(tmp,"%s<%s>",GetToken($1), GetToken($3));
  $$= PutToken(tmp);
}
;

*/


/*
pointer_specifier:/*empty
{
  $$=0;
  
}
| '*' pointer_specifier
{
  $$=$2+1;
}
;
*/

pointer_specifier:/*empty*/
{
	$$=0;
	constPointerPositions.clear();
}
| '*' const_specifier pointer_specifier
{
	$$=$3+1; /* modivied by David (added const)*/
	constPointerPositions.push_back(($2 == 1 ? true : false));
}
;

/* controll if "const" word is present - Added by david */
const_specifier:/*empty*/
{
  $$=0;
}
| CONST_KEYWORD
{
  $$=1;
}
;

ref_specifier: /*empty*/
{
  $$=0;
}
| '&'
{
  $$=1;
}
;

/*
Method declaration 
*/

function_definition: constructor_definition
| destructor_definition
| method_definition pure_virtual_decl
;

pure_virtual_decl: /*empty*/
| '=' INTEGER
{
  method->isPureVirtual=paroc_utils::isEqual(GetToken($2),"0");
}
;

constructor_definition: constructor_name '(' argument_declaration ')' od_specifier  
{
  if (!paroc_utils::isEqual(method->name,currentClass->GetName()))
    {
      errormsg("Bad declaration of constructor\n");
      exit(1);
    }
  if ($5!=-1)
    {
      ObjDesc &od=((Constructor *)method)->GetOD();
      od.SetCode(GetToken($5));
    }
  currentClass->AddMember(method);
}
;

constructor_name: ID
{
  method=new Constructor(currentClass,accessmodifier);
  method->SetLineInfo(linenumber-1);

  strcpy(method->name,GetToken($1));
}
;

destructor_definition: '~' destructor_name '('  ')' 
{
  if (!paroc_utils::isEqual(method->name,currentClass->GetName()))
    {
      errormsg("Bad declaration of destructor\n");
      exit(1);
    }
	strcpy(method->name,currentClass->GetName());
}
| VIRTUAL_KEYWORD '~' destructor_name '('  ')' 
{
  if (!paroc_utils::isEqual(method->name,currentClass->GetName()))
    {
      errormsg("Bad declaration of destructor\n");
      exit(1);
    }
	strcpy(method->name,currentClass->GetName());
	method->isVirtual=true;
	
}
;

destructor_name: ID
{
  method=new Destructor(currentClass,accessmodifier);
  method->SetLineInfo(linenumber-1);
  currentClass->AddMember(method);
  strcpy(method->name,GetToken($1));
}
;

/*
method_definition: decl_specifier pointer_specifier ref_specifier function_name '(' argument_declaration ')' 
{
  //Old data:  argument_type function_name 
  strcpy(method->returnparam.name,"_RemoteRet");

  DataType *type=returntype;
  if ($2>0)
    {
      type=new TypePtr(NULL, $2 , type);
      thisCodeFile->AddDataType(type);
    }

  if ($3)
    {
      method->returnparam.isRef=true;
    }
  method->returnparam.SetType(type);

}
|  fct_specifier decl_specifier  pointer_specifier ref_specifier function_name '(' argument_declaration ')'
{
  strcpy(method->returnparam.name,"_RemoteRet");

  DataType *type=returntype;
  if ($3>0)
    {
      type=new TypePtr(NULL, $3 , type);
      thisCodeFile->AddDataType(type);
    }
  
  if ($4)
    {
      method->returnparam.isRef=true;
    }

  method->returnparam.SetType(type);

  method->isVirtual=(($1 & 1)!=0);
  if (($1 & 8)!=0) method->isConcurrent=true;
  else if (($1 & 32)!=0) method->isConcurrent=false;

  method->isHidden=(($1 & 64)!=0);
  method->isMutex=(($1 & 16)!=0);

  if (($1 & 6)==2) method->invoketype=invokesync;
  else if (($1 & 6)==4) method->invoketype=invokeasync;
  //else method->invoketype=autoselect;
}
| '[' marshal_opt_list ']' decl_specifier  pointer_specifier ref_specifier function_name { UpdateMarshalParam($2,&(method->returnparam) ); } '(' argument_declaration ')'
{
  DataType *type=returntype;
  if ($5>0)
    {
      type=new TypePtr(NULL, $5 , type);
      thisCodeFile->AddDataType(type);
    }
  
  if ($6)
    {
      method->returnparam.isRef=true;
    }
  method->returnparam.SetType(type);
  
  strcpy(method->returnparam.name,"_RemoteRet");
}
| fct_specifier  '[' marshal_opt_list ']' decl_specifier  pointer_specifier ref_specifier function_name { UpdateMarshalParam($3,&(method->returnparam) ); } '(' argument_declaration ')'
{
  strcpy(method->returnparam.name,"_RemoteRet");

  DataType *type=returntype;
  if ($6>0)
    {
      type=new TypePtr(NULL, $6 , type);
      thisCodeFile->AddDataType(type);
    }
  
  if ($7)
    {
      method->returnparam.isRef=true;
    }
  method->returnparam.SetType(type);

  method->isVirtual=(($1 & 1)!=0);

  if (($1 & 8)!=0) method->isConcurrent=true;
  else if (($1 & 32)!=0) method->isConcurrent=false;
  
  method->isMutex=(($1 & 16)!=0);
  method->isHidden=(($1 & 64)!=0);
  
  if (($1 & 6)==2) method->invoketype=invokesync;
  else if (($1 & 6)==4) method->invoketype=invokeasync;
  // else method->invoketype=autoselect;
}
;
*/

/*
method_definition: decl_specifier pointer_specifier ref_specifier function_name '(' argument_declaration ')' 
{
	setReturnParam($2,$3, 0);
}
|  fct_specifier decl_specifier pointer_specifier ref_specifier function_name '(' argument_declaration ')'
{
	setReturnParam($3,$4, ($1 & 1));
	setPOPCMethodeModifier($1 & 254);
}
| '[' marshal_opt_list ']' decl_specifier  pointer_specifier ref_specifier function_name { UpdateMarshalParam($2,&(method->returnparam) ); } '(' argument_declaration ')'
{
	setReturnParam($5,$6, ($1 & 1));
}
| fct_specifier  '[' marshal_opt_list ']' decl_specifier  pointer_specifier ref_specifier function_name { UpdateMarshalParam($3,&(method->returnparam) ); } '(' argument_declaration ')'
{
	setReturnParam($6,$7, ($1 & 1));
	setPOPCMethodeModifier($1 & 254);
}
;
*/


method_definition: decl_specifier pointer_specifier ref_specifier function_name '(' argument_declaration ')' const_specifier
{
	setReturnParam($2,$3, 0);
	
	method->isGlobalConst = ($8 == 1 ? true : false); 
	errorGlobalMehtode(method->isGlobalConst);
}
| const_virutal_specifier decl_specifier pointer_specifier ref_specifier function_name '(' argument_declaration ')' const_specifier
{
	setReturnParam($3,$4, $1);
	
	method->isGlobalConst = ($9 == 1 ? true : false); 
	errorGlobalMehtode(method->isGlobalConst);
}
| decl_specifier const_virutal_specifier pointer_specifier ref_specifier function_name '(' argument_declaration ')' const_specifier
{
	setReturnParam($3,$4, $2);
	
	method->isGlobalConst = ($9 == 1 ? true : false); 
	errorGlobalMehtode(method->isGlobalConst);
}
| const_virutal_specifier decl_specifier const_virutal_specifier pointer_specifier ref_specifier function_name '(' argument_declaration ')' const_specifier
{
	setReturnParam($4,$5, ($1 | $3));
	
	method->isGlobalConst = ($10 == 1 ? true : false);
	errorGlobalMehtode(method->isGlobalConst); 
}
| fct_specifier const_virutal_empty_specifier decl_specifier const_virutal_empty_specifier pointer_specifier ref_specifier function_name '(' argument_declaration ')' const_specifier
{
	setReturnParam($5,$6, ($2|$4));
	setPOPCMethodeModifier($1);
	
	method->isGlobalConst = ($11 == 1 ? true : false);
	errorGlobalMehtode(method->isGlobalConst);
}
| '[' marshal_opt_list ']' const_virutal_empty_specifier decl_specifier const_virutal_empty_specifier pointer_specifier ref_specifier function_name { UpdateMarshalParam($2,&(method->returnparam) ); } '(' argument_declaration ')' const_specifier
{
	setReturnParam($7,$8, ($4 | $6));
	
	method->isGlobalConst = ($13 == 1 ? true : false);
	errorGlobalMehtode(method->isGlobalConst);
}
| fct_specifier  '[' marshal_opt_list ']' const_virutal_empty_specifier decl_specifier const_virutal_empty_specifier pointer_specifier ref_specifier function_name { UpdateMarshalParam($3,&(method->returnparam) ); } '(' argument_declaration ')' const_specifier
{
	setReturnParam($8,$9, ($5 & 7));
	setPOPCMethodeModifier($1);
	
	method->isGlobalConst = ($14 == 1 ? true : false);
	errorGlobalMehtode(method->isGlobalConst);
}
;

fct_specifier:  fct_spec
{
  $$=$1;
}
|  fct_specifier fct_spec
{
	/* error if multimple time same reserved word */
	if (($2 & $1) != 0)
	{
		errormsg("Multiple semantics keyword");
		exit(1);
	}
  
	$$=$1 | $2;
}
;

/*
fct_spec: VIRTUAL_KEYWORD
{
	$$=1;
} 
| SYNC_INVOKE
{
	$$=2;
}
| ASYNC_INVOKE
{
	$$=4;
} 
| CONCURRENT
{
	$$=8;
}
| SEQUENTIAL
{
	$$=32;
}
| MUTEX
{
  $$=16;
}
| HIDDEN
{
  $$=64;
}
;
*/

/* remove C++ Virtual - david  */
fct_spec: SYNC_INVOKE
{
	$$=2;
}
| ASYNC_INVOKE
{
	$$=4;
} 
| CONCURRENT
{
	$$=8;
}
| SEQUENTIAL
{
	$$=32;
}
| MUTEX
{
  $$=16;
}
| HIDDEN
{
  $$=64;
}
;

/* added by david, for dedection of const or/and vitural methode modivier */
const_virutal_specifier: VIRTUAL_KEYWORD
{
	$$=1;
} 
| CONST_KEYWORD
{
	$$=2;
}
| CONST_KEYWORD VIRTUAL_KEYWORD
{
	$$=3;
}
| VIRTUAL_KEYWORD CONST_KEYWORD
{
	$$=3;
}
;

/* added by david, for dedection of none, const or/and vitural methode modivier */
const_virutal_empty_specifier: /* empty */
{
	$$=0;
}
|VIRTUAL_KEYWORD
{
	$$=1;
} 
| CONST_KEYWORD
{
	$$=2;
}
| CONST_KEYWORD VIRTUAL_KEYWORD
{
	$$=3;
}
| VIRTUAL_KEYWORD CONST_KEYWORD
{
	$$=3;
}
;

function_name: ID // realistic addition of funciton?
{
  method=new Method(currentClass,accessmodifier);
  method->SetLineInfo(linenumber-1);
  currentClass->AddMember(method);
  strcpy(method->name,GetToken($1));
  returntype=currenttype;
  currenttype=NULL;
}
;

od_specifier: /*empty*/
{
  $$=-1;
}
| '@' '{' od_exprlist '}'
{
  $$=$3;
}
;

od_exprlist: /*empty*/
{
  $$=-1;
}
//Added by clementval
| ID '.' ID '(' expr_decl  ',' expr_decl ',' expr_decl ')' ';' od_exprlist
{

  char *odtmp=GetToken($1);
  assert(method!=NULL);
  if (!paroc_utils::isEqual(odtmp,"od"))
    {
      sprintf(tmp,"Invalid OD expression: %s.%s",odtmp,GetToken($3));
      errormsg(tmp);
      exit(1);
    }

  sprintf(tmp,"od.%s(%s,%s,%s);",GetToken($3),GetToken($5),GetToken($7),GetToken($9));
  if ($12!=-1) strcat(tmp,GetToken($12));
  $$=PutToken(tmp);
}
//End of add
| ID '.' ID '(' expr_decl  ',' expr_decl ')' ';' od_exprlist
{

  char *odtmp=GetToken($1);
  assert(method!=NULL);
  if (!paroc_utils::isEqual(odtmp,"od"))
    {
      sprintf(tmp,"Invalid OD expression: %s.%s",odtmp,GetToken($3));
      errormsg(tmp);
      exit(1);
    }

  sprintf(tmp,"od.%s(%s,%s);",GetToken($3),GetToken($5),GetToken($7));
  if ($10!=-1) strcat(tmp,GetToken($10));
  $$=PutToken(tmp);
}
|  ID '.' ID '(' expr_decl ')' ';' od_exprlist
{
  char *odtmp=GetToken($1);
  assert(method!=NULL);
  if (!paroc_utils::isEqual(odtmp,"od"))
    {
      sprintf(tmp,"Invalid OD expression: %s.%s",odtmp,GetToken($3));
      errormsg(tmp);
      exit(1);
    }
  
  
  //add by Yao, Jun 14 2014
  char *odtmpp=GetToken($3);
  if (paroc_utils::isEqual(odtmpp,"SensorDes"))
  {    
  	
  	std::string path = GetToken($5);
	std::string tvalue;
	tvalue = "  char* fpath = " + path + ";\n"; //file path name
	tvalue = tvalue + "  std::ifstream DesFile;\n  DesFile.open(fpath);\n  Json::Reader reader;\n  Json::Value root;\n reader.parse(DesFile,root,false);\n  std::string ppower,pmemory,purl;\n  Json::Value addvalue = root[\"machine\"];\n  for (int i = 0; i < addvalue.size(); ++i)\n  {Json::Value temp_value = addvalue[i];\n   ppower = temp_value[\"power\"].asString();\n  pmemory = temp_value[\"memory\"].asString();\n  purl = temp_value[\"url\"].asString();}\n  if(ppower!=\"\"){int powNum = atoi(ppower.c_str());od.power(powNum);}\n  if(pmemory!=\"\"){int memNum = atoi(pmemory.c_str()); od.memory(memNum); }\n  if(purl!=\"\"){od.url(purl.c_str());}\n";	

	const char* pvalue = tvalue.c_str();
	sprintf(tmp,"%s",pvalue);

  	if ($8!=-1) strcat(tmp,GetToken($8));
  	$$=PutToken(tmp);
  }
//end add
  else 
  {
  sprintf(tmp,"od.%s(%s);",GetToken($3),GetToken($5));
  if ($8!=-1) strcat(tmp,GetToken($8));
  $$=PutToken(tmp);}
}

| ID '=' expr_decl od_expr_nonstrict ';' od_exprlist
{
    char *odtmp=GetToken($1);
    if (paroc_utils::isEqual(odtmp,"host"))
      {
	sprintf(tmp,"od.url(%s);",GetToken($3));
	if ($6!=-1) strcat(tmp,GetToken($6));
	if ($4!=-1)
	  {
	    errormsg("OD: host should be a string expression. Non-strict description is not allowed");
	    exit(1);
	  }
      } 
    else if (paroc_utils::isEqual(odtmp,"jobcontact"))
      {
	sprintf(tmp,"od.joburl(%s);",GetToken($3));
	if ($6!=-1) strcat(tmp,GetToken($6));
	if ($4!=-1)
	  {
	    errormsg("OD: jobcontact should be a string expression. Non-strict description is not allowed");
	    exit(1);
	  }
      } 
    else if (paroc_utils::isEqual(odtmp,"memory"))
      {
	sprintf(tmp,"od.memory(%s",GetToken($3));
	if ($4!=-1)
	  {
	    strcat(tmp,",");
	    strcat(tmp,GetToken($4));
	  }
	strcat(tmp,");");
	if ($6!=-1) strcat(tmp,GetToken($6));
      } 
    else if (paroc_utils::isEqual(odtmp,"power"))
      {
	sprintf(tmp,"od.power(%s",GetToken($3));
	if ($4!=-1)
	  {
	    strcat(tmp,",");
	    strcat(tmp,GetToken($4));
	  }
	strcat(tmp,");");
//Added by clementval
	if ($6!=-1) strcat(tmp,GetToken($6));
      } 
    else if (paroc_utils::isEqual(odtmp,"search"))
      {
	sprintf(tmp,"od.search(%s",GetToken($3));
	if ($4!=-1)
	  {
	    strcat(tmp,",");
	    strcat(tmp,GetToken($4));
	  }
	if ($5!=-1)
	  {
	    strcat(tmp,",");
	    strcat(tmp,GetToken($5));
	  }
	strcat(tmp,");");
//End of add
	if ($6!=-1) strcat(tmp,GetToken($6));
      } 
    else if (paroc_utils::isEqual(odtmp,"network"))
    {
	sprintf(tmp,"od.bandwidth(%s",GetToken($3));
	if ($4!=-1)
	  {
	    strcat(tmp,",");
	    strcat(tmp,GetToken($4));
	  }
	strcat(tmp,");");
	if ($6!=-1) strcat(tmp,GetToken($6));
    } 
    else if (paroc_utils::isEqual(odtmp,"walltime"))
      {
	sprintf(tmp,"od.walltime(%s);",GetToken($3));
	if ($4!=-1)
	  {
	    errormsg("OD: walltime  should be a number expression. Non-strict description is not allowed");
	    exit(1);
	  }
      } 
    else
      {
	errormsg("Unknown OD declaration");
	exit(1);
      }
    $$=PutToken(tmp);
}
;

od_expr_nonstrict: /*empty*/
{
    $$=-1;
}
| NONSTRICT_OD_OP expr_decl
{
    $$=$2;
}
;
/*
METHOD ARGUMENT DECLARATIONS
 */
argument_declaration: /*empty*/
| argument_list;

argument_list:  arg_declaration
| arg_declaration ',' argument_list 
;

arg_declaration: marshal_decl cv_qualifier decl_specifier cv_qualifier pointer_specifier ref_specifier argument_name array_declarator arg_default_value
{
	Param *t=method->AddNewParam();
	UpdateMarshalParam($1,t);
	
	
	DataType *type=currenttype;
	if ($5>0) {
      type=new TypePtr(NULL, $5 , type, constPointerPositions);
      thisCodeFile->AddDataType(type);
	}

	if ($6) {
		t->isRef=true;
	}
	
	// Handle standard array passing declaration
	if($8 == 0){
		t->isArray=true;
		//Find last int as size of array
		int nb=method->params.GetSize()-1;
		
		std::string size_variable_name;
		
		for (int j=0;j<nb;j++) {
			Param &p=*(method->params[j]);
			if(strcmp("int", p.GetType()->GetName())==0){
				size_variable_name.clear();
				size_variable_name.append(p.GetName());
			}
		}
		if(size_variable_name.length() == 0){
			//
			sprintf(tmp,"Could not find size to marshall array: %s\n", GetToken($7));
			errormsg(tmp);
			exit(1);
		}
		strcpy(tmpSize, size_variable_name.c_str());
		UpdateMarshalParam(67,t);
      type=new TypePtr(NULL, 1, type, constPointerPositions);
      thisCodeFile->AddDataType(type);
	} else if ($8 != -1) {
		type=new TypeArray(NULL, GetToken($8) , type);
		thisCodeFile->AddDataType(type);
	}

	t->SetType(type);
	if ($7 != -1) {
		strcpy(t->name,GetToken($7));
	} else {
		if(strcmp("void", t->GetType()->GetName()) == 0){
			t->isVoid = true;
		} else {
			sprintf(t->name,"V_%d",++counter);
		}
	}

	t->isConst=(($2==1) || ($4==1));
	if ($9 >= 0) 
		t->defaultVal=strdup(GetToken($9));
} 
;

marshal_decl:  /*empty*/
{
  $$=0;
}
| '[' marshal_opt_list ']'
{
	$$=$2;
}
;

marshal_opt_list: marshal_opt
{
	$$=$1;
}
| marshal_opt ',' marshal_opt_list
{
	$$=$1 | $3 ;
}
;

marshal_opt: INPUT
{
  $$=PARAM_IN;
}
| OUTPUT
{
  $$=PARAM_OUT;
}
| SIZE '=' expr_decl 
{
  strcpy(tmpSize,GetToken($3));
  $$=PARAMSIZE;
}
| PROC '=' ID
{
  strcpy(tmpProc,GetToken($3));
  $$=USERPROC;
}
;


cv_qualifier: /*empty*/
{
	$$=0;
}
| CONST_KEYWORD
{
	$$=1;
}
| VOLATILE_KEYWORD
{
	$$=2;
}
;

argument_name: /*empty*/
{
	$$=-1;
}
| ID
{
	$$=$1;
}
;

arg_default_value: /*empty*/
{
  $$=-1;
}
| '=' expr_decl
{
  $$=$2;
}
;


/*
EXPRESSION DECLARATION
The expression is used to parse the OD
*/


expr_decl: expr_name array_declarator 
{
  if ($2<0) $$=$1;
  else 
    {
      sprintf(tmp, "%s%s",GetToken($1),GetToken($2));
      $$=PutToken(tmp);
    }
}
| number
{
  $$=$1;
}
| STRING
{
    $$=$1;
}
| expr_decl '+' expr_decl
{
  sprintf(tmp,"%s + %s",GetToken($1), GetToken($3));
  $$=PutToken(tmp);
}
| expr_decl '-' expr_decl
{
  sprintf(tmp,"%s - %s",GetToken($1), GetToken($3));
  $$=PutToken(tmp);
}
| expr_decl '*' expr_decl
{
  sprintf(tmp,"%s * %s",GetToken($1), GetToken($3));
  $$=PutToken(tmp);
}
| expr_decl '/' expr_decl
{
  sprintf(tmp,"%s / %s",GetToken($1), GetToken($3));
  $$=PutToken(tmp);
}
| expr_decl '%' expr_decl
{
  sprintf(tmp,"%s %% %s",GetToken($1), GetToken($3));
  $$=PutToken(tmp);
}
| expr_decl '&' expr_decl
{
  sprintf(tmp,"%s & %s",GetToken($1), GetToken($3));
  $$=PutToken(tmp);
}
| expr_decl '|' expr_decl
{
  sprintf(tmp,"%s | %s",GetToken($1), GetToken($3));
  $$=PutToken(tmp);
}
| expr_decl '^' expr_decl
{
  sprintf(tmp,"%s ^ %s",GetToken($1), GetToken($3));
  $$=PutToken(tmp);
}
| expr_decl AND_OP expr_decl
{
  sprintf(tmp,"%s && %s",GetToken($1), GetToken($3));
  $$=PutToken(tmp);
}
| expr_decl OR_OP expr_decl
{
  sprintf(tmp,"%s || %s",GetToken($1), GetToken($3));
  $$=PutToken(tmp);
}
| expr_decl EQUAL_OP expr_decl
{
  sprintf(tmp,"%s == %s",GetToken($1), GetToken($3));
  $$=PutToken(tmp);
}
| expr_decl '=' expr_decl
{
  sprintf(tmp,"%s=%s",GetToken($1), GetToken($3));
  $$=PutToken(tmp);
}
| expr_decl '?' expr_decl ':' expr_decl
{
  sprintf(tmp,"%s ? %s : %s",GetToken($1), GetToken($3), GetToken($5));
  $$=PutToken(tmp);
}
| expr_decl NOTEQUAL_OP expr_decl
{
  sprintf(tmp,"%s != %s",GetToken($1), GetToken($3));
  $$=PutToken(tmp);
}
| expr_decl '>' expr_decl
{
  sprintf(tmp,"%s>%s",GetToken($1), GetToken($3));
  $$=PutToken(tmp);
}
| expr_decl '<' expr_decl
{
  sprintf(tmp,"%s<%s",GetToken($1), GetToken($3));
  $$=PutToken(tmp);
}
| expr_decl GREATEREQUAL_OP expr_decl
{
  sprintf(tmp,"%s>=%s",GetToken($1), GetToken($3));
  $$=PutToken(tmp);
}
| expr_decl LESSEQUAL_OP expr_decl
{
  sprintf(tmp,"%s<=%s",GetToken($1), GetToken($3));
  $$=PutToken(tmp);
}
|  '-' expr_decl %prec UMINUS
{
  sprintf(tmp,"-%s",GetToken($2));
  $$=PutToken(tmp);
}
| '(' expr_decl ')'
{
  sprintf(tmp,"(%s)",GetToken($2));
  $$=PutToken(tmp);
}
| expr_name '(' ')'
{
  sprintf(tmp,"%s()",GetToken($1));
  $$=PutToken(tmp);
}
| expr_name '(' expr_list ')'
{
  sprintf(tmp,"%s(%s)",GetToken($1),GetToken($3));
  $$=PutToken(tmp);
 
}
| '*' expr_decl
{
  sprintf(tmp,"*%s",GetToken($2));
  $$=PutToken(tmp);
} 
| '&' expr_decl
{
  sprintf(tmp,"&%s",GetToken($2));
  $$=PutToken(tmp);
}
| '!' expr_decl
{
  sprintf(tmp,"!%s",GetToken($2));
  $$=PutToken(tmp);
}
| '~' expr_decl
{
  sprintf(tmp,"~%s",GetToken($2));
  $$=PutToken(tmp);
}
;

number: INTEGER
{
  $$=$1;
}
| REAL
{
  $$=$1;
}
;

expr_name: ID
{
  $$=$1;
} 
| ID SCOPE ID
{
  sprintf(tmp,"%s::%s",GetToken($1),GetToken($3));
  $$=PutToken(tmp);  
}
;


expr_list:  expr_decl
{
  $$=$1;
}
| expr_list ',' expr_decl
{
  sprintf(tmp,"%s, %s", GetToken($1),GetToken($3));
  $$=PutToken(tmp);
}
;



%%

#include "config.h"

void Usage()
{
  fprintf(stderr,"POP-C++ preprocessor version %s\nparoccpp [-onlyclient | -onlyserver] [POP-C++ source] [C++ source]\n", VERSION);
  exit(1);
}

void CleanStack()
{
  if (typestack.GetCount()) fprintf(stderr,"STRUCT list: %d elements\n",typestack.GetCount());
  currentstruct=NULL;
  structContainer=NULL;
  typestack.RemoveAll();
}

void Push(TypeClassStruct *x)
{
  typestack.AddHead(x);
}

TypeClassStruct *Pop()
{
  POSITION pos=typestack.GetHeadPosition();
  if (pos==NULL) return NULL;
  TypeClassStruct *t=typestack.GetAt(pos);
  typestack.RemoveHead();
  return t;
}

TypeClassStruct *Peek()
{
  POSITION pos=typestack.GetHeadPosition();
  if (pos==NULL) return NULL;
  TypeClassStruct *t=typestack.GetAt(pos);
  return t;
}

// Update marshall options for a specific parameter
void UpdateMarshalParam(int flags, Param *t)
{
	if (flags!=0) {
		if (flags & PARAM_IN) t->isInput=true;
		if (flags & PARAM_OUT) t->isOutput=true;
      if (flags & USERPROC) t->marshalProc=strdup(tmpProc);
      if (flags & PARAMSIZE) t->paramSize=strdup(tmpSize);
	}
}

extern FILE *yyin;

int main(int argc, char **argv)
{
	bool client=true;
	bool broker=true;
	isWarningEnable=true;
	isImplicitPackEnable=false;
	isPOPCPPCompilation=false;
	isAsyncAllocationDisable=false;

	if (paroc_utils::checkremove(&argc,&argv,"-parclass-nobroker")!=NULL){
		
		broker=false;
	
	} 	
	
	
	if (paroc_utils::checkremove(&argc,&argv,"-parclass-nointerface")!=NULL) {
		
		client=false;
	
	}  
	
	if (paroc_utils::checkremove(&argc,&argv,"-no-warning")!=NULL) {
		
		isWarningEnable=false;
	
	}  

	if (paroc_utils::checkremove(&argc,&argv,"-popcpp-compilation")!=NULL) {
		
		isPOPCPPCompilation=true;
	
	}  
	
	if (paroc_utils::checkremove(&argc,&argv,"-no-implicit-pack")!=NULL) {

		isImplicitPackEnable=false;
	
	}
	if (paroc_utils::checkremove(&argc,&argv,"-no-async-allocation")!=NULL) {
		isAsyncAllocationDisable=true;
	
	}	

	int ret;
	indexsource=-1;
	errorcode=0;
	if (argc<2){
		
		Usage();
	
	} else {
		if ((ret=ParseFile(argv[1], ((argc>2) ? argv[2] : NULL), client, broker, isWarningEnable, isImplicitPackEnable, isPOPCPPCompilation, isAsyncAllocationDisable))!=0)	{
			fprintf(stderr,"Parse POP-C++ code failed (error=%d)\n",ret);
			exit(ret);
		}
	}
	return (errorcode!=0);
}

void yyerror(const  char *s)
{
  if (insideClass)
    {
      errormsg(s);
    }
}

void errormsg(const  char *s)
{
  fprintf(stderr,"%s:%d: ERROR: %s\n",filename,linenumber-1,s);
  errorcode=1;
}

int yywrap() {
	return(1);
}


int base=1;

int ParseFile(char *infile, char *outfile, bool client, bool broker, bool isWarningEnable, bool isImplicitPackEnable, bool isPOPCPPCompilation, bool isAsyncAllocationDisable)
{
	if (infile==NULL || *infile=='-'){
		yyin=stdin;
	} else {
      yyin=fopen(infile,"rt");
      if (yyin==NULL) {
	   	perror(infile);
	   	return errno;
		}
      strcpy(filename,infile);
	}
	
	linenumber=1;
	thisCodeFile=new CodeFile(NULL);
	if (outfile!=NULL) {
		
		thisCodeFile->SetOutputName(outfile);	

	}
	

	// Set the code file as core compilation if the compilation flag is present.
	if(isPOPCPPCompilation)
		thisCodeFile->SetAsCoreCompilation();

	// Disable asynchronous parallel object allocation if the compilation flag is present.
	if(isAsyncAllocationDisable)	
		thisCodeFile->DisableAsyncAllocation();	
		
	insideClass=false;
	othercodes.SetSize(0);
	startPos=-1;
	

	int ret=yyparse();
	if (ret==0) {
		FILE *outf;
      if (outfile==NULL || *outfile=='-'){
     		
      	outf=stdout;
      
      } else {
			outf=fopen(outfile,"wt");
	  		if (outf==NULL){
		      ret=errno;
		      perror(outfile);
	   	}
		}
      if (outf!=NULL) {
			CArrayChar output(0,32000);
			thisCodeFile->GenerateCode(output, client, broker);
			fwrite((char *)output,1, output.GetSize(),outf);
      }
      if (outf!=stdout) {
			fclose(outf);
      
		}
	}
  
	othercodes.SetSize(0);

	if (yyin!=stdin) {
		
		fclose(yyin);
	
	}
	delete thisCodeFile;
	thisCodeFile=NULL;
	
	isParclassDeclared = false; 
	
	return ret;
	
}


/* ---- SET normal C++ function return attributes ----- */
void setReturnParam(int pointer, int ref, int const_virtual)
{
	//Old data:  argument_type function_name 
	strcpy(method->returnparam.name,"_RemoteRet");

	DataType *type=returntype;
	if (pointer>0)
	{
		//type=new TypePtr(NULL, pointer, type);
		type=new TypePtr(NULL, pointer, type, constPointerPositions);
		thisCodeFile->AddDataType(type);
		
		constPointerPositions.clear(); // empty used struct
	}

	if (ref)
	{
		method->returnparam.isRef=true;
	}
	
	method->isVirtual=((const_virtual & 1)!=0);
	method->returnparam.isConst = ((const_virtual & 2)!=0);
	
	method->returnparam.SetType(type);
	
}

/* ---- POPC methode attributes ---- */
void setPOPCMethodeModifier(int settings)
{
	
	// TEST Mutex, prallel, seq or CONC (hinden ??)
	if ((settings & 56)==8) 
		method->isConcurrent=true;
	else if ((settings & 56)==32) 
		method->isConcurrent=false;
	else if ((settings & 56)==16)
		method->isMutex= true;
	else if ((settings & 56)!=0)
	{
		errormsg("Multiple seq, conc or mutex keyword");
		exit(1);
	}
	
	if ((settings & 64)!=0)
		method->isHidden=true;
	
	// TEST SYNC or ASYNC
	if ((settings & 6)==6) 
	{
		errormsg("Multiple sync, async keyword");
		exit(1);
	}
	else if ((settings & 6)==4) method->invoketype=invokeasync;
	else if ((settings & 6)==2) method->invoketype=invokesync;
	//else method->invoketype=autoselect;
}

void errorGlobalMehtode(bool isGlobal)
{
	if(isGlobal)
	{
		errormsg("inspectors/const member functions are supported in the current version of POP-C++");
		exit(1);
	}
	else
	{
		return;
	}
}

