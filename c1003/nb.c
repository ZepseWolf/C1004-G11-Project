
    /****************************************************************************************************************************************************************************/
    /*Read data from input file                                                                                                                                                 */
    /* Feature 1 :Season of analysis                           - Winter= -1 Spring= -0.33 Summer= 0.33 Fall= 1                                                                  */
    /* Feature 2 :Age of Analysis                              - Age 18 to 36 : 0 to 1                                                                                          */
    /* Feature 3 :Childish Disease                             - Yes=0, No=1                                                                                                    */
    /* Feature 4 :Accident or serious trauma                   - Yes=0, No=1                                                                                                    */
    /* Feature 5 :Surgical Intervention                        - Yes=0, No=1                                                                                                    */
    /* Feature 6 :High fevers in last year                     - Less than three months ago= -1 More than three months ago= 0 No= 1                                             */
    /* Feature 7 :Frequency of alcohol consumption             - Several times a day : 0.2 Every day : 0.4 Several times a week :0.6 Once a week :0.8 Hardly ever or never : 1  */
    /* Feature 8 :Smoking Habit                                - Never : -1 Occasional : 0 Daily : 1                                                                            */
    /* Feature 9 :Number of hours spent sitting per day        - Ene-16 ( 0 range to 1)                                                                                         */
    /* Outcome   :Semen Diagnosis                              - Altered=1, Normal=0                                                                                            */
    /****************************************************************************************************************************************************************************/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define total_data_size 100
// #define testing_data_size 20
// #define training_data_size 80
#define feature_size 10
#define PI 3.14159265


void main();
void sortByClassification(
  int totalnum,
  int *count0 ,
  int *count1 ,
  double raw_input[total_data_size][feature_size] , 
  double input_0[total_data_size][feature_size] , 
  double input_1[total_data_size][feature_size]
);

void trainFeature(
  int training_size,
  double training_input [total_data_size][feature_size] , 
  double calculated_training[feature_size][5]
);

void testFeature( 
  int classification , 
  int testing_size , 
  double testing_input[total_data_size][feature_size],
  double calculated_training_0[feature_size][5],
  double calculated_training_1[feature_size][5],
  double *total_error_count,
  double total_training_count_0, 
  double total_training_count_1,
  int set
);

double standardGaussianDis(
  double x, 
  double sd , 
  double mean
);

void errorCal(
  int real_classification, 
  double predicted_total_count_0 ,
  double predicted_total_count_1, 
  double *total_error_count,
  int set
);


int training_data_size, testing_data_size;

