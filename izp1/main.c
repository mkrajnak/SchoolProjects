/*
 * Soubor:  proj1.c
 * Datum:   2014/11/10
 * Autor:   Martin Krajňák, xkrajn02@stud.fit.vutbr.cz
 * Projekt: Výpočty v tabuľke, projekt c. 1 pro predmet IZP
 * Popis:   Program poskytuje možnosti práce s tabuľkou ktorá je načítaná na štandartnom vstupe.
 *          Schopnosťou programu je nielen výpis buniek pomocou funkcie select, ale aj výpočty
 *          aritmetického priemeru a súčtu buniek v tabuľke, ak je v nich obsiahnutá numerická hodnota.
 *          Pýchou programu je schopnosť nájdenie minima a maxima, a možnosť výpisu buniek v intervaloch,
 *          a to po riadkoch, stĺpcoch alebo z oboch intervalov naraz pomocou funkcie range.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void help (){ 
   printf ("This sofware providing operations with tables, it is able to view colum,\n" 
      "row, colums, rows, or range of colums and rows via select funkction.\n"
      "It can count sum or average if selected colums contain numeric values,\n"
      "and it is also able to find minimum and maximum values inside colums and rows\n"
      "Examples:\n\t ./prog select col 1\n"
               "\t ./prog select cols 1 2\n"
               "\t ./prog sum col 1\n"
               "\t ./prog avg col 1\n"
               "\t ./prog select range 1\n");
}  
//funkction for checking if numeric argument includes char
//if char was detected, 0 is returned and detected string is printed
double checkArgv(char **argv,int i){
      char *white;  
      double a = strtod(argv[i],&white);  
      if(strlen(white) != 0){ 
            fprintf(stderr,"Argument error, unknow input%s",white);
            return 0;
      }
      else  
         return a;   //no char detected, numeric value is returned 
}

/*operation inserted by user is printed, if recognized*/
void operation(char **argv,double *results){

   if(results[4] !=0 ){
         error("Error: Words detected in requested operation");
         return;
   }
   if (strcmp(argv[1],"sum") == 0){
         printf("%.10g\n",results[0]);
   }
   else if (strcmp(argv[1],"avg") == 0){
         printf("%.10g\n",(results[0]/results[1]));
   }
   else if (strcmp(argv[1],"max") == 0){
         printf("%.10g\n", results[2] );
   }
   else if (strcmp(argv[1],"min") == 0){
         printf("%.10g\n", results[3] );
   }
   else{
      error("Operaction not recognized");
   }

}
/*providing operations on table colums and rows*/
void selector(char **argv, char *tok, double *results){

   double cislo;        //if token is a number,make it double 
   char *white;         //if argument (number) is inserted with char
   cislo = strtod(tok,&white );
   if(strlen(white)==0 || (strcmp(white,"\n")==0)){   // if number is found
         if (strcmp(argv[1],"select")==0){            //if operation is select, value is printed
               printf("%.10g\n",cislo );
         }
         results[0]+=cislo;         // sum operation
         results[1]++;              //number counter
         if(results[3] == 0){
               results[3]=cislo;    //first value is set as minimum
         }
         if (cislo > results[2]){   //  maximum
               results[2] = cislo;
         }
         if (cislo < results[3]){   // minimum
               results[3] = cislo;
         }      
   }
   else { // data in cell is "word"
         if (strcmp(argv[1],"select")==0){ 
               printf("%s\n",tok);
         }
         results[4]++;              //number counter
   }
}
/*RANGE OPERATIONS*/   
void range(char **argv ){ 
   int row=0,col=0;                 // counters
   double results[5] = {0,0,0,0,0}; // suma, nums, max, min, words
   int numArgs[4];                  //numeric arguments needed for range operation
   char temp[1024];                 //temporary field for one line of table
   char *tok;                       //line is stripped to tokens
   for (int i = 0; i < 4; ++i){     //checking inserted parameters
      numArgs[i]=checkArgv(argv,i+3);
      if (numArgs[i]==0){
         return;
      }
   }
   if (numArgs[1] < numArgs[0] || numArgs[3] < numArgs[2]){//checking if args was inserted in right position
      error("Wrong position of numeric arguments");
      return;
   }
  while (fgets (temp, 1024, stdin) !=0){                   //reading line from stdin
         tok = strtok(temp," ");                           //stripping first value from line
         row++;
         while(tok!=NULL){
               col++;
               if(row>=numArgs[0] && row<=numArgs[1] && col>=numArgs[2] && col<=numArgs[3]){ //checking inteval inserted by user 
                    selector(argv,tok,results);            // data is computed by selector
               }
               tok = strtok(NULL," ");
         }
         if (col < numArgs[2] || numArgs[2] <= 0 || col < numArgs[3] || numArgs[3] <= 0){ //checking if file even has that many colums
               error("Error: No data in selected colum");
               return ;
         }
         col=0;
   }
   if (row < numArgs[0] || numArgs[0] <= 0 || row < numArgs[1] || numArgs[1] <= 0){       //checking if file even has that many rows
            if(row == 0)
                  error("File is empty"); 
            else
                  error("Error: No data in selected row");
            return;
   }
   if(strcmp(argv[1],"select")!=0){          // if select wasnt called operation
               operation(argv,results);      // results of required operation is printed
   }                                       
}

