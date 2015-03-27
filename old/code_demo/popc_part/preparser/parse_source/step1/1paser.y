%{
	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	#include <vector>
	
	#include <json/json.h>
	#include <iostream>
	#include <fstream>

	int yylex(void);
	void yyerror(char *);
	extern FILE* yyin; 
	extern FILE* yyout; 

	using namespace std;

	std::ofstream fname;

	std::ofstream oofs;
	Json::Value substitution;
	Json::Value& sub_value = substitution["ParameterElement"];
	
	Json::FastWriter writer;
	std::string json_append_file;
	std::string sub_append_file;

	//global parameter
	int partmpTag=0;
	vector <string> paramList;
%}
	%union{
		char* str;
		int num;
	}
	%token TEMP
	%token END INCLUDE 


	%left DAL 
	%right DAR 
	
	%token VEC
	%token WELL
	
	%type <str> program express keyword template ind indparam otherdef vect1 addition
	%token <str> ID	RT INSTR OPERATOR SYMBOL SPKEYWORD ALR DECPOINT
	%token <num> DIGIT
%%
	program: express program	{}
	| express			{}
	|				{}
	;	

	express: keyword express	{}
	| ID ALR			{fprintf(yyout,"%s%s",$1,$2);
					printf("id alr: %s %s\n", $1,$2);}
	| ID ALR ID			{fprintf(yyout,"%s%s%s",$1,$2,$3);
					printf("id alr id: %s %s %s\n", $1,$2,$3);}
	| ID				{fprintf(yyout," %s",$1);
					printf("id: %s \n", $1);}
	| ALR				{fprintf(yyout," %s",$1);
					printf("ALR: %s \n", $1);}
	| DIGIT				{fprintf(yyout," %d",$1);
					printf("digit: %d \n", $1);}
	| DECPOINT			{fprintf(yyout,"%s",$1);
					printf("decimal point: %s \n", $1);}
	| OPERATOR			{fprintf(yyout,"%s",$1);
					printf("operator: %s \n", $1);}
	| SYMBOL			{fprintf(yyout,"%s",$1);
					printf("symbol: %s \n", $1);}
	| SPKEYWORD			{fprintf(yyout,"%s",$1);
					printf("keyword: %s \n", $1);}
	| END				{fprintf(yyout,"\n");}
	;

	keyword:
	template 		{}
	| vect1			{}
	| INSTR			{fprintf(yyout,"%s",$1);}
	| DAL			{fprintf(yyout,"<<");}
	| DAR			{fprintf(yyout,">>");}
	;

	vect1:
	VEC ALR vect2		{}
	| VEC ALR ID ALR		{fprintf(yyout," vector <%s>",$3);}
	;
	
	vect2:
	VEC ALR ID ALR ALR		{fprintf(yyout," vector < vector <%s> >",$3);}
	;
	
	template:
	ind indparam		{}	
	| TEMP addition		{paramList.clear();}
	| ID ALR ID ALR 	{fprintf(yyout," %s%s%s%s",$1,$2,$3,$4);}
	;

	addition:
	tmpparam addition		{}
	| tmpparam classdec	        {}
	;

	tmpparam:
	  ALR ID ID ALR		{ 
					fprintf(yyout,"template %s %s %s %s ",$1,$2,$3,$4);
					string tmpParam=$3;
				 	paramList.push_back(tmpParam);
				}
	| ALR ID ID SYMBOL    	{ 
					fprintf(yyout,"template %s %s %s %s ",$1,$2,$3,$4);
					string tmpParam=$3;
					paramList.push_back(tmpParam);
				}
	| ID ID SYMBOL		{ 
					fprintf(yyout,"%s %s %s ",$1,$2,$3);
					string tmpParam=$2;
					paramList.push_back(tmpParam);
				}
	| ID ID ALR		{ 
					fprintf(yyout,"%s %s %s ",$1,$2,$3);
					string tmpParam=$2;
					paramList.push_back(tmpParam);
				}
	;
	
	classdec:
	typedec paramdec classdec	{}
	| paramdec			{} 		
	;
	
	typedec:
	ID ID			{	// class name is unique
					fprintf(yyout,"%s %s",$1,$2);
					// record in the file
					string curClassType=$1;
					if(curClassType=="parclass")  
					{
						printf("parclass located! %s",$2);
						partmpTag=1;

						Json::Value addtion;
						addtion["orName"]=$2;
						
						Json::Value& paramLT=addtion["orParameters"];
						for(int i=0;i<paramList.size();++i)
						{
							Json::Value tmp=paramList[i];
							paramLT.append(tmp);
						}						
						sub_value.append(addtion);
		
						sub_append_file.clear();					
						sub_append_file = writer.write(substitution);
					}
				}
						
	;
	
	paramdec:
	ALR ID ALR		{fprintf(yyout," %s %s %s",$1,$2,$3);}
	| ALR DIGIT ALR		{fprintf(yyout," %s %d %s",$1,$2,$3);}
	| ALR ID SYMBOL		{fprintf(yyout," %s %s %s",$1,$2,$3);}
	| ALR DIGIT SYMBOL	{fprintf(yyout," %s %d %s",$1,$2,$3);}
	| ID SYMBOL		{fprintf(yyout," %s %s",$1,$2);}
	| DIGIT SYMBOL		{fprintf(yyout," %d %s",$1,$2);}
	| ID ALR		{fprintf(yyout," %s %s",$1,$2);}
	| DIGIT ALR		{fprintf(yyout," %d %s",$1,$2);}
	| 
	;

	
	ind:
	INCLUDE			{fprintf(yyout,"#include ");}
	| WELL ID		{fprintf(yyout,"#%s ",$2);}
	;
	
	indparam:
	ALR ID ALR		{fprintf(yyout,"<%s>",$2);}
	| ALR OPERATOR ALR	{fprintf(yyout,"<%s>",$2);}
	| ALR ID SYMBOL OPERATOR ALR	{fprintf(yyout,"<%s%s%s>",$2,$3,$4);}
	| ALR VEC ALR		{fprintf(yyout,"<vector>");}
	| INSTR			{fprintf(yyout,"%s",$1);}
	| otherdef		{}
	;

	otherdef:
	ID			{fprintf(yyout,"%s",$1);}
	| END			{fprintf(yyout,"\n");} 
	;


%%
	int yywrap()
	{
    return 1;
	}	
	void yyerror(char *s) {
	fprintf(stderr, "%s\n", s);
	}
	int main(int argv, char** argc) {
	
	const char* sFile=argc[1];
	FILE* infp=fopen(sFile, "r"); 
	
	if(infp==NULL)
	{
		printf("can not open %s \n",sFile);
		return -1;
	}
	

	yyin=infp;

	string fileName=argc[1];
	int index=fileName.find('.',0);
	printf("index : %d\n",index);
	string behind=fileName.substr(index,fileName.size());
	string front=fileName.substr(0,index);
	char num='1';
	string whole=front+"_"+num+behind;

	
	FILE* outfp=fopen(whole.c_str(), "w"); 
	
	if(outfp==NULL)
	{
		printf("can not open outfile \n");
		return -1;
	}
	
	  
	yyout=outfp;
	
	//ofs.open("info.json",ios::app);
	oofs.open("substitution.json",ios::app);
	fname.open("filename.txt",ios::app);

	yyparse();
	//ofs << json_append_file;
	oofs << sub_append_file;
	fname<<fileName<<endl;
	
	fclose(infp);
	fclose(outfp);
	//ofs.close();
	oofs.close();
	fname.close();

	return 0;
	}
