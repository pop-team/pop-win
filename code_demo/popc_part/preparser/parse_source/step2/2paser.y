%{
	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	
	#include <json/json.h>
	#include <iostream>
	#include <fstream>

	int yylex(void);
	void yyerror(char *);
	extern FILE* yyin; 
	extern FILE* yyout; 

	using namespace std;
	std::ofstream ofs;
	Json::Reader reader;
	Json::Value root;
	Json::Value& add_value = root["ArgumentElement"];

	std::ifstream iifs;
	Json::Reader sub_reader;
	Json::Value substitution;
	Json::Value& sub_value = substitution["ParameterElement"];
	
	Json::FastWriter writer;
	std::string json_append_file;

	//modification
	typedef struct Count
	{
		string name;
		int count;
	}nameCount;
	
	vector <nameCount> countList;
	
	vector <string> argumList;

	string code;

	int newMultiTempFlag=0; //flag for the template with mutlitple arguments.

	Json::Value append_value;

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
	
	
	%token <str> ID	RT INSTR OPERATOR SYMBOL SPKEYWORD ALR COMMA DECPOINT
	%token <num> DIGIT
%%
	program: express program	{}
	| express			{}
	|				{}
	;	

	express: keyword express	{}
	| ID ALR			{fprintf(yyout," %s%s",$1,$2);
					printf(" id arl: %s %s\n", $1,$2);}
	| ID COMMA			{fprintf(yyout," %s%s",$1,$2);
					printf("id comma: %s %s\n", $1,$2);}
	| ID				{fprintf(yyout," %s",$1);
					printf("id: %s \n", $1);}
	| DECPOINT			{fprintf(yyout,"%s",$1);
					printf("decimal point: %s \n", $1);}
	| COMMA				{fprintf(yyout," %s",$1);
					printf("comma: %s \n", $1);}
	| OPERATOR			{fprintf(yyout," %s",$1);
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
	| DIGIT			{fprintf(yyout," %d", $1);}
	| INSTR			{fprintf(yyout,"%s",$1);}
	| DAL			{fprintf(yyout,"<<");}
	| DAR			{fprintf(yyout,">>");}
	;

	vect1:
	VEC ALR vect2		{}
	| VEC ALR ID ALR		{fprintf(yyout," vector %s%s%s",$2,$3,$4);}
	;
	
	vect2:
	VEC ALR ID ALR ALR		{fprintf(yyout," vector < vector %s>",$3);}
	;
	

	template:
	ind indparam		{}	
	| TEMP addition		{}
	| instance		{} //instantiation of template
	;
	
	instance:
	ID ALR ID ALR ID		{
						string curName=$1;
						//check curName is the parclass template or not
						int tmpFlag=0;
						for (int i = 0; i < sub_value.size(); ++i)
						{
							Json::Value temp_value = sub_value[i];
							if(temp_value["orName"]==curName) 	//ture, it is a parclass template
							{
								printf("locate curName: %s\n", curName.c_str());
								tmpFlag=1;
								
								int Match=1;
								append_value.clear(); //reset global appending value
								printf("argumentList number: %d\n", add_value.size());
								for (int i=0; i < add_value.size(); ++i) //check existed name and arguments
								{
									Json::Value tem= add_value[i];
									string currentName=$1;
									string currentArgum=$3;
									Json::Value& array=tem["arguementList"];
									if(tem["orName"]==$1)		//name match with info.json
									{
										printf("check arguments!\n");
										int Flag=0;
										for (int k=0;k<array.size();++k)
										{
											string curArg=array[k].asString();
											printf("string :%s!\n",curArg.c_str());
											if(curArg!=$3)
											{
												Flag=1;
												printf("argument not matched!!\n");
												break;
											}
										}	
										if(Flag==0)
										{
											Match=0; //name fully matched, do nothing
											printf("Name and argument matched! Keep existed name\n");
											curName.clear();
											curName=tem["newName"].asString();
										}
										else 	{Match=1;}  //no name matched	
									}
								}
								if (Match==1)//no name and argument fully matched, add new template information
								{
									curName.append("_");
									int nameIndex=0;
									int mark=0; //found mark
									if (!countList.empty())		//check the original name is existed or not
									{
										for(int t=0;t<countList.size();++t)
										{
											if (countList[t].name==curName)
											{	countList[t].count++;
												nameIndex=countList[t].count;
												mark=1;
											}
										}
									}
									if (mark==0)
									{
										nameCount toto;
										toto.name=curName;
										toto.count=0;
										countList.push_back(toto);
									}
									char num[4];
									sprintf(num,"%d",nameIndex);
									curName.append(num);
									printf("newname: %s\n", curName.c_str()); //generate new name	

										
									append_value["orName"] = $1;
									append_value["newName"] = curName;
									
									Json::Value& arlist=append_value["arguementList"];
									string argument=$3;
									Json::Value tmp=argument;
									arlist.append(tmp);								
						
									add_value.append(append_value);
									printf("No Name and argument matched!\n");
								}	
							}
						}
						if (tmpFlag==1)		//parclass template
						{
							json_append_file.clear();
							json_append_file = writer.write(root);	
							printf(" %s\n", json_append_file.c_str());
							fprintf(yyout," %s %s",curName.c_str(),$5);
						}
						else			//normal template
						{
							fprintf(yyout," %s %s %s %s %s",$1,$2,$3,$4,$5);	
						}
					}
	| ID ALR DIGIT ALR ID		{
						string curName=$1;
						//check curName is the parclass template or not
						int tmpFlag=0;
						for (int i = 0; i < sub_value.size(); ++i)
						{
							Json::Value temp_value = sub_value[i];
							if(temp_value["orName"]==curName) 	//ture, it is a parclass template
							{
								printf("locate curName: %s\n", curName.c_str());
								tmpFlag=1;
								
								int Match=1;
								append_value.clear(); //reset global appending value
								printf("argumentList number: %d\n", add_value.size());
								for (int i=0; i < add_value.size(); ++i) //check existed name and arguments
								{
									Json::Value tem= add_value[i];
									string currentName=$1;
									//string currentArgum=$3;
									int tt=$3;
									char dig[4];
									sprintf(dig, "%d", tt);
									string currentArgum=dig;
									//
									Json::Value& array=tem["arguementList"];
									if(tem["orName"]==$1)		//name match with info.json
									{
										printf("check arguments!\n");
										int Flag=0;
										for (int k=0;k<array.size();++k)
										{
											string curArg=array[k].asString();
											printf("string :%s!\n",curArg.c_str());
											if(curArg!=currentArgum)
											{
												Flag=1;
												printf("argument not matched!!\n");
												break;
											}
										}	
										if(Flag==0)
										{
											Match=0; //name fully matched, do nothing
											printf("Name and argument matched! Keep existed name\n");
											curName.clear();
											curName=tem["newName"].asString();
										}
										else 	{Match=1;}  //no name matched	
									}
								}
								if (Match==1)//no name and argument fully matched, add new template information
								{
									curName.append("_");
									int nameIndex=0;
									int mark=0; //found mark
									if (!countList.empty())		//check the original name is existed or not
									{
										for(int t=0;t<countList.size();++t)
										{
											if (countList[t].name==curName)
											{	countList[t].count++;
												nameIndex=countList[t].count;
												mark=1;
											}
										}
									}
									if (mark==0)
									{
										nameCount toto;
										toto.name=curName;
										toto.count=0;
										countList.push_back(toto);
									}
									char num[4];
									sprintf(num,"%d",nameIndex);
									curName.append(num);
									printf("newname: %s\n", curName.c_str()); //generate new name	

										
									append_value["orName"] = $1;
									append_value["newName"] = curName;
									
									Json::Value& arlist=append_value["arguementList"];
									//string argument=$3;

									int tt=$3;
									char dig[4];
									sprintf(dig, "%d", tt);
									string argument=dig;
									//
									Json::Value tmp=argument;
									arlist.append(tmp);								
						
									add_value.append(append_value);
									printf("No Name and argument matched!\n");
								}	
							}
						}
						if (tmpFlag==1)		//parclass template
						{
							json_append_file.clear();
							json_append_file = writer.write(root);	
							printf(" %s\n", json_append_file.c_str());
							fprintf(yyout," %s %s",curName.c_str(),$5);
						}
						else			//normal template
						{
							fprintf(yyout," %s %s %d %s %s",$1,$2,$3,$4,$5);	
						}
					}
	| ID ALR ID  			{
						string curName=$1;
						//check curName is the parclass template or not
						int tmpFlag=0;
						for (int i = 0; i < sub_value.size(); ++i)
						{
							Json::Value temp_value = sub_value[i];
							if(temp_value["orName"]==curName) 	//ture, it is a parclass template
							{
								printf("locate curName: %s\n", curName.c_str());
								tmpFlag=1;
								
								int Match=1;
								append_value.clear(); //reset global appending value
								printf("argumentList number: %d\n", add_value.size());
								for (int i=0; i < add_value.size(); ++i) //check existed name and arguments
								{
									Json::Value tem= add_value[i];
									string currentName=$1;
									string currentArgum=$3;
									Json::Value& array=tem["arguementList"];
									if(tem["orName"]==$1)		//name match with info.json
									{
										printf("check arguments!\n");
										int Flag=0;
										for (int k=0;k<array.size();++k)
										{
											string curArg=array[k].asString();
											printf("string :%s!\n",curArg.c_str());
											if(curArg!=$3)
											{
												Flag=1;
												printf("argument not matched!!\n");
												break;
											}
										}	
										if(Flag==0)
										{
											Match=0; //name fully matched, do nothing
											printf("Name and argument matched! Keep existed name\n");
											curName.clear();
											curName=tem["newName"].asString();
										}
										else 	{Match=1;}  //no name matched	
									}
								}
								if (Match==1)//no name and argument fully matched, add new template information
								{
									curName.append("_");
									int nameIndex=0;
									int mark=0; //found mark
									if (!countList.empty())		//check the original name is existed or not
									{
										for(int t=0;t<countList.size();++t)
										{
											if (countList[t].name==curName)
											{	countList[t].count++;
												nameIndex=countList[t].count;
												mark=1;
											}
										}
									}
									if (mark==0)
									{
										nameCount toto;
										toto.name=curName;
										toto.count=0;
										countList.push_back(toto);
									}
									char num[4];
									sprintf(num,"%d",nameIndex);
									curName.append(num);
									printf("newname: %s\n", curName.c_str()); //generate new name	

									append_value["orName"] = $1;
									append_value["newName"] = curName;
									
									argumList.clear();
									string argument=$3;
									argumList.push_back(argument);								
						
									printf("No Name and argument matched!\n");
									fprintf(yyout," %s",curName.c_str());
								}	
							}
						}
						if (tmpFlag==1)		//parclass template
						{
							newMultiTempFlag=1; // in the multi-arguments template declaration 
							printf("multi-argument flag=%d\n",newMultiTempFlag);
						}
						else			//normal template
						{
							fprintf(yyout," %s %s %s",$1,$2,$3);	
						}							
					}
	| ID ALR DIGIT  			{
						string curName=$1;
						//check curName is the parclass template or not
						int tmpFlag=0;
						for (int i = 0; i < sub_value.size(); ++i)
						{
							Json::Value temp_value = sub_value[i];
							if(temp_value["orName"]==curName) 	//ture, it is a parclass template
							{
								printf("locate curName: %s\n", curName.c_str());
								tmpFlag=1;
								
								int Match=1;
								append_value.clear(); //reset global appending value
								printf("argumentList number: %d\n", add_value.size());
								for (int i=0; i < add_value.size(); ++i) //check existed name and arguments
								{
									Json::Value tem= add_value[i];
									string currentName=$1;
									//string currentArgum=$3;
									int tt=$3;
									char dig[4];
									sprintf(dig, "%d", tt);
									string currentArgum=dig;
									//
									Json::Value& array=tem["arguementList"];
									if(tem["orName"]==$1)		//name match with info.json
									{
										printf("check arguments!\n");
										int Flag=0;
										for (int k=0;k<array.size();++k)
										{
											string curArg=array[k].asString();
											printf("string :%s!\n",curArg.c_str());
											if(curArg!=currentArgum)
											{
												Flag=1;
												printf("argument not matched!!\n");
												break;
											}
										}	
										if(Flag==0)
										{
											Match=0; //name fully matched, do nothing
											printf("Name and argument matched! Keep existed name\n");
											curName.clear();
											curName=tem["newName"].asString();
										}
										else 	{Match=1;}  //no name matched	
									}
								}
								if (Match==1)//no name and argument fully matched, add new template information
								{
									curName.append("_");
									int nameIndex=0;
									int mark=0; //found mark
									if (!countList.empty())		//check the original name is existed or not
									{
										for(int t=0;t<countList.size();++t)
										{
											if (countList[t].name==curName)
											{	countList[t].count++;
												nameIndex=countList[t].count;
												mark=1;
											}
										}
									}
									if (mark==0)
									{
										nameCount toto;
										toto.name=curName;
										toto.count=0;
										countList.push_back(toto);
									}
									char num[4];
									sprintf(num,"%d",nameIndex);
									curName.append(num);
									printf("newname: %s\n", curName.c_str()); //generate new name	

									append_value["orName"] = $1;
									append_value["newName"] = curName;
									
									argumList.clear();
									//string argument=$3;
									int tt=$3;
									char dig[4];
									sprintf(dig, "%d", tt);
									string argument=dig;					
									//
									argumList.push_back(argument);								
						
									printf("No Name and argument matched!\n");
									fprintf(yyout," %s",curName.c_str());
								}	
							}
						}
						if (tmpFlag==1)		//parclass template
						{
							newMultiTempFlag=1; // in the multi-arguments template declaration 
							printf("multi-argument flag=%d\n",newMultiTempFlag);
						}
						else			//normal template
						{
							fprintf(yyout," %s %s %d",$1,$2,$3);	
						}							
					}


	| COMMA ID 			{	
						if(newMultiTempFlag==1)
						{
							printf("multi-argument flag=%d\n",newMultiTempFlag);
							
							string argument=$2;		
							argumList.push_back(argument);
																			
							
							printf("New Arguments added to list. %s \n",argument.c_str());
						}
						else 
						{	printf("Arguments ID : %s \n",$2);
							fprintf(yyout," %s %s",$1,$2);
						}
					}
	| COMMA DIGIT 			{	
						if(newMultiTempFlag==1)
						{
							printf("multi-argument flag=%d\n",newMultiTempFlag);
							
							//string argument=$2;	

							int tt=$2;
							char dig[4];
							sprintf(dig, "%d", tt);
							string argument=dig;
							//	
							argumList.push_back(argument);
																			
							
							printf("New Arguments added to list. %s \n",argument.c_str());
						}
						else 
						{	printf("Arguments ID : %d \n",$2);
							fprintf(yyout," %s %d",$1,$2);
						}
					}
	| ALR ID			{
						if (newMultiTempFlag==1)	//end of template
						{
							newMultiTempFlag=0; //end appending operation
							printf("multi-argument flag=%d\n",newMultiTempFlag);								
							printf("End list appending. \n");
							
							Json::Value& arlist=append_value["arguementList"];
							for (int k=0;k<argumList.size();++k)
							{							
								Json::Value tmp=argumList[k];
								arlist.append(tmp);
							}								
						
							add_value.append(append_value);
							

							json_append_file.clear();
							json_append_file = writer.write(root);	
							printf(" %s\n", json_append_file.c_str());
							
							fprintf(yyout," %s",$2);
						}
						else 
						{	printf("end Arguments : %s \n",$2);
							fprintf(yyout," %s %s",$1,$2);
						}
					}
	;
	
	addition:
	tmpparam addition		{}
	| tmpparam classdec	        {}
	;

	tmpparam:
	  ALR ID ID ALR		{fprintf(yyout,"template %s %s %s %s ",$1,$2,$3,$4);}
	| ALR ID ID COMMA    	{fprintf(yyout,"template %s %s %s, ",$1,$2,$3);}
	| ID ID COMMA		{fprintf(yyout,"%s %s, ",$1,$2);}
	| ID ID ALR		{fprintf(yyout,"%s %s %s ",$1,$2,$3);}
	;
	
	classdec:
	ID ID				{	printf("class name:%s\n",$1);	
						fprintf(yyout,"%s %s",$1,$2);
					}
	| paramdec classdec		{}
	| paramdec			{}		
	;
	
	paramdec:
	ID ID ALR			{	printf("class name:%s\n",$2);	
						fprintf(yyout,"%s %s %s",$1,$2,$3);
					}
	| ID ALR			{	printf("class name:%s\n",$1);	
						fprintf(yyout,"%s %s",$1,$2);				
					}
	| ID ALR SYMBOL SYMBOL		{	printf("end of parameters %s\n",$1);
						fprintf(yyout," %s %s%s%s",$1,$2,$3,$4);}
	| ID COMMA			{
						printf("appendition of parameters: %s\n",$1);
						fprintf(yyout," %s,",$1);
					}
	| methods			{printf("method in\n");}
	;

	methods:
	SPKEYWORD SYMBOL	{fprintf(yyout," %s %s",$1,$2);}
	| ID SYMBOL		{fprintf(yyout," %s %s",$1,$2);}
	| ID ID COMMA		{fprintf(yyout," %s %s %s",$1,$2,$3);}
	| ID ID SYMBOL		{fprintf(yyout," %s %s %s",$1,$2,$3);}
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
	|			{}
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
	printf("index1 : %d\n",index1);
	string front=fileName.substr(0,index1);
	int index2=fileName.find('.',0);
	string behind=fileName.substr(index2,fileName.size());
	char num='2';
	string whole=front+"_"+num+behind;

	FILE* outfp=fopen(whole.c_str(), "w"); 
	
	if(outfp==NULL)
	{
		printf("can not open outfile \n");
		return -1;
	}
	
	  
	yyout=outfp;
	
	ofs.open("info.json",ios::app);
	//oofs.open("substitution.json",ios::app);
	

	iifs.open("substitution.json");
	if (!sub_reader.parse(iifs, substitution, false))
    	{ return -1; }

	
	

	
	yyparse();
	ofs << json_append_file;
	//oofs << sub_append_file;
	
	fclose(infp);
	fclose(outfp);
	ofs.close();
	iifs.close();

	return 0;
	}