void rowsCols(char **argv){
   int row=0,col=0;              // counter of rows and cols
   double results[5]={0,0,0,0,0};
   char temp[1024];              // reading a line of file
   double fNumArg,sNumArg;
   char *tok;                    // stripping files to tokens
   fNumArg = checkArgv(argv,3);
   if ((strcmp(argv[2],"rows")==0) ||  (strcmp(argv[2],"cols")==0)) //if operation with two numeric arguments is called,
         sNumArg = checkArgv(argv,4);                               //second argument will be checked
   if (fNumArg == 0 || sNumArg==0)
         return;
   else if (((strcmp(argv[2],"rows")==0) ||  (strcmp(argv[2],"cols")==0)) && fNumArg>sNumArg){
         error("Wrong position of numeric arguments");
         return;
   }
   while (fgets (temp, 1024, stdin) !=0){
         tok = strtok(temp," ");
         row++;   
         while(tok!=NULL){
               col++; 
               /* checking operation, and position in table against arguments inserted by user */              
               if((((row >= fNumArg && row <= sNumArg) && (strcmp(argv[2],"rows")==0)) || ((col >= fNumArg && col <= sNumArg) && (strcmp(argv[2],"cols")==0))) 
                     || (((row == fNumArg) && (strcmp(argv[2],"row")==0)) || ((col == fNumArg) && (strcmp(argv[2],"col")==0)))){
                     selector(argv,tok,results);
               }
               tok = strtok(NULL," ");
         }                       //checking if selected col has data
         if (((col < sNumArg || sNumArg <= 0 || col < fNumArg || fNumArg <= 0) &&  (strcmp(argv[2],"cols")==0)) || ((col < fNumArg || fNumArg <= 0) &&  (strcmp(argv[2],"col")==0))){ 
               error("Error: No data in selected colum");
               return ;
         }
         col =0;
   }                             // checking if selected row has data
   if (((row < sNumArg || sNumArg <= 0 || row < fNumArg || fNumArg <= 0) && (strcmp(argv[2],"rows")==0)) || ((row < fNumArg || fNumArg <= 0) && (strcmp(argv[2],"row")==0))){ 
         if(row == 0)
               error("File is empty"); 
            else
               error("Error: No data in selected row");
            return ;
   }
   if(strcmp(argv[1],"select")!=0){
         operation(argv,results);
   }
}

int main (int argc, char *argv[]){
   
   if (argc == 1){
      error("No input parameters");
      return 0;
   }
   else if (argc == 2 && (strcmp(argv[1],"--help")==0)){   
      help();
      return 1;   
   }
   else if (argc < 4  || argc > 7) {
      error("Not enough or too many parameters inserted");
      return 0;
   }
   else if (strcmp(argv[2],"range")==0 && argc == 7){
      range(argv);
      return 1;
   }
  else if (argc == 4 && (strcmp(argv[2],"row")==0 || strcmp(argv[2],"col")==0)){
      rowsCols(argv);
      return 1;
   }      
   else if (argc == 5 && (strcmp(argv[2],"rows")==0 || strcmp(argv[2],"cols")==0)){ 
      rowsCols(argv);
      return 1;
   }
   else 
      error("Wrong parameters inserted");
   return 0;
}