#include <stdio.h>
#include <stdlib.h>  //rand()
#include <time.h>    //time(NULL)

void print_array(int nums[],int n);
void get_user_inputs(int nums[],int n);
void find_smallest_largest_mostOccur_avg(int nums[],int n);

//mostOccur
void mostOccurence(int nums[],int n, int* mostOccur_num, int* mostOccur_count);
//quicksort
void swap(int* a, int* b);
int partition (int arr[], int low, int high);
void quickSort(int arr[], int low, int high);


int main()
{
  const int MAX_NUM = 10;
  int numbers[MAX_NUM];
  get_user_inputs(numbers,MAX_NUM);
  // // print_array(numbers,3);
  find_smallest_largest_mostOccur_avg(numbers,MAX_NUM);

}

void print_array(int nums[],int n)
{
  int i;
  printf("array: ");
  for(i=0; i<n; i++)
  {
    printf("%d\t",nums[i]);
  }
  printf("\n");
}

void get_user_inputs(int nums[],int n)
{
  int i;
  printf("You have to enter total %d numbers\n",n);
  for (i=0; i<n; i++)
  {
    printf("Enter num #%d: ",(i+1));
    scanf("%d",&nums[i]);
  }
}

void find_smallest_largest_mostOccur_avg(int nums[],int n)
{
  int i, smallest=nums[0], largest=nums[0], sum=nums[0];
  int mostOccur_num, mostOccur_count;
  float average;

  for (i=1; i<n; i++)
  {
    //for smallest
    if (nums[i]<smallest)
      smallest = nums[i];

    //for largest
    if (nums[i]>largest)
      largest = nums[i];

    //for average
    sum += nums[i];
  }

  //finding average
  average = sum/n;


  //most occuring
  mostOccurence(nums,n,&mostOccur_num,&mostOccur_count);

  //display results
  printf("\n\nResults:\n");
  printf("Smallest # : %d\n",smallest);
  printf("Largest #  : %d\n",largest);
  printf("Average  : %f\n",average);
  printf("Most occurred number: %d,   occurred %d times\n",mostOccur_num,mostOccur_count);

}

//mostOccur
void mostOccurence(int nums[],int n, int* mostOccur_num, int* mostOccur_count)
{
  int i,temp_count = 1;
  quickSort(nums,0,n-1);

  *mostOccur_num = nums[0];
  *mostOccur_count=1;

  for(i=1; i<n; i++)
  {
    if(nums[i] == nums[i-1])
    // number is same as previous number
    {
      if(nums[i] == *mostOccur_num)
      //number is the already selected most occuring number
      {
        *mostOccur_count = *mostOccur_count+1;
      }
      else
      // number is the temp number
      {
        temp_count++;

        if(temp_count>*mostOccur_count)
        // new most occurred number
        {
          *mostOccur_num = nums[i];
          *mostOccur_count = temp_count;
          temp_count = 1; //reset temp_count
        }
      }

    }

  }
}

//quicksort
void swap(int* a, int* b) 
{ 
    int t = *a; 
    *a = *b; 
    *b = t; 
} 

int partition (int arr[], int low, int high) 
{ 
    int pivot = arr[high];
    int i = (low - 1);
  
    for (int j = low; j <= high- 1; j++) 
    { 
        if (arr[j] <= pivot) 
        { 
            i++;
            swap(&arr[i], &arr[j]); 
        } 
    } 
    swap(&arr[i + 1], &arr[high]); 
    return (i + 1); 
}

void quickSort(int arr[], int low, int high) 
{ 
    if (low < high) 
    { 
        int pi = partition(arr, low, high); 
        quickSort(arr, low, pi - 1); 
        quickSort(arr, pi + 1, high); 
    } 
}




/*Good read
https://stackoverflow.com/questions/5087062/how-to-get-int-from-stdio-in-c
*/

/*Sources
quicksort: https://www.geeksforgeeks.org/quick-sort/
*/
