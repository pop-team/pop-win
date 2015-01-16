%{
	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	
	#include <json/json.h>
	#include <iostream>
	#include <fstream>

	#include <vector>
	using namespace std;

	int yylex(void);
	void yyerror(char *);
	extern FILE* yyin; 
	extern FILE* yyout; 
	//	
	//
	typedef struct tmpInfo
	{
		vector <string> dType;
		vector <string> nType;
		std::string dName;
		std::string nName;
		int index;
	}tpInfo;

	vector <tpInfo> tplist;  //for one type of template


	//std::ofstream outCheck;
	std::ifstream fname;	
	//

	std::ifstream ifs;
	Json::Reader root_reader;
	Json::Value root;
	

	std::ifstream iifs;
	Json::Reader sub_reader;
	Json::Value substitution;

	Json::FastWriter writer;
	std::string json_append_file;

	int tmpdecTag=0; //template declaration tag
	int inTag=0;	// in brace tag
	
	
	string curTemplateName;
	string code[5]; //predefine

	//new modification
	string appendition;

	string ParamList;
	
	
%}
	%union{
		char* str;
		int num;
	}
	%token TEMP
	%token END INCLUDE 

	%left DAL LBRACE
	%right DAR RBRACE
	
	%token VEC
	%token WELL
	
	%type <str> program express keyword template ind indparam otherdef vect1 addition
	%token <str> ID	RT INSTR OPERATOR SYMBOL SPKEYWORD ALR PACK UID COMMA DECPOINT
	%token <num> DIGIT
