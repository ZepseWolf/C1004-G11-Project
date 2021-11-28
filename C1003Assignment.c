#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define total_data_size 100
#define testing_data_size 20
#define training_data_size total_data_size-testing_data_size
#define feature_size 10
#define PI 3.141592


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
  double calculated_training[feature_size][5],
  double P_DiagnosisTotal
  //long double postp_train
);

void testFeature( 
  int classification , 
  int testing_size , 
  double testing_input[total_data_size][feature_size],
  double calculated_training_0[feature_size][5],
  double calculated_training_1[feature_size][5],
  double *total_error_count
);

double standardGaussianDis(
  double x, 
  double variance , 
  double mean
);

void errorCal(
  int real_classification, 
  double predicted_total_count_0 ,
  double predicted_total_count_1, 
  double *total_error_count
);

void main(){
    int i ,ii , total_training_count_0, total_training_count_1,total_testing_count_0,total_testing_count_1; 
    double* season_of_0; 
    // double input[total_data_size][feature_size];
    double raw_training_input[total_data_size][feature_size];
    double raw_testing_input[total_data_size][feature_size];
    double training_input_0[total_data_size][feature_size];
    double training_input_1[total_data_size][feature_size];
    double testing_input_0[total_data_size][feature_size];
    double testing_input_1[total_data_size][feature_size];
    double calculated_training_0[feature_size][5]; //To set Conditional Probility 
    double calculated_training_1[feature_size][5]; //To set Conditional Probility 
    double total_error_count =0 ;

    double trainingoutput[training_data_size][1];
    double P_Diagnosis[training_data_size][2];
    double P_DiagnosisTotal[2]; 
    double P_DiagnosisTotal_0=P_DiagnosisTotal[0];
    double P_DiagnosisTotal_1=P_DiagnosisTotal[1];
    long double postp_Diagnosis_normal_train[training_data_size][1];
    long double postp_Diagnosis_altered_train[training_data_size][1];

    FILE *file_ptr;

    file_ptr=fopen("fertility_Diagnosis_Data_Group9_11.txt","r");
    // file_ptr=fopen("sampleNB.txt","r");
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
    
    //lp
    // double trainingoutput[training_data_size][1]; 
    //get prior probability
    /*3)Assigned training output set from input*/
    for (i=0;i<=training_data_size-1;i++)
    {
            trainingoutput[i][0]=raw_training_input[i][feature_size];
    }
    /*Find the prior probability of Diagnosis normal 0 or altered 1 */
    // double P_Diagnosis[training_data_size][2];
    // double P_DiagnosisTotal[2]; 
    for (i=0;i<=training_data_size-1;i++)
    {
           if (trainingoutput[i][0]==1)
           {
                P_Diagnosis[i][1]=1; /*Diagnosis=altered=1   if outcome is 1 then 1*/ 
                P_Diagnosis[i][0]=0; 
           }
           else
           {
               P_Diagnosis[i][0]=1; /*Diagnosis=normal=0*/
               P_Diagnosis[i][1]=0;
           }        
    
    }
    P_DiagnosisTotal[0]=0;  //initialise as 0 for normal
    P_DiagnosisTotal[1]=0;  //initialise as 0 for altered
    for (i=0;i<=training_data_size-1;i++)
    {
        if (P_Diagnosis[i][0]==1)
        {
             P_DiagnosisTotal[0]+=1; /*Diagnosis=Normal=0   this is my P(Y=0) */
        }
        else
        {
            P_DiagnosisTotal[1]+=1; /*Diagnosis=altered=1   this is my P(Y=1)*/
        }
    }
    printf("\nprior probabilty diagnosis normal of training set: %lf\n", P_DiagnosisTotal[0]);
    double P_DiagnosisTotal_0=P_DiagnosisTotal[0];
    printf("\nprior probabilty diagnosis altered of training set: %lf\n", P_DiagnosisTotal[1]);
    double P_DiagnosisTotal_1=P_DiagnosisTotal[1];

    sortByClassification( training_data_size , &total_training_count_0, &total_training_count_1 , raw_training_input , training_input_0 , training_input_1); // Output will be count0 , count1 , training_input_0, training_input_1
    sortByClassification( testing_data_size , &total_testing_count_0, &total_testing_count_1 , raw_testing_input , testing_input_0 , testing_input_1); // Output will be count0 , count1 , training_input_0, training_input_1
    
    trainFeature(total_training_count_0 ,training_input_0,calculated_training_0 , P_DiagnosisTotal_0);//, postp_Diagnosis_normal_train
    trainFeature(total_training_count_1 ,training_input_1,calculated_training_1 , P_DiagnosisTotal_1);//, postp_Diagnosis_altered_train);

    printf("\n =========================Confusion Matrix Start=========================");
    testFeature(0, total_testing_count_0 , testing_input_0 , calculated_training_0,calculated_training_1,&total_error_count);
    testFeature(1, total_testing_count_1 , testing_input_1 , calculated_training_0,calculated_training_1,&total_error_count);
    printf("\n ==========================Confusion Matrix End =========================");
    printf("\n Total error     : %lf %%" ,total_error_count*100/testing_data_size);
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
  double calculated_training[feature_size][5],
  double P_DiagnosisTotal
  //long double postp_train
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

    //lp
    double P_training_f1_Diagnosis=0;
    
    for(i=0;i<training_size;i++){
      //Feature 1   
      if(training_input[i][0] == 1){  
        f1_count1++;
        // P_training_f1_Diagnosis = 
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

    //conditional probability
    f1_count1 /= P_DiagnosisTotal;
    f1_count2 /= P_DiagnosisTotal;
    f1_count3 /= P_DiagnosisTotal;
    f1_count4 /= P_DiagnosisTotal;
    f2_total /= P_DiagnosisTotal;
    f3_count1 /= P_DiagnosisTotal;
    f3_count2 /= P_DiagnosisTotal;
    f4_count1 /= P_DiagnosisTotal;
    f4_count2 /= P_DiagnosisTotal;
    f5_count1 /= P_DiagnosisTotal;
    f5_count2 /= P_DiagnosisTotal;
    f6_count1 /= P_DiagnosisTotal;
    f6_count2 /= P_DiagnosisTotal;
    f6_count3 /= P_DiagnosisTotal;
    f7_count1 /= P_DiagnosisTotal;
    f7_count2 /= P_DiagnosisTotal;
    f7_count3 /= P_DiagnosisTotal;
    f7_count4 /= P_DiagnosisTotal;
    f7_count5 /= P_DiagnosisTotal;
    f8_count1 /= P_DiagnosisTotal;
    f8_count2 /= P_DiagnosisTotal;
    f8_count3 /= P_DiagnosisTotal;
    f9_total /= P_DiagnosisTotal;

    for(i=0;i<training_size;i++){
      //Feature 1   
      if(training_input[i][0] == 1){  
        
        P_training_f1_Diagnosis = f1_count1;
      }
      else if(training_input[i][0] == 0.33){
        P_training_f1_Diagnosis = f1_count2;
      }
      else if(training_input[i][0] == -0.33){
        P_training_f1_Diagnosis = f1_count3;
      }
      else if(training_input[i][0] == -1){
        P_training_f1_Diagnosis = f1_count4;
      }
    }
    
    printf("\nP_training_f1_Diagnosis: %f", P_training_f1_Diagnosis);

    //posterior probability:
    // long double postp_Diagnosis_train=P_training_f1_Diagnosis*P_test_Maritial_Default_No*P_test_annualincome_Default_No*P_DiagnosisTotal;

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
  double *total_error_count
){
  int i;
  double total_0,total_1,predicted_total_count_0 =0,predicted_total_count_1=0;
  for(i = 0 ; i< testing_size; i++){ 
    // total_0 = 0;
    // total_1 = 0;
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
      printf("\n total_0: %lf \n", total_0);
     printf("\n total_1: %lf \n", total_1);

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

      //is the above total_0 and total_1 prior probabilty of normal and altered?

      //Feature 2 
      total_0 *= standardGaussianDis(testing_input[i][1],calculated_training_0[1][0] , calculated_training_0[1][1]);
      total_1 *= standardGaussianDis(testing_input[i][1],calculated_training_1[1][0] , calculated_training_1[1][1]);
      //Feature 9
      total_0 *= standardGaussianDis(testing_input[i][8],calculated_training_0[8][0], calculated_training_0[8][1]);
      total_1 *= standardGaussianDis(testing_input[i][8],calculated_training_1[8][0] , calculated_training_1[8][1]);
        
      if(total_0 >total_1){
        //Predicted 0
        predicted_total_count_0 ++ ;
      }else{
        //Predicted 1
        predicted_total_count_1 ++ ;
      }
    //end of loop
    
  }
  errorCal(classification,predicted_total_count_0,predicted_total_count_1, total_error_count);
  
}

double standardGaussianDis(double x, double variance , double mean){
  double cal = 0;
  cal = exp( ( pow( (x - mean)/variance,2))/-2)/sqrt(2*PI) ; 
  // printf("\n standard gaussian dis : %lf",cal);
  return cal;
}

void errorCal(int real_classification, double predicted_total_count_0 ,double predicted_total_count_1 ,double *total_error_count ){
  // static x ;
  // double c = ;
  
  if(real_classification == 0){
    *total_error_count += predicted_total_count_1;
    printf("\n True negatives  : %lf %%",predicted_total_count_0*100/testing_data_size);
    printf("\n False positives : %lf %%",predicted_total_count_1*100/testing_data_size);
  }
  else if(real_classification == 1){
    *total_error_count += predicted_total_count_0;
    printf("\n True positives  : %lf %%",predicted_total_count_1*100/testing_data_size);
    printf("\n False negatives : %lf %%",predicted_total_count_0*100/testing_data_size);
  }
  else{
    printf("\n No such classification exist!");
  }
};