void main(){
    clock_t start,end;
    double cpu_time_used;
    int i ,ii , total_training_count_0=0, total_training_count_1=0,total_testing_count_0=0,total_testing_count_1=0; 
    double* season_of_0; 
    double raw_training_input[total_data_size][feature_size];
    double raw_testing_input[total_data_size][feature_size];
    double training_input_0[total_data_size][feature_size];
    double training_input_1[total_data_size][feature_size];
    double testing_input_0[total_data_size][feature_size];
    double testing_input_1[total_data_size][feature_size];
    double calculated_training_0[feature_size][5]; //To set Conditional Probility 
    double calculated_training_1[feature_size][5]; //To set Conditional Probility 
    double total_error_count =0 ;
    int trainingset=0, testingset=1;
    double summary[2][5];
    char *label[5]={"50/50","60/40","70/30","80/20","90/10"};
    start = clock();
    for(int counter = 0; counter < 5; counter++)
    { switch(counter)
      {
          case 0:
          training_data_size = 50;
          testing_data_size = 50;
          printf("\n\n    For 50/50");
          break;

          case 1:
          training_data_size = 60;
          testing_data_size = 40;
          printf("\n\n    For 60/40");
          break;

          case 2:
          training_data_size = 70;
          testing_data_size = 30;
          printf("\n\n    For 70/30");
          break;

          case 3:
          training_data_size = 80;
          testing_data_size = 20;
          printf("\n\n    For 80/20");
          break;

          case 4:
          training_data_size = 90;
          testing_data_size = 10;
          printf("\n\n    For 90/10");
          break;
      }
                                        
    FILE *file_ptr;
    file_ptr=fopen("fertility_Diagnosis_Data_Group9_11.txt","r");
    if (file_ptr==NULL)
    {
        printf("File could not be opened \n");
        exit(1);
    }
    for(i=0;i<training_data_size;i++)
    {   
        fscanf(file_ptr,"%lf, %lf, %lf , %lf , %lf , %lf , %lf , %lf , %lf , %lf " , &raw_training_input[i][0],&raw_training_input[i][1],&raw_training_input[i][2],&raw_training_input[i][3],&raw_training_input[i][4],&raw_training_input[i][5],&raw_training_input[i][6],&raw_training_input[i][7],&raw_training_input[i][8],&raw_training_input[i][9]); 
    }
    for(i=0; i<testing_data_size; i++)
    { 
       fscanf(file_ptr,"%lf, %lf, %lf , %lf , %lf , %lf , %lf , %lf , %lf , %lf " , &raw_testing_input[i][0],&raw_testing_input[i][1],&raw_testing_input[i][2],&raw_testing_input[i][3],&raw_testing_input[i][4],&raw_testing_input[i][5],&raw_testing_input[i][6],&raw_testing_input[i][7],&raw_testing_input[i][8],&raw_testing_input[i][9]); 
    }
    fclose(file_ptr);
    


    sortByClassification( training_data_size , &total_training_count_0, &total_training_count_1 , raw_training_input , training_input_0 , training_input_1); // Output will be count0 , count1 , training_input_0, training_input_1
    sortByClassification( testing_data_size , &total_testing_count_0, &total_testing_count_1 , raw_testing_input , testing_input_0 , testing_input_1); // Output will be count0 , count1 , training_input_0, training_input_1
    
    trainFeature(total_training_count_0 ,training_input_0,calculated_training_0 );
    trainFeature(total_training_count_1 ,training_input_1,calculated_training_1 );

    total_error_count = 0;//reset

    printf("\n =========================Confusion Matrix Start=========================");
    testFeature(0, total_testing_count_0 , testing_input_0 , calculated_training_0,calculated_training_1,&total_error_count,total_training_count_0, total_training_count_1, testingset);
    testFeature(1, total_testing_count_1 , testing_input_1 , calculated_training_0,calculated_training_1,&total_error_count,total_training_count_0, total_training_count_1, testingset);
    printf("\n ==========================Confusion Matrix End =========================");
    printf("\n Total error for testing set   : %lf %%" ,total_error_count*100/testing_data_size);
    summary[testingset][counter] = total_error_count*100/testing_data_size;
    total_error_count = 0; //reset 

    printf("\n\n =========================Confusion Matrix Start=========================");
    testFeature(0, total_training_count_0 , training_input_0 , calculated_training_0,calculated_training_1,&total_error_count,total_training_count_0, total_training_count_1, trainingset);
    testFeature(1, total_training_count_1 , training_input_1 , calculated_training_0,calculated_training_1,&total_error_count,total_training_count_0, total_training_count_1, trainingset);
    printf("\n ==========================Confusion Matrix End =========================");
    printf("\n Total error for training set   : %lf %%" ,total_error_count*100/training_data_size);
    summary[trainingset][counter]=total_error_count*100/training_data_size;
  }


    printf("\n\n\nCase \t\tTraining Error \t\tTesting Error");
    for(i=0;i<1;i++)
    {
      for(int j=0;j<5;j++)
      {
        printf("\n%s",label[j]);
        printf("\t\t %lf",summary[i][j]);
        printf("\t\t %lf",summary[i+1][j]);
      }
    }
  end = clock();
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  printf("\n\nTime taken: %lf",cpu_time_used);

}

