#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h> 

typedef struct Point {
    int x, y;
} Point;

int main() {
  // char filename[100];
  // printf("File name? ");
  // scanf("%s", filename);

  FILE* file = fopen("medium.csv", "r");

  // get the width and height
  char* first_line = (char*)(malloc(1000 * sizeof(char)));
  fscanf(file, "%s", first_line);
  int width = atoi(strtok(first_line, ","));
  int height = atoi(strtok(NULL, ","));

  printf("width: %d \n", width);
  printf("height: %d \n", height);

  // declare the 2D array
  // int data[height][width];
  int** data = (int**)malloc(height * sizeof(int*));
  for (int i = 0; i < height; i++) {
    data[i] = (int*)malloc(width * sizeof(int));
  }

  char* line = (char*)(malloc(10000 * sizeof(char)));
  int i = 0;
  while (fscanf(file, "%s", line) != EOF) {
      // printf("line %d: %s\n", i+1, line);
      char* elevation = strtok(line, ",");

      int j = 0;
      while (elevation != NULL) {
        data[i][j] = atoi(elevation);
        elevation = strtok(NULL, ",");
        j++;
      }
      i++;
  }

  // printf("2D array: \n");
  // for (int i = 0; i < height; i++) {
  //   for (int j = 0; j < width; j++) {
  //     printf("%d ", data[i][j]);
  //   }
  //   printf("\n");
  // }

  Point coord[width * height];
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      Point p;
      p.x = j;
      p.y = i;
      coord[j + i * width] = p;
      // printf("px: %d py: %d ", p.x, p.y);
    }
  }
  
  // printf("point: \n\n");
  // for (int i = 0; i < width * height; i++) {
  //     printf("px: %d py: %d\n", coord[i].x, coord[i].y);
  // }

  // find the peak and lowest point
  int min = data[0][0];
  int max = data[0][0];
  int max_index_x = 0;
  // int max_index_y = 0;
  int min_index_x = 0;
  // int min_index_y = 0;
  Point lowest_point;
  Point peak;

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
        if (data[i][j] > max) {
          max = data[i][j];
          max_index_x = j + i * width;
          // max_index_x = j;
          // max_index_y = i;
        }

        if (data[i][j] < min) {
          min = data[i][j];
          min_index_x = j + i * width;
          // min_index_x = j;
          // min_index_y = i;
        }
    }
  }
  lowest_point = coord[min_index_x];
  peak = coord[max_index_x];

  printf("max: %d\n", max);
  printf("max.x: %d\n", peak.x);
  printf("max.y: %d\n", peak.y);
  printf("min: %d\n", min);
  printf("min.x: %d\n", lowest_point.x);
  printf("min.y: %d\n", lowest_point.y);


  // finding the path
  Point path[width];
  int start = data[0][0];
  int start_row = 0;
  int col = 0;

  for (int row = 0; row < height; row++) {
    if (data[row][col] < start) {
      start = data[row][col];
      // data[row][col] = -1;
      start_row = row;
      path[col] = coord[col + row * width];
    }
  }
  printf("\nstart: %d\n", start);
  printf("start_index_y: %d\n", start_row);
  printf("path[0]: x = %d y = %d\n", path[col].x, path[col].y);

  int current_row = start_row;
  int forward;
  int forward_up;
  int forward_down;

  for (col = 0; col < width - 1; col++) {
    forward = abs(data[current_row][col] - data[current_row][col+1]);
    // printf("data[current_row][col]: %d, data[current_row][col+1]: %d\n", data[current_row][col], data[current_row][col+1]);
    // printf("forward: %d\n", forward);

    if (current_row < 1) {
      forward_up = INT_MAX;
    } else {
      forward_up = abs(data[current_row][col] - data[current_row-1][col+1]);
    //   printf("data[current_row][col]: %d, data[current_row-1][col+1]: %d\n", data[current_row][col], data[current_row-1][col+1]);
    //   printf("forward_up: %d\n", forward_up);
    }
    
    if (current_row >= height - 1) {
      forward_down = INT_MAX;
    } else {
      forward_down = abs(data[current_row][col] - data[current_row+1][col+1]);
    //   printf("data[current_row][col]: %d, data[current_row+1][col+1]: %d\n", data[current_row][col], data[current_row+1][col+1]);
    //   printf("forward_down: %d\n", forward_down);
    }
    
    data[current_row][col] = -10;

    // determine the path
    printf("col: %d\n",col);
    if(forward <= forward_up && forward <= forward_down) {
      printf("current_row: %d\n",current_row);
      path[col+1] = coord[col + 1 + current_row * width];
    //   printf("path[col+1]: x = %d y = %d\n", path[col+1].x, path[col+1].y);
      printf("end\n\n");
      // data[current_row][col+1] = -10;
    } else if(forward_up < forward && forward_up < forward_down) {
        // decrement the current row position
        current_row--;
        printf("current_row: %d\n",current_row);
        path[col+1] = coord[col + 1 + current_row * width];
        // printf("path[col+1]: x = %d y = %d\n", path[col+1].x, path[col+1].y);
        printf("end\n\n");
        // data[current_row-1][col+1] = -10;
    } else if(forward_down < forward && forward_down < forward_up) {
        // increment the current row position
        current_row++;
        printf("current_row: %d\n",current_row);
        path[col+1] = coord[col + 1 + current_row * width];
        // printf("path[col+1]: x = %d y = %d\n", path[col+1].x, path[col+1].y);
        printf("end\n\n");
        // data[current_row+1][col+1] = -10;
    } else if(forward_up == forward_down) {
        printf("current_row: %d\n",current_row);
        // printf("path[col+1]: x = %d y = %d\n", path[col+1].x, path[col+1].y);
        printf("end\n\n");
        srand(time(NULL));
        int random = rand() % 2;
        if(random == 0) {
          current_row--;
          // data[current_row-1][col+1] = -10;
        } else {
          current_row++;
          // data[current_row+1][col+1] = -10;
        }
    }
  }
//   printf("col: %d\n",col);
//   printf("current_row: %d\n",current_row);
  data[current_row][col] = -10;


  printf("Lowest Elevation Change Route: (%d, %d)", path[0].x, path[0].y);
  for (int i = 1; i < width; i++) {
    printf(", (%d, %d)", path[i].x, path[i].y);
  }

  // printf("\n\n2D array: \n");
  // for (int i = 0; i < height; i++) {
  //   for (int j = 0; j < width; j++) {
  //     printf("%d ", data[i][j]);
  //   }
  //   printf("\n");
  // }


  // graphical output

  FILE * file_output = fopen("mountain_graph_medium.ppm","w");
    fprintf(file_output, "P3\n");
    fprintf(file_output, "%d %d\n", width, height);
    fprintf(file_output, "%d\n", 255);
    printf("max:%d", max);
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
          if (j == peak.x && i == peak.y) {
            fprintf(file_output, "%d\n%d\n%d\n", 255, 0, 0);
          } else if (data[i][j] < 0) {
              fprintf(file_output, "%d\n%d\n%d\n", 0, 255, 0);
          } else {
              int gray = 255 * data[i][j] / (max - min);
              fprintf(file_output, "%d\n%d\n%d\n", gray, gray, gray);
          }
      }
    }
}