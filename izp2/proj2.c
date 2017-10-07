/*
 * File:    proj2.c
 * Date:    17.11.2014
 * Author:  Martin Krajňák, xkrajn02@stud.fit.vutbr.cz
 * Project: Iterační výpočty, projekt c. 1 pro predmet IZP
 * Desc.:   Software for tan function calculation. Calculation are made
 *          via taylor series and continued fraction. Software is providing
 *          comparison of both of calculation and calculation made by tan 
 *          function from math.h.
 *          It is also able to calculate distance from object and height
 *          of object if correct parameters are inserted.  
 * .data + .bss = staticke data 
 *     616+ 16 = 632
 *       12408
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define HELP "--help"
#define TAN "--tan"
#define C "-c"
#define M "-m"
#define ZERO 0
#define MAXANGLE 1.4
#define MAXITERATIONS 14

 void help(){
   printf("Software for tan function calculation. Calculations are made\n"
           "via taylor series and continued fraction. Software is comparing\n"
           "computed values with values returned from tan function defined \n"
           "in math.h.\n"
           "It is also able to calculate distance from object and height\n"
           "of object if correct parameters are inserted.\n"
           "Examples:\n"
                     "\t./proj2 --tan 1.024 6 10\n"
                     "\t./proj2 -m 0.3 0.9\n"
                     "\t./proj2 -c 1.7 -m 0.15 1.3\n"
            "Author: Martin Krajnak xkrajn02@stud.fit.vutbr.cz\n"   );
 }

void error(char *error){
   fprintf(stderr,"Error: %s\nType --help for help\n",error );
   return;
}

bool checkAngleInterval(double angle){
   if(angle > ZERO && angle <= MAXANGLE)
      return true;
   else{
      error("Angle is not from interval 0 < A <= 1.4 or unknown input");
      return false;
   }
}
//checking if arguments for --tan are correctly entered
bool checkBE(double begin, double end){
   if( begin <= end && begin > ZERO && end > ZERO && end < MAXITERATIONS)
      return true;
   else{
      error("Wrong interval of values, or undefined values inserted");
      return false;
   }
}
// parsing arguments to double, unwanted string check
double checkNumArgs(char **argv,int argPos){
   char *white;  
   double a = strtod(argv[argPos],&white);  
   if(strlen(white) != ZERO){ 
      fprintf(stderr,"Error: Argument error, unknown input:%s\n",white);
      return 0;
   }
   else  
      return a;   //no char detected, numeric value is returned 
}
//calculating absolute value of parameter
double myAbs(double num){
   return (num>=0) ? num : -num; 
}
int findInterval(double angle){
   int iterations = 3;
   double breaks[9] = {0,21e-3,106e-3,275e-3,520e-3,814e-3,1121e-3,1387e-3,14e-1};
   for (int i = 0; i < 8; ++i)
   {
      if(angle > breaks[i] && angle <= breaks[i+1]){
         break;
      }
      iterations++;
   }
   return iterations;
}
// tangens calcutated by taylor series
double taylor_tan(double angle, unsigned int n){ 
   long  int cit [13] = {1, 1, 2, 17, 62, 1382, 21844, 929569, 6404582,
                     443861162, 18888466084, 113927491862, 58870668456604 };  //numerators
   long int  men [13] = {1, 3, 15, 315, 2835, 155925, 6081075,                //denumerators
                     638512875,10854718875, 1856156927625, 
                     194896477400625, 49308808782358125,
                     3698160658676859375 };
   double tangens=0,power = angle*angle;
   for (unsigned int i = 0; i < n; ++i)
   {
      tangens += cit[i]*angle/men[i];
      angle *= power; 
   }
   return tangens;
}
// tangens calculated by continued fraction
double cfrac_tan(double angle, unsigned int n){
   double tangens = 0;
   double values = 2*(n+1)-3;    //variable for numbers in denumerator
   for (unsigned int i = n; i > 0; --i)
   {
      tangens = 1/((values/angle)-tangens);
      values -= 2;
   }
   return tangens;
}
/*compares tanges from taylor series and continued fraction with
* mathematical function privided by math.h library*/
void cmpTan(double angle, double begin, double end){
      double resTaylor;
      double resFrac;
      double resMath = tan(angle);
      for (int i = begin; i <= end; ++i)
      {  
         resTaylor = taylor_tan(angle,i);
         resFrac = cfrac_tan(angle,i);
         printf("%d %e %e %e %e %e\n",i,resMath,resTaylor,myAbs(resMath - resTaylor),
            resFrac,myAbs(resMath - resFrac));  
      }
      return;
}

double calcDistance(double angle,double deviceHeight){
   return deviceHeight / cfrac_tan(angle,findInterval(angle));
}

double calcHeight(double angle, double distance){
   return distance * cfrac_tan(angle,findInterval(angle));
}
//checking input argument
void checkArgs(int argc, char **argv){
      double deviceHeight = 1.5;                   // implicit value of deviceHeight
      double angleA,angleB,begin,end;              // variables for rest of inserted parameters
      if (argc == 2 && (strcmp(argv[1],HELP)==0)){ //help
            help();
            return;
      }
      else if (argc == 3 && strcmp(argv[1],M)==0 ){   //distance of object
            angleA = checkNumArgs(argv,2);
            if(!(checkAngleInterval(angleA)))
                  return;
            else
               printf("%.10e\n",calcDistance(angleA,deviceHeight) );   
      }
      else if (argc == 4 && strcmp(argv[1],M)==0 ){//distance + height of object aplha + beta angle
            angleA = checkNumArgs(argv,2);
            angleB = checkNumArgs(argv,3);
            if(!(checkAngleInterval(angleA)) || !(checkAngleInterval(angleB)))
                  return;
            else{
               double distance = calcDistance(angleA,deviceHeight); 
               printf("%.10e\n%.10e\n", distance, (calcHeight(angleB,distance) + deviceHeight) );
            }
      }
      else if (argc == 5 && strcmp(argv[1],C)==0 && strcmp(argv[3],M)==0 ){//distance + height of object alpha angle only
            angleA = checkNumArgs(argv,4);
            deviceHeight = checkNumArgs(argv,2);
            if(!(checkAngleInterval(angleA)))
                  return;
            else{
               printf("%.10e\n",calcDistance(angleA,deviceHeight));
            }
      }
      else if (argc == 5 && strcmp(argv[1],TAN)==0 ){//tangens comparison
            angleA = checkNumArgs(argv,2);
            begin = checkNumArgs(argv,3);
            end = checkNumArgs(argv,4);
            if(!(checkBE(begin,end)) ||  !(checkAngleInterval(angleA)))
                  return;
            else 
                  cmpTan(angleA,begin,end);
      }
      else if (argc == 6 && strcmp(argv[1],C)==0 && strcmp(argv[3],M)==0 ){//if optional height of device is inserted
            angleA = checkNumArgs(argv,4);
            angleB = checkNumArgs(argv,5);
            deviceHeight = checkNumArgs(argv,2);
            if(!(checkAngleInterval(angleA)) || !(checkAngleInterval(angleB)))
                  return;
            else{
               double distance = calcDistance(angleA,deviceHeight); 
               printf("%.10e\n%.10e\n",distance, (calcHeight(angleB,distance) + deviceHeight) );
            }
      }
      else{  
            error("Operation is not defined");
            return;
      }
}
int main (int argc, char **argv){
   checkArgs(argc,argv); 
   return 0;
}