void sortByClassification( int totalnum ,int *count0 ,int *count1 ,double raw_input[total_data_size][feature_size] ,double input_0[total_data_size][feature_size], double input_1[total_data_size][feature_size]){
    int  i = 0, ii =0;
    *count0 = 0 ;
    *count1 = 0 ;
     
    for (i = 0 ; i < totalnum; i++){
        if(raw_input[i][9] == 0){
            for( ii = 0 ; ii < feature_size ;ii++ ){
                input_0[*count0][ii] = raw_input[i][ii] ;
            //    printf("\n %lf , II = %d , counter =%d" ,input_0[i][count0] , ii , count0);
            }
            *count0 = *count0+1;
        }
        else if (raw_input[i][9] == 1){
            for( ii = 0 ; ii < feature_size ;ii++ ){
                input_1[*count1][ii] = raw_input[i][ii] ;
                //  printf("\n it worked %lf" ,raw_input[i][ii]);
            }
            *count1 = *count1+1;;
        }
    }
}

void trainFeature(
  int training_size,
  double training_input [total_data_size][feature_size],
  double calculated_training[feature_size][5]
  ){
    // Train features 1 , 3 , 4 , 5 , 6 , 7 , 8
    int i ,ii ;
    //Feature  1
    double f1_count1 = 0,f1_count2 = 0,f1_count3 = 0,f1_count4 = 0 ;
    //Feature  2  , Range
    double f2_total = 0, f2_average = 0,f2_variance = 0 ;
    //Feature  3
    double f3_count1 = 0,f3_count2 = 0 ;
    //Feature  4
    double f4_count1 = 0,f4_count2 = 0 ;
    //Feature  5
    double f5_count1 = 0,f5_count2 = 0 ;
    //Feature  6
    double f6_count1 = 0,f6_count2 = 0 ,f6_count3 = 0 ;
    //Feature  7
    double f7_count1 = 0,f7_count2 = 0 ,f7_count3 = 0 , f7_count4 = 0 , f7_count5 = 0 ;
    //Feature  8
    double f8_count1 = 0,f8_count2 = 0 , f8_count3 = 0 ;
    //Feature  9  , Range
    double f9_total = 0, f9_average = 0,f9_variance = 0 ;

    
    for(i=0;i<training_size;i++){
      //Feature 1   
      if(training_input[i][0] == 1){  
        f1_count1++;
      }
      else if(training_input[i][0] == 0.33){
        f1_count2++;
      }
      else if(training_input[i][0] == -0.33){
        f1_count3++;
      }
      else if(training_input[i][0] == -1){
        f1_count4++;
      }

      //Feature 3
      if(training_input[i][2] == 0){
        f3_count1++;
        // printf("\n it worked ?? %d" ,f1_count1);
      }
      else if(training_input[i][2] == 1){
        f3_count2++;
      }

      //Feature 4
      if(training_input[i][3] == 0){
        f4_count1++;
        // printf("\n it worked ?? %d" ,f1_count1);
      }
      else if(training_input[i][3] == 1){
        f4_count2++;
      }

      //Feature 5
      if(training_input[i][4] == 0){
        f5_count1++;
      }
      else if(training_input[i][4] == 1){
        f5_count2++;
      }

      //Feature 6
      if(training_input[i][5] == -1){
        f6_count1++;
      }
      else if(training_input[i][5] == 0){
        f6_count2++;
      }
      else if(training_input[i][5] == 1){
        f6_count3++;
      }

      //Feature 7
      if(training_input[i][6] == 0.2){
        f7_count1++;
        
      }
      else if(training_input[i][6] ==  0.4){
        f7_count2++;
      }
      else if(training_input[i][6] == 0.6){
        f7_count3++;
      }
      else if(training_input[i][6] == 0.8){
        f7_count4++;
      }
      else if(training_input[i][6] == 1){
        f7_count5++;
      }
      
      //Feature 8
      if(training_input[i][7] == -1){
        f8_count1++;
      }
      else if(training_input[i][7] ==  0){
        f8_count2++;
      }
      else if(training_input[i][7] == 1){
        f8_count3++;
      }

      //Feature 2 total sum
      f2_total += training_input[i][1];

      //Feature 9 total sum
      f9_total += training_input[i][8];
    }

    // End of loop 
    // Calculate Variance
    for(i=0;i<training_size;i++){
      f2_variance  += pow((training_input[i][1] - (f2_total/training_size)),2);
      f9_variance  += pow((training_input[i][8] - (f9_total/training_size)),2);
    }

    f2_variance = sqrt(f2_variance/(training_size-1));
    f9_variance = sqrt(f9_variance/(training_size-1));

    // printf("\n The sd of f2 =%lf  and f9 = %lf",f2_variance,f9_variance);   To remove soon
    
    calculated_training[0][0] = f1_count1/training_size; 
    calculated_training[0][1] = f1_count2/training_size;
    calculated_training[0][2] = f1_count3/training_size;
    calculated_training[0][3] = f1_count4/training_size; 

    calculated_training[1][0] = f2_variance;
    calculated_training[1][1] = f2_total/training_size;
    
    calculated_training[2][0] = f3_count1/training_size;
    calculated_training[2][1] = f3_count2/training_size;
    
    calculated_training[3][0] = f4_count1/training_size;
    calculated_training[3][1] = f4_count2/training_size;

    calculated_training[4][0] = f5_count1/training_size;
    calculated_training[4][1] = f5_count2/training_size;

    calculated_training[5][0] = f6_count1/training_size;
    calculated_training[5][1] = f6_count2/training_size;
    calculated_training[5][2] = f6_count3/training_size;

    calculated_training[6][0] = f7_count1/training_size;
    calculated_training[6][1] = f7_count2/training_size;
    calculated_training[6][2] = f7_count3/training_size;
    calculated_training[6][3] = f7_count4/training_size;
    calculated_training[6][4] = f7_count5/training_size;
     
    calculated_training[7][0] = f8_count1/training_size;
    calculated_training[7][1] = f8_count2/training_size;
    calculated_training[7][2] = f8_count3/training_size;

    calculated_training[8][0] = f9_variance;
    calculated_training[8][1] = f9_total/training_size;
    // printf("\n f2 average: %lf ", f2_total/training_size); To remove soon
    
}

