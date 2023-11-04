
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h> 

#define MAX_POINTS 10
#define GRAV_CONST 7

typedef struct {
    float x;
    float y;
    float mass;
    float vx;
    float vy;
} Point;

Point points[MAX_POINTS];
int num_points;
int time1;

float forcesArrayX [MAX_POINTS][MAX_POINTS] = {0};
float forcesArrayY [MAX_POINTS][MAX_POINTS] = {0};

//pthread_mutex_t mutex;

// функция заполнения двумерного массива сил
void *fillRow(void *arg) {
    int row = *((int *)arg);
    for (int col = row; col < num_points; col++) {
        if (row != col)
        {
            // точка начальная - row
            // точка, на которую действуем - col
            
            float forceX = (GRAV_CONST * points[row].mass * points[col].mass * ( points[col].x - points[row].x )) / ( pow(sqrt( pow(points[row].x-points[col].x,2) + pow(points[row].y-points[col].y,2) ),3) );
            forcesArrayX[row][col] = forceX;
            forcesArrayX[col][row] = -forceX;
            
            float forceY = (GRAV_CONST * points[row].mass * points[col].mass * ( points[col].y - points[row].y )) / ( pow(sqrt( pow(points[row].x-points[col].x,2) + pow(points[row].y-points[col].y,2) ),3) );
            forcesArrayY[row][col] = forceY;
            forcesArrayY[col][row] = -forceY;
        }
    }
    pthread_exit(NULL);
}


int main() {
    pthread_t threads[MAX_POINTS];
    int row_numbers[MAX_POINTS];
    
    printf("Enter the number of points: ");
    scanf("%d", &num_points);
    
    printf("Enter the time in seconds: ");
    scanf("%d", &time1);
    
    for (int i = 0; i < num_points; i++) {
        printf("Enter the coordinates (x, y) for point %d: ", i + 1);
        scanf("%f %f", &points[i].x, &points[i].y);
        
        printf("Enter the mass for point %d: ", i + 1);
        scanf("%f", &points[i].mass);
        
        printf("Enter the velocities (Vx, Vy) for point %d: ", i + 1);
        scanf("%f %f", &points[i].vx, &points[i].vy);
    }
    printf("\n");
    printf("\t\t   (X, Y),\t (Vx, Vy)\n");
    
  //  pthread_mutex_init(&mutex, NULL);
  
  
   for (int t = 0; t < time1 ; ++t)
   {
    
        
            // составь тут двумерную таблицу (считаем действие каждой частицы на каждую)
            
        for (int i = 0; i < num_points; i++) {
            row_numbers[i] = i;
            if (pthread_create(&threads[i], NULL, fillRow, &row_numbers[i]) != 0) {
                perror("pthread_create");
                exit(1);
            }
        }
        
        // Wait for threads to finish
        for (int i = 0; i < num_points; i++) {
            pthread_join(threads[i], NULL);
        }
        
      //  pthread_mutex_destroy(&mutex);
      
      
         
        for (int i = 0; i < num_points; ++i)
        {
            // суммируем каждую силу forcesArray
            points[i].x += points[i].vx;
            points[i].y += points[i].vy;
            
            
            for (int j=0; j < num_points; ++j)
            {
                if (i != j)
                {
                    // или если избавиться от второго 2d массива: points[i].vx += forcesArrayX[i][j] * (points[j].x2 - points[i].x1);
                    points[i].vx += forcesArrayX[i][j];
                    points[i].vy += forcesArrayY[i][j];
                }
            }
            
    
            
           printf("Point %d at time %d: (%.2f, %.2f), (%.2f, %.2f)\n", i + 1, t + 1, points[i].x, points[i].y, points[i].vx, points[i].vy);
        }
        
        printf("\n");
        
   }
        
        
        
    //debug
    // printf("\n");
    
    // for (int i=0;i<num_points;++i)
    // {
    //     for (int j=0; j<num_points;++j)
    //     {
    //         printf("%d, ", forcesArrayX[i][j]);
    //     }
        
    //     printf("\n");
    // }
    
    
    printf("Final positions of the points:\n");
    for (int i = 0; i < num_points; i++) {
        printf("Point %d: (%.2f, %.2f)\n", i + 1, points[i].x, points[i].y);
    }
    
    return 0;
}