%%
	program: express program	{}
	| express			{}
	|				{}
	;	

	express: keyword express	{}
	| ID ALR			{
						string curID=$1;
						int subTag=0;
						int paramIndex;
						if((tmpdecTag!=0)&&(curTemplateName==tplist[0].dName))
						{	//locate the index of corresponding parameters in the tplist
							vector <string> tnt=tplist[0].dType;
							for (int j=0; j < tnt.size(); ++j)
							{
								if(curID==tnt[j])
								{
									subTag=1;	//parameter matched
									paramIndex=j;	//index in dType list
									break;	
								}
							}
							//substittuion
							
							for (int i=0; i < tplist.size(); ++i)
							{
								if (subTag==1)	//parameter substitution
								{
									code[i]=code[i]+" "+tplist[i].nType[paramIndex]+$2;
								}
								else //subTag=0, no substitution
								{
									code[i]=code[i]+" "+curID+$2;
								}
							}
						}
						else 
						{
							fprintf(yyout," %s %s",$1,$2);
							printf("id alr: %s %s\n", $1,$2);
						}
					}
	| ID COMMA			{
						string curID=$1;
						int subTag=0;
						int paramIndex;
						if((tmpdecTag!=0)&&(curTemplateName==tplist[0].dName))
						{	//locate the index of corresponding parameters in the tplist
							vector <string> tnt=tplist[0].dType;
							for (int j=0; j < tnt.size(); ++j)
							{
								if(curID==tnt[j])
								{
									subTag=1;	//parameter matched
									paramIndex=j;	//index in dType list
									break;	
								}
							}
							//substittuion
							
							for (int i=0; i < tplist.size(); ++i)
							{
								if (subTag==1)	//parameter substitution
								{
									code[i]=code[i]+" "+tplist[i].nType[paramIndex]+$2;
								}
								else //subTag=0, no substitution
								{
									code[i]=code[i]+" "+curID+$2;
								}
							}
						}
						else 
						{
							fprintf(yyout," %s %s",$1,$2);
							printf("id comma: %s %s\n", $1,$2);
						}
					}
	| ID				{
						printf("id: %s \n", $1);
						string curID=$1;
						int typeTag=0;
						int nameTag=0;
						int paramIndex;

						if(tmpdecTag!=0)
						{
							vector <string> tnt=tplist[0].dType;
							printf("tnt:%s \n",tnt[0].c_str());
							printf("curID:%s \n",curID.c_str());
							for (int j=0; j < tnt.size(); ++j)
							{
								if(curID==tnt[j])
								{
									typeTag=1;	//parameter matched
									paramIndex=j;	//index in dType list
									printf("paramIndex:%d \n",paramIndex);
									break;	
								}
							}
							// parameter substittuion
							for (int i=0; i < tplist.size(); ++i)
							{
								if (typeTag==1)	//parameter substitution
								{
									code[i]=code[i]+" "+tplist[i].nType[paramIndex];
								}
							}

							for (int i=0; i < tplist.size(); ++i)
							{
								if (tplist[i].dName==curID) //name substitution happen
								{
									nameTag=1;
									code[i]=code[i]+" "+tplist[i].nName;
								}
							}
							for (int i=0; i < tplist.size(); ++i)
							{
								if ((typeTag==0)&&(nameTag==0)) //type and name substitution not happen
								{
									code[i]=code[i]+" "+curID;
								}
							}
						}
						else fprintf(yyout," %s",$1);
						
					}

		
	| DECPOINT			{
						printf("decimal point: %s \n", $1);
						if(tmpdecTag!=0)
						{	for (int i=0; i < tplist.size(); ++i)
							{
							code[i]=code[i]+$1;
							}
						}
						else fprintf(yyout,"%s",$1);
					}
	| COMMA				{
						printf("comma: %s \n", $1);
						if(tmpdecTag!=0)
						{	for (int i=0; i < tplist.size(); ++i)
							{
							code[i]=code[i]+$1;
							}
						}
						else fprintf(yyout,"%s",$1);
					}
	| LBRACE 			{	
						printf("lbrace: { \n");
				 		if(tmpdecTag!=0)
						{
							inTag=inTag+1;
							printf("located template in brace \n");
							for (int i=0; i < tplist.size(); ++i)
							{
								code[i]=code[i]+"{";
							}
						}
						else fprintf(yyout,"{ ");
					}
	| RBRACE 			{
						
						printf("rbrace: } \n");

						if(tmpdecTag!=0)
						{
							inTag=inTag-1;
							if (inTag==0)	//out of template
							{
								tmpdecTag=0;
								
								printf("located template out brace \n");
								for (int i=0; i < tplist.size(); ++i)
								{
									code[i]=code[i]+"} \n";
									fprintf(yyout," %s",code[i].c_str());
									code[i].clear();
								}
								curTemplateName.clear();  //end of implementation of template					
							}
							else 
							{
								for (int i=0; i < tplist.size(); ++i)
								{
									code[i]=code[i]+"} ";
								}
							}
							
						}
						else fprintf(yyout,"} \n");
						printf("current tmp and intag: %d , %d \n",tmpdecTag, inTag);
					}
	| RBRACE SYMBOL			{
						printf("rbrace: }%s \n",$2);
						string sym=$2;
						if(tmpdecTag!=0)
						{
							inTag=inTag-1;
							if (inTag==0)  //out of template
							{
								tmpdecTag=0;
								printf("located template out brace \n");
								for (int i=0; i < tplist.size(); ++i)
								{
									code[i]=code[i]+"}"+sym+"\n";
									fprintf(yyout," %s",code[i].c_str());
									code[i].clear();
								}
								curTemplateName.clear();
							
							}
							else 
							{
								for (int i=0; i < tplist.size(); ++i)
								{
									code[i]=code[i]+"}"+sym;
								}
							}
						}
						else fprintf(yyout," }%s \n",$2);
						printf("current tmp and intag: %d , %d \n",tmpdecTag, inTag);
					}
	| OPERATOR			{	
						printf("operator: %s \n", $1);
						if((tmpdecTag!=0)&&(curTemplateName==tplist[0].dName))
						{	for (int i=0; i < tplist.size(); ++i)
							{
							code[i]=code[i]+$1;
							}
						}
						else fprintf(yyout," %s",$1);
					}
	| SYMBOL			{
						printf("symbol: %s \n", $1);
						if((tmpdecTag!=0)&&(curTemplateName==tplist[0].dName))
						{	for (int i=0; i < tplist.size(); ++i)
							{
							code[i]=code[i]+$1;
							}
						}
						else fprintf(yyout,"%s",$1);
					}
	| SPKEYWORD			{
						printf("spkeyword: %s \n", $1);
						string curWord=$1;
						string symb=curWord.substr(0,1);
						string curID=curWord.substr(1,curWord.size());
						printf("subword: %s, symbol: %s \n",curID.c_str(),symb.c_str());
						int typeTag=0;
						int nameTag=0;
						int paramIndex;

						if(tmpdecTag!=0)
						{
							vector <string> tnt=tplist[0].dType;
							for (int j=0; j < tnt.size(); ++j)
							{
								if(curID==tnt[j])
								{
									typeTag=1;	//parameter matched
									paramIndex=j;	//index in dType list
									break;	
								}
							}
							// parameter substittuion
							for (int i=0; i < tplist.size(); ++i)
							{
								if (typeTag==1)	//parameter substitution
								{
									code[i]=code[i]+" "+symb+tplist[i].nType[paramIndex];
								}
							}

							for (int i=0; i < tplist.size(); ++i)
							{
								if (tplist[i].dName==curID) //name substitution happen
								{
									nameTag=1;
									code[i]=code[i]+" "+symb+tplist[i].nName;
								}
							}
							for (int i=0; i < tplist.size(); ++i)
							{
								if ((typeTag==0)&&(nameTag==0)) //type and name substitution not happen
								{
									code[i]=code[i]+" "+symb+curID;
								}
							}
						}
						else fprintf(yyout," %s",$1);
					}
	| END				{
						if(tmpdecTag!=0)
						{	for (int i=0; i < tplist.size(); ++i)
							{
								code[i]=code[i]+" \n";
							}
						}
						else fprintf(yyout,"\n");
					}
	;

	keyword:
	template 		{}
	| UID DIGIT SYMBOL	{	//inside the template
					int digital=$2;
					printf("classuid located! %d \n",$2);
					if(tmpdecTag!=0)
					{	for (int i=0; i < tplist.size(); ++i)
						{
							char tmpDig[4];
							sprintf(tmpDig,"%d",digital);
							digital++;
							code[i]=code[i]+" "+$1+tmpDig+$3;
						}
					}
					else fprintf(yyout," %s%d%s",$1,$2,$3);
				}
	| PACK packFollow 	{	//outside the template
				}
	| VEC vect1		{}
	| DIGIT			{
					char tmpp[4];
					sprintf(tmpp,"%d",$1);
					if((tmpdecTag!=0)&&(curTemplateName==tplist[0].dName))
					{	for (int i=0; i < tplist.size(); ++i)
						{
						code[i]=code[i]+" "+tmpp;
						}
					}
					else fprintf(yyout," %d", $1);
				}
	| INSTR			{
					if((tmpdecTag!=0)&&(curTemplateName==tplist[0].dName))
					{	for (int i=0; i < tplist.size(); ++i)
						{
						code[i]=code[i]+" "+$1;
						}
					}
					else fprintf(yyout,"%s",$1);
				}
	| DAL			{
					if((tmpdecTag!=0)&&(curTemplateName==tplist[0].dName))
					{	for (int i=0; i < tplist.size(); ++i)
						{
						code[i]=code[i]+"<<";
						}
					}
					else fprintf(yyout,"<<");
				}
	| DAR			{
					if((tmpdecTag!=0)&&(curTemplateName==tplist[0].dName))
					{	for (int i=0; i < tplist.size(); ++i)
						{
						code[i]=code[i]+">>";
						}
					}
					else fprintf(yyout,">>");
				}
	;

	packFollow:
	frontPack packFollow		{}
	|				{}
	;
	
	frontPack:
	SYMBOL ID		{
					string curID=$2;
					printf("located pack name: %s\n",$2);
					int firstTag=0;
					Json::Value com_value=root["ArgumentElement"];  //info.json
					vector <string> newNameList;
					for (int i=0;i<com_value.size();++i)
					{
						Json::Value thisValue=com_value[i];
						if (thisValue["orName"]==curID)
						{
							newNameList.push_back(thisValue["newName"].asString());
						}
					}
					if (!newNameList.empty())
					{
						firstTag=1;
						fprintf(yyout,"pack%s",$1);
						fprintf(yyout,"%s",newNameList[0].c_str());
						for (int j=1;j<newNameList.size();++j)
						{
							fprintf(yyout,",%s",newNameList[j].c_str());
						}
						// end appending new name 	
					}
					if (firstTag==0)
					{
						fprintf(yyout,"pack%s %s",$1,$2);
					}
				}
	| COMMA ID		{
					string curID=$2;
					printf("inside pack: %s\n",$2);
					int firstTag=0;
					Json::Value com_value=root["ArgumentElement"];  //info.json
					vector <string> newNameList;
					for (int i=0;i<com_value.size();++i)
					{
						Json::Value thisValue=com_value[i];
						if (thisValue["orName"]==curID)
						{
							newNameList.push_back(thisValue["newName"].asString());
						}
					}
					if (!newNameList.empty())
					{
						firstTag=1;
						fprintf(yyout,"%s",$1);
						fprintf(yyout,"%s",newNameList[0].c_str());
						for (int j=1;j<newNameList.size();++j)
						{
							fprintf(yyout,",%s",newNameList[j].c_str());
						}
						// end appending new name 	
					}
					if (firstTag==0)
					{
						fprintf(yyout,"%s %s",$1,$2);
					}
				}
	| SYMBOL		{
					fprintf(yyout,"%s ",$1);
				}
	;

	vect1:
	ALR VEC ALR ID ALR ALR	{
					string cutType=$4;
					int paramIndex;
					int subTag=0;
					if((tmpdecTag!=0)&&(curTemplateName==tplist[0].dName))
					{	
						vector <string> tnt=tplist[0].dType;
						for (int j=0; j < tnt.size(); ++j)
						{
							if(cutType==tnt[j])
							{
								subTag=1;	//parameter matched
								paramIndex=j;	//index in dType list
								break;	
							}
						}
						//substittuion
							
						for (int i=0; i < tplist.size(); ++i)
						{
							if (subTag==1)	//parameter substitution
							{
								code[i]=code[i]+"vector < vector <"+tplist[i].nType[paramIndex]+"> >";
							}
							else //subTag=0, no substitution
							{
								code[i]=code[i]+"vector < vector <"+cutType+"> >";
							}
						}
					}
					else fprintf(yyout,"vector < vector <%s> >",$4);
				}
	| ALR ID ALR		{
					string cutType=$2;
					int paramIndex;
					int subTag=0;
					if((tmpdecTag!=0)&&(curTemplateName==tplist[0].dName))
					{	
						vector <string> tnt=tplist[0].dType;
						for (int j=0; j < tnt.size(); ++j)
						{
							if(cutType==tnt[j])
							{
								subTag=1;	//parameter matched
								paramIndex=j;	//index in dType list
								break;	
							}
						}
						//substittuion
							
						for (int i=0; i < tplist.size(); ++i)
						{
							if (subTag==1)	//parameter substitution
							{
								code[i]=code[i]+"vector <"+tplist[i].nType[paramIndex]+">";
							}
							else //subTag=0, no substitution
							{
								code[i]=code[i]+"vector < "+cutType+"> ";
							}
						}
					}
					else fprintf(yyout,"vector <%s>",$2);
				}
	;
	
	template:
	ind indparam		{}	
	| TEMP addition		{	printf("templated located !\n");
				}
	| instance		{} //instantiation of template
	;
						//substitution happen inside the template
	instance:
	ID ALR ID ALR ID		{	//only rename the class name
						printf(" other template instantiation located : %s \n",$1);
						string curID=$1;						
						int typeTag=0;
						int nameTag=0;
						int paramIndex;


						string scurID=$3;
						int stypeTag=0;
						int snameTag=0;
						int sparamIndex;

						if(tmpdecTag!=0)
						{
							vector <string> tnt=tplist[0].dType;
							for (int j=0; j < tnt.size(); ++j)
							{
								if(curID==tnt[j])
								{
									typeTag=1;	//parameter matched
									paramIndex=j;	//index in dType list
									break;	
								}
							}
							// parameter substittuion
							for (int i=0; i < tplist.size(); ++i)
							{
								if (typeTag==1)	//parameter substitution
								{
									code[i]=code[i]+" "+tplist[i].nType[paramIndex];
								}
							}

							for (int i=0; i < tplist.size(); ++i)
							{
								if (tplist[i].dName==curID) //name substitution happen
								{
									nameTag=1;
									code[i]=code[i]+" "+tplist[i].nName;
								}
							}
							for (int i=0; i < tplist.size(); ++i)
							{
								if ((typeTag==0)&&(nameTag==0)) //type and name substitution not happen
								{
									//code[i]=code[i]+" "+curID+" "+$2+" "+$3+" "+$4+" "+$5;
									code[i]=code[i]+" "+curID+" "+$2;
								}
								//else code[i]=code[i]+" "+$2+" "+$3+" "+$4+" "+$5;
								else code[i]=code[i]+" "+$2;
							}
							
							//start second parameter
							for (int j=0; j < tnt.size(); ++j)
							{
								if(scurID==tnt[j])
								{
									stypeTag=1;	//parameter matched
									sparamIndex=j;	//index in dType list
									break;	
								}
							}
							for (int i=0; i < tplist.size(); ++i)
							{
								if (stypeTag==1)	//parameter substitution
								{
									code[i]=code[i]+" "+tplist[i].nType[paramIndex];
								}
							}
							for (int i=0; i < tplist.size(); ++i)
							{
								if (tplist[i].dName==scurID) //name substitution happen
								{
									snameTag=1;
									code[i]=code[i]+" "+tplist[i].nName;
								}
							}
							for (int i=0; i < tplist.size(); ++i)
							{
								if ((stypeTag==0)&&(snameTag==0)) //type and name substitution not happen
								{
									//code[i]=code[i]+" "+curID+" "+$2+" "+$3+" "+$4+" "+$5;
									code[i]=code[i]+" "+scurID+" "+$4+" "+$5;
								}
								//else code[i]=code[i]+" "+$2+" "+$3+" "+$4+" "+$5;
								else code[i]=code[i]+" "+$4+" "+$5;
							}
						}
						else fprintf(yyout," %s %s %s %s %s",$1,$2,$3,$4,$5);
					}
	| ID ALR ID			{	//only rename the class name
						printf(" other template instantiation located : %s \n",$1);
						string curID=$1;
						int typeTag=0;
						int nameTag=0;
						int paramIndex;

						string scurID=$3;
						int stypeTag=0;
						int snameTag=0;
						int sparamIndex;

						if(tmpdecTag!=0)
						{
							vector <string> tnt=tplist[0].dType;
							for (int j=0; j < tnt.size(); ++j)
							{
								if(curID==tnt[j])
								{
									typeTag=1;	//parameter matched
									paramIndex=j;	//index in dType list
									break;	
								}
							}
							// parameter substittuion
							for (int i=0; i < tplist.size(); ++i)
							{
								if (typeTag==1)	//parameter substitution
								{
									code[i]=code[i]+" "+tplist[i].nType[paramIndex];
								}
							}

							for (int i=0; i < tplist.size(); ++i)
							{
								if (tplist[i].dName==curID) //name substitution happen
								{
									nameTag=1;
									code[i]=code[i]+" "+tplist[i].nName;
								}
							}
							for (int i=0; i < tplist.size(); ++i)
							{
								if ((typeTag==0)&&(nameTag==0)) //type and name substitution not happen
								{
									//code[i]=code[i]+" "+curID+" "+$2+" "+$3;
									code[i]=code[i]+" "+curID+" "+$2;
								}
								//else code[i]=code[i]+" "+$2+" "+$3;
								else code[i]=code[i]+" "+$2;
							}

							//start second parameters
							for (int j=0; j < tnt.size(); ++j)
							{
								if(scurID==tnt[j])
								{
									stypeTag=1;	//parameter matched
									sparamIndex=j;	//index in dType list
									break;	
								}
							}
							for (int i=0; i < tplist.size(); ++i)
							{
								if (stypeTag==1)	//parameter substitution
								{
									code[i]=code[i]+" "+tplist[i].nType[paramIndex];
								}
							}
							for (int i=0; i < tplist.size(); ++i)
							{
								if (tplist[i].dName==scurID) //name substitution happen
								{
									snameTag=1;
									code[i]=code[i]+" "+tplist[i].nName;
								}
							}
							for (int i=0; i < tplist.size(); ++i)
							{
								if ((stypeTag==0)&&(snameTag==0)) //type and name substitution not happen
								{
									//code[i]=code[i]+" "+curID+" "+$2+" "+$3;
									code[i]=code[i]+" "+scurID;
								}
								//else code[i]=code[i]+" "+$2+" "+$3;
								else code[i]=code[i];
							}

						}
						else fprintf(yyout," %s %s %s",$1,$2,$3);			
					}

	| COMMA ID			{	
						printf(" comma id in arguments : %s \n",$2);
						string curID=$2;
						int typeTag=0;
						int nameTag=0;
						int paramIndex;

						if(tmpdecTag!=0)
						{
							vector <string> tnt=tplist[0].dType;
							for (int j=0; j < tnt.size(); ++j)
							{
								if(curID==tnt[j])
								{
									typeTag=1;	//parameter matched
									paramIndex=j;	//index in dType list
									break;	
								}
							}
							// parameter substittuion
							for (int i=0; i < tplist.size(); ++i)
							{
								if (typeTag==1)	//parameter substitution
								{
									code[i]=code[i]+" "+$1+tplist[i].nType[paramIndex];
								}
							}

							for (int i=0; i < tplist.size(); ++i)
							{
								if (tplist[i].dName==curID) //name substitution happen
								{
									nameTag=1;
									code[i]=code[i]+" "+$1+tplist[i].nName;
								}
							}
							for (int i=0; i < tplist.size(); ++i)
							{
								if ((typeTag==0)&&(nameTag==0)) //type and name substitution not happen
								{
									code[i]=code[i]+" "+$1+curID;
								}
							}
						}
						else fprintf(yyout," %s %s",$1,$2);
					}
	| ALR ID		{
						printf(" end arguments appendition : %s \n",$2);

						if(tmpdecTag!=0)
						{
							for (int i=0; i < tplist.size(); ++i)
							{
								code[i]=code[i]+" "+$1+" "+$2;
							}
							
						}
						else fprintf(yyout," %s %s",$1,$2);
					}
	;
	
	addition:
	tmpparam addition		{}
	| tmpparam classdec	        {}
	;

	tmpparam:	
	  ALR ID ID ALR			{
						ParamList.clear();
						ParamList=ParamList+"template "+$1+$2+" "+$3+$4;
					}
	| ALR ID ID  		    	{
						ParamList.clear();
						ParamList=ParamList+"template "+$1+$2+" "+$3;					
					}
	| COMMA ID ID 			{
						ParamList=ParamList+$1+$2+" "+$3;
					}
	| COMMA ID ID ALR		{
						ParamList=ParamList+$1+$2+" "+$3+$4;
					}
	;

	classdec:
	ID ID				{	printf("class name:%s\n",$1);	
						string tpFlag=$1;
						if (tpFlag=="parclass") //parclass template located
						{	printf("brace located! \n");	
							tplist.clear();
							curTemplateName.clear();
						
							tmpdecTag=1;
							string curName=$2;
							curTemplateName=curName;

							Json::Value com_value=root["ArgumentElement"];  //info.json
							Json::Value com_subinfo=substitution["ParameterElement"];  //substitution.json
							// count the type of same template class
							for (int i = 0; i < com_value.size(); ++i)
							{
								int cnt=1;
								Json::Value temp_value = com_value[i];
								if (temp_value["orName"]==curName)
								{	//form the tplist of
									tmpInfo tmp;
									tmp.dName=curName;
									tmp.nName=temp_value["newName"].asString();
									for (int n = 0; n < temp_value["arguementList"].size();++n)
									{	//load the arguementList
										Json::Value tmm=temp_value["arguementList"][n];	
										tmp.nType.push_back(tmm.asString());
									}
									for (int k = 0; k <  com_subinfo.size(); ++k)
									{	//locate corresponding item in parameterlist
										Json::Value temp_subinfo=com_subinfo[k];
										if(temp_subinfo["orName"]==curName)
										{
											printf("Make dType ! CurName:%s \n",curName.c_str());
											for (int m=0;m<temp_subinfo["orParameters"].size();++m)
											{
												Json::Value tnn=temp_subinfo["orParameters"][m];
												tmp.dType.push_back(tnn.asString());
											}
										}
									}
									

									tmp.index=cnt;
									cnt++;
									tplist.push_back(tmp);
								}	
							}
						
							//generate code
							for (int j =0; j < tplist.size(); ++j)
							{
								code[j] = code[j]+$1+" "+tplist[j].nName;
								printf("code %d: %s",j,code[j].c_str());
								printf("new Name: %s at code : %d \n",tplist[j].nName.c_str(),j);
							}
						}
						else 	//normal template
						{	
							fprintf(yyout," %s %s %s",ParamList.c_str(),$1,$2);
							ParamList.clear();
						}
					}
	| paramdec classdec		{}
	| paramdec			{}		
	;
	
	paramdec:
	ID ID ALR			{	printf("class name:%s\n",$2);	
						printf("method class name:%s\n",$2);	// template method
						
						int FLag=0;
						printf("brace located! \n");	
						tplist.clear();
						curTemplateName.clear();
						
						string curName=$2;
						curTemplateName=curName;

						Json::Value com_value=root["ArgumentElement"];  //info.json
						Json::Value com_subinfo=substitution["ParameterElement"];  //substitution.json
							// count the type of same template class

						//check the nanme of class
						for (int i = 0; i < com_value.size(); ++i)
						{
							Json::Value temp_value = com_value[i];
							if(temp_value["orName"]==curName)
							{
								printf("orName located in the template methods: %s %s \n",$1,$2);
								FLag=1;
								break;
							}
						}					

						if(FLag==1)	//parclass located
						{
							tmpdecTag=1;
							for (int i = 0; i < com_value.size(); ++i)
							{
								int cnt=1;
								Json::Value temp_value = com_value[i];
								if (temp_value["orName"]==curName)
								{	//form the tplist of
									tmpInfo tmp;
									tmp.dName=curName;
									tmp.nName=temp_value["newName"].asString();
									for (int n = 0; n < temp_value["arguementList"].size();++n)
									{	//load the arguementList
										Json::Value tmm=temp_value["arguementList"][n];	
										tmp.nType.push_back(tmm.asString());
									}
									for (int k = 0; k <  com_subinfo.size(); ++k)
									{	//locate corresponding item in parameterlist
										Json::Value temp_subinfo=com_subinfo[k];
										if(temp_subinfo["orName"]==curName)
										{
											printf("Make dType ! CurName:%s \n",curName.c_str());
											for (int m=0;m<temp_subinfo["orParameters"].size();++m)
											{
												Json::Value tnn=temp_subinfo["orParameters"][m];
												tmp.dType.push_back(tnn.asString());
											}
										}
									}
									
									tmp.index=cnt;
									cnt++;
									tplist.push_back(tmp);
										
								}
							}
							//replace the $1 return type
							string rtType=$1;
							int subTag=0;
							int paramIndex;
							vector <string> tnt=tplist[0].dType;
							for (int j=0; j < tnt.size(); ++j)
							{
								if(rtType==tnt[j])
								{
									subTag=1;	//parameter matched
									paramIndex=j;	//index in dType list
									break;	
								}
							}
							//type replace
							
							for (int i=0; i < tplist.size(); ++i)
							{
								if (subTag==1)	//parameter substitution
								{
									code[i]=code[i]+" "+tplist[i].nType[paramIndex];
								}
								else //subTag=0, no substitution
								{
									code[i]=code[i]+" "+rtType;
								}
							}						
							
								//generate code
							for (int j =0; j < tplist.size(); ++j)
							{
								code[j] = code[j]+" "+tplist[j].nName;
								printf("new Name: %s at code : %d \n",tplist[j].nName.c_str(),j);
							}
							
						}
						else 	//normal template
						{	
							fprintf(yyout," %s %s %s %s",ParamList.c_str(),$1,$2,$3);
							ParamList.clear();
						}						

					}
	| ID ALR			{	printf("class name:%s\n",$1);	
						int FLag=0;
						printf("Constructor located! \n");	
						tplist.clear();
						curTemplateName.clear();
						
						string curName=$1;
						curTemplateName=curName;

						Json::Value com_value=root["ArgumentElement"];  //info.json
						Json::Value com_subinfo=substitution["ParameterElement"];  //substitution.json
						// count the type of same template class

						//check the nanme of class
						for (int i = 0; i < com_value.size(); ++i)
						{
							Json::Value temp_value = com_value[i];
							if(temp_value["orName"]==curName)
							{
								printf("orName located in the template declaration: %s \n",$1);
								FLag=1;
								break;
							}
						}					

						if(FLag==1)	//parclass located
						{
							tmpdecTag=1;
							for (int i = 0; i < com_value.size(); ++i)
							{
								int cnt=1;
								Json::Value temp_value = com_value[i];
								if (temp_value["orName"]==curName)
								{	//form the tplist of
									tmpInfo tmp;
									tmp.dName=curName;
									tmp.nName=temp_value["newName"].asString();
									for (int n = 0; n < temp_value["arguementList"].size();++n)
									{	//load the arguementList
										Json::Value tmm=temp_value["arguementList"][n];	
										tmp.nType.push_back(tmm.asString());
									}
									for (int k = 0; k <  com_subinfo.size(); ++k)
									{	//locate corresponding item in parameterlist
										Json::Value temp_subinfo=com_subinfo[k];
										if(temp_subinfo["orName"]==curName)
										{
											printf("Make dType ! CurName:%s \n",curName.c_str());
											for (int m=0;m<temp_subinfo["orParameters"].size();++m)
											{
												Json::Value tnn=temp_subinfo["orParameters"][m];
												tmp.dType.push_back(tnn.asString());
											}
										}
									}
									
									tmp.index=cnt;
									cnt++;
									tplist.push_back(tmp);
										
								}
							}
							//generate code
							for (int j =0; j < tplist.size(); ++j)
							{
								code[j] = code[j]+" "+tplist[j].nName;
								printf("new Name: %s at code : %d \n",tplist[j].nName.c_str(),j);
							}
							
						}
						else 	//normal template
						{	
							fprintf(yyout," %s %s %s",ParamList.c_str(),$1,$2);
							ParamList.clear();
						}	
				
					}
	| ID ALR SYMBOL SYMBOL		{	printf("end of parameters %s\n",$1);
						if (tmpdecTag==1)
						{
							for (int j =0; j < tplist.size(); ++j)
							{
								code[j] = code[j]+" "+$3+$4;
							}
						}
						else
						{
							fprintf(yyout,"%s %s %s %s%s", ParamList.c_str(),$1,$2,$3,$4);
							ParamList.clear();
						}
					}
	| ID COMMA			{
						printf("more parameters: %s\n",$1);
						if(tmpdecTag==1)
						{
						}
						else
						{
							ParamList=ParamList+$1+$2;
						}
					}
	| methods			{printf("method in\n");}
	;

	methods:
	SPKEYWORD SYMBOL	{	//start of method name and param,sub name
					printf("spkeyword in method: %s \n", $1);
					string curWord=$1;
					string symb=curWord.substr(0,1);
					string curID=curWord.substr(1,curWord.size());
					int nameTag=0;
					if (tmpdecTag==1)
					{
						for (int i=0; i < tplist.size(); ++i)
						{
							if (tplist[i].dName==curID) //name substitution happen
							{
								nameTag=1;
								code[i]=code[i]+symb+tplist[i].nName+$2;
							}
						}
						for (int i=0; i < tplist.size(); ++i)
						{
							if (nameTag==0) //type and name substitution not happen
							{
								code[i]=code[i]+symb+curID+$2;
							}
						}					
					}
					else
					{
						fprintf(yyout,"%s %s",$1,$2);
					}
				}
	| ID SYMBOL		{	//start of method name and param,sub name
					string curID=$1;
					int nameTag=0;
					if (tmpdecTag==1)
					{
						for (int i=0; i < tplist.size(); ++i)
						{
							if (tplist[i].dName==curID) //name substitution happen
							{
								nameTag=1;
								code[i]=code[i]+tplist[i].nName+$2;
							}
						}
						for (int i=0; i < tplist.size(); ++i)
						{
							if (nameTag==0) //type and name substitution not happen
							{
								code[i]=code[i]+curID+$2;
							}
						}	
					}
					else
					{
						fprintf(yyout,"%s %s",$1,$2);
					}
				}
	| ID ID COMMA		{	// parameter, sub type
					printf("id in method: %s %s\n", $1,$2);
					string curID1=$1;
					string curID2=$2;
					int subTag1=0;
					int subTag2=0;
					int paramIndex; 
					if (tmpdecTag==1)
					{
						vector <string> tnt=tplist[0].dType;
						for (int j=0; j < tnt.size(); ++j)
						{
							if(curID1==tnt[j])
							{
								subTag1=1;	//parameter matched
								paramIndex=j;	//index in dType list
								break;	
							}
						}
						//substittuion
							
						for (int i=0; i < tplist.size(); ++i)
						{
							if (subTag1==1)	//parameter substitution
							{
								code[i]=code[i]+" "+tplist[i].nType[paramIndex];
							}
							else //subTag=0, no substitution
							{
								code[i]=code[i]+" "+curID1;
							}
						}

						for (int j=0; j < tnt.size(); ++j)
						{
							if(curID2==tnt[j])
							{
								subTag2=1;	//parameter matched
								paramIndex=j;	//index in dType list
								break;	
							}
						}
						//substittuion					
						for (int i=0; i < tplist.size(); ++i)
						{
							if (subTag2==1)	//parameter substitution
							{
								code[i]=code[i]+" "+tplist[i].nType[paramIndex]+$3;
							}
							else //subTag=0, no substitution
							{
								code[i]=code[i]+" "+curID2+$3;
							}
						}
						
					}
					else
					{
						fprintf(yyout,"%s %s %s",$1,$2,$3);
					}				
				}
	| ID ID SYMBOL		{
					printf("id in method: %s %s\n", $1,$2);
					string curID1=$1;
					string curID2=$2;
					int subTag1=0;
					int subTag2=0;
					int paramIndex; 
					if (tmpdecTag==1)
					{
						vector <string> tnt=tplist[0].dType;
						for (int j=0; j < tnt.size(); ++j)
						{
							if(curID1==tnt[j])
							{
								subTag1=1;	//parameter matched
								paramIndex=j;	//index in dType list
								break;	
							}
						}
						//substittuion
							
						for (int i=0; i < tplist.size(); ++i)
						{
							if (subTag1==1)	//parameter substitution
							{
								code[i]=code[i]+" "+tplist[i].nType[paramIndex];
							}
							else //subTag=0, no substitution
							{
								code[i]=code[i]+" "+curID1;
							}
						}

						for (int j=0; j < tnt.size(); ++j)
						{
							if(curID2==tnt[j])
							{
								subTag2=1;	//parameter matched
								paramIndex=j;	//index in dType list
								break;	
							}
						}
						//substittuion					
						for (int i=0; i < tplist.size(); ++i)
						{
							if (subTag2==1)	//parameter substitution
							{
								code[i]=code[i]+" "+tplist[i].nType[paramIndex];
							}
							else //subTag=0, no substitution
							{
								code[i]=code[i]+" "+curID2;
							}
							code[i]=code[i]+$3;
						}
						
					}
					else
					{
						fprintf(yyout," %s %s %s",$1,$2,$3);
					}
				}
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
	| INSTR			{ //fprintf(yyout,"%s",$1);
					string tmpfile=$1;
					tmpfile=tmpfile.substr(1,tmpfile.size());
					int bIndex=tmpfile.find('\"',0);
					tmpfile=tmpfile.substr(0,bIndex);
					printf("include file :%s\n",tmpfile.c_str());
					string tmpline;
					int subFlag=0;
					while ( getline (fname,tmpline) )
    					{
      						if (tmpline==tmpfile)
						{
							subFlag=1;
							char num='3';
							int index=tmpline.find('.',0);
							string front=tmpline.substr(0,index);
							string behind=tmpline.substr(index,tmpline.size());
							string newName=front+"_"+num+behind;
							fprintf(yyout,"\"%s\"",newName.c_str());
						}
    					}
					if (subFlag==0)
					{fprintf(yyout,"%s",$1);}

				}
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
	int index1=fileName.find('_',0);
	printf("index2 : %d\n",index1);
	string front=fileName.substr(0,index1);
	int index2=fileName.find('.',0);
	string behind=fileName.substr(index2,fileName.size());
	char num='3';
	string whole=front+"_"+num+behind;
	
	FILE* outfp=fopen(whole.c_str(), "w"); 
	
	if(outfp==NULL)
	{
		printf("can not open outfile \n");
		return -1;
	}
	
	  
	yyout=outfp;

	//outCheck.open("outInfo.cc");
	fname.open("filename.txt");	
	
	ifs.open("info.json");
	iifs.open("substitution.json");
	
	if (!root_reader.parse(ifs, root, false))
    	{ return -1; }
	Json::Value& add_value = root["ArgumentElement"];

	if (!sub_reader.parse(iifs, substitution, false))
    	{ return -1; }
	Json::Value& sub_value = substitution["ParameterElement"];

	yyparse();
	/*
	for (int i=0; i < tplist.size(); i++)
	{
		
		if(!code[i].empty())
			//outCheck<<code[i]<<endl;
			printf("%s/n",code[i].c_str());
		else break;
	}*/
	//outCheck.close();	
	
	fclose(infp);
	fclose(outfp);
	ifs.close();
	iifs.close();
	fname.close();
	
	return 0;
	}