void testFeature( 
  int classification , 
  int testing_size , 
  double testing_input[total_data_size][feature_size],
  double calculated_training_0[feature_size][5],
  double calculated_training_1[feature_size][5],
  double *total_error_count,
  double total_training_count_0, 
  double total_training_count_1,
  int set
){
  int i;
  double total_0,total_1,predicted_total_count_0 =0,predicted_total_count_1=0;
  for(i = 0 ; i< testing_size; i++){ 
    total_0 = 0;
    total_1 = 0;
      //Feature 1 
      if(testing_input[i][0] == 1){  
      total_0 = calculated_training_0[0][0];
      total_1 = calculated_training_1[0][0];
      }
      else if(testing_input[i][0] == 0.33){
      total_0 = calculated_training_0[0][1];
      total_1 = calculated_training_1[0][1];
      }
      else if(testing_input[i][0] == -0.33){
      total_0 = calculated_training_0[0][2];
      total_1 = calculated_training_1[0][2];
      }
      else if(testing_input[i][0] == -1){
        total_0 = calculated_training_0[0][3];
        total_1 = calculated_training_1[0][3];
      }

      //Feature 3
      if(testing_input[i][2] == 0){
        total_0 *= calculated_training_0[2][0];
        total_1 *= calculated_training_1[2][0];
      }
      else if(testing_input[i][2] == 1){
        total_0 *= calculated_training_0[2][1];
        total_1 *= calculated_training_1[2][1];
      }

      //Feature 4
      if(testing_input[i][3] == 0){
        total_0 *= calculated_training_0[3][0];
        total_1 *= calculated_training_1[3][0];
      }
      else if(testing_input[i][3] == 1){
        total_0 *= calculated_training_0[3][1];
        total_1 *= calculated_training_1[3][1];
      }

      //Feature 5
      if(testing_input[i][4] == 0){
        total_0 *= calculated_training_0[4][0];
        total_1 *= calculated_training_1[4][0];
      }
      else if(testing_input[i][4] == 1){
        total_0 *= calculated_training_0[4][1];
        total_1 *= calculated_training_1[4][1];
      }

      //Feature 6
      if(testing_input[i][5] == -1){
        total_0 *= calculated_training_0[5][0];
        total_1 *= calculated_training_1[5][0];
      }
      else if(testing_input[i][5] == 0){
        total_0 *= calculated_training_0[5][1];
        total_1 *= calculated_training_1[5][1];
      }
      else if(testing_input[i][5] == 1){
        total_0 *= calculated_training_0[5][2];
        total_1 *= calculated_training_1[5][2];
      }

      //Feature 7
      if(testing_input[i][6] == 0.2){
        total_0 *= calculated_training_0[6][0];
        total_1 *= calculated_training_1[6][0];
      }
      else if(testing_input[i][6] ==  0.4){
        total_0 *= calculated_training_0[6][1];
        total_1 *= calculated_training_1[6][1];
      }
      else if(testing_input[i][6] == 0.6){
        total_0 *= calculated_training_0[6][2];
        total_1 *= calculated_training_1[6][2];
      }
      else if(testing_input[i][6] == 0.8){
        total_0 *= calculated_training_0[6][3];
        total_1 *= calculated_training_1[6][3];
      }
      else if(testing_input[i][6] == 1){
        total_0 *= calculated_training_0[6][4];
        total_1 *= calculated_training_1[6][4];
      }
      
      //Feature 8
      if(testing_input[i][7] == -1){
        total_0 *= calculated_training_0[7][0];
        total_1 *= calculated_training_1[7][0];
      }
      else if(testing_input[i][7] ==  0){
        total_0 *= calculated_training_0[7][1];
        total_1 *= calculated_training_1[7][1];
      }
      else if(testing_input[i][7] == 1){
        total_0 *= calculated_training_0[7][2];
        total_1 *= calculated_training_1[7][2];
      }

      //Feature 2 
      total_0 *= standardGaussianDis(testing_input[i][1],calculated_training_0[1][0] , calculated_training_0[1][1]);
      total_1 *= standardGaussianDis(testing_input[i][1],calculated_training_1[1][0] , calculated_training_1[1][1]);
      //Feature 9
      total_0 *= standardGaussianDis(testing_input[i][8],calculated_training_0[8][0], calculated_training_0[8][1]);
      total_1 *= standardGaussianDis(testing_input[i][8],calculated_training_1[8][0] , calculated_training_1[8][1]);
      // Mutiply total prob
      total_0 *= (total_training_count_0/training_data_size);
      total_1 *= (total_training_count_1/training_data_size);
      if(total_0 >total_1){
        //Predicted 0
        predicted_total_count_0 ++ ;
      }else{
        //Predicted 1
        predicted_total_count_1 ++ ;
      }
    //end of loop
  }
  errorCal(classification,predicted_total_count_0,predicted_total_count_1, total_error_count, set);
}

double standardGaussianDis(double x, double sd , double mean){
  double cal = 0;
  double z=( x-mean)/sd;
  cal = expl( ( pow( (x - mean)/sd,2))/-2)/sqrt(2*PI) ; 
  // cal = 1/sqrt(2*PI*sd*sd)*expl(-0.5*pow(z,2));
  // printf("\n standard gaussian dis : %lf",cal);
  return cal;
}

void errorCal(int real_classification, double predicted_total_count_0 ,double predicted_total_count_1 ,double *total_error_count, int set){
  // static x ;
  // double c = ;
  double size;
  if(set==0)
  {
    size = training_data_size;
  }
  if(set==1)
  {
    size = testing_data_size;
  }

  if(real_classification == 0){
    *total_error_count += predicted_total_count_1;
    printf("\n True negatives  : %lf %%",predicted_total_count_0*100/size);
    printf("\n False positives : %lf %%",predicted_total_count_1*100/size);
  }
  else if(real_classification == 1){
    *total_error_count += predicted_total_count_0;
    printf("\n True positives  : %lf %%",predicted_total_count_1*100/size);
    printf("\n False negatives : %lf %%",predicted_total_count_0*100/size);
  }
  else{
    printf("\n No such classification exist!");
  }
};
