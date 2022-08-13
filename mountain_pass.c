// Mengmeng Yang, CS 131, Autumn 2020, Section 2084
// Programming Assignment #7, 11/23/2020
//
// The program will read in a CSV file of topographic (land elevation) data into a 2D array, 
// find the highest point, chart a low elevation change path through the land shown and output an image of the path

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h> 

// define the Point type that stores an x, y coordinate
typedef struct Point {
  int x, y;
} Point;

// scan the file passed in and store the two values on the first line
// as width and height
// paramters: file - file to read in; width - int pointer; height - int pointer
void get_w_and_h(FILE* file, int* width, int* height) {
  char* first_line = (char*)(malloc(100 * sizeof(char)));
    fscanf(file, "%s", first_line);
    *width = atoi(strtok(first_line, ","));
    *height = atoi(strtok(NULL, ","));
}

// continue to read the file and store the elevations in to the 2D array
// paramters: file - file to read in; width - int pointer; height - int pointer; data - 2D array
void get_data(FILE* file, int* width, int* height, int** data) {
  char* line = (char*)(malloc(10000 * sizeof(char)));
  int i = 0;
  while (fscanf(file, "%s", line) != EOF) {
      char* elevation = strtok(line, ",");

      int j = 0;
      while (elevation != NULL) {
        data[i][j] = atoi(elevation);
        elevation = strtok(NULL, ",");
        j++;
      }
      i++;
  }
}

// store the x, y coordinates iin to Point one by one
// paramters: width - int pointer; height - int pointer; coord - array of Point
void pointer(int width, int height, Point* coord) {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      Point p;
      p.x = j;
      p.y = i;
      coord[j + i * width] = p;
    }
  }
}

// find the peak and lowest point
// paramters: data - 2D array; width - int pointer; height - int pointer;
// peak - Point; lowest_point - Point; min - int pointer; max - int pointer
void find_peak_lowest(int** data, int width, int height, Point* peak, Point* lowest_point, int* min, int* max) {
  *min = data[0][0];
  *max = data[0][0];

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
        if (data[i][j] > *max) {
          *max = data[i][j];
          peak->x = j;
          peak->y = i;
        }

        if (data[i][j] < *min) {
          *min = data[i][j];
          lowest_point->x = j;
          lowest_point->y = i;
        }
    }
  }
  data[peak->y][peak->x] = -20;
}

// choose the right path
// paramters: data - 2D array; width - int pointer; height - int pointer;
// coord - array of Points; point - array of Points
void find_path(int** data, int width, int height, Point* coord, Point* path) {
  int start = data[0][0];
  int start_row = 0;
  int col = 0;

  for (int row = 0; row < height; row++) {
    if (data[row][col] < start) {
      start = data[row][col];
      start_row = row;
      path[col] = coord[col + row * width];
    }
  }

  int current_row = start_row;
  int forward;
  int forward_up;
  int forward_down;

  for (col = 0; col < width - 1; col++) {
    forward = abs(data[current_row][col] - data[current_row][col+1]);

    if (current_row < 1) {
      forward_up = INT_MAX;
    } else {
      forward_up = abs(data[current_row][col] - data[current_row-1][col+1]);
    }
    
    if (current_row >= height - 1) {
      forward_down = INT_MAX;
    } else {
      forward_down = abs(data[current_row][col] - data[current_row+1][col+1]);
    }
    
    data[current_row][col] = -10;

    // determine the path
    if(forward <= forward_up && forward <= forward_down) {
      path[col+1] = coord[col + 1 + current_row * width];
      // data[current_row][col+1] = -10;
    } else if(forward_up < forward && forward_up < forward_down) {
        // decrement the current row position
        current_row--;
        path[col+1] = coord[col + 1 + current_row * width];
    } else if(forward_down < forward && forward_down < forward_up) {
        // increment the current row position
        current_row++;
        path[col+1] = coord[col + 1 + current_row * width];
    } else if(forward_up == forward_down) {
      // "flip" the coin
        srand(time(NULL));
        int random = rand() % 2;
        if(random == 0) {
          current_row--;
        } else {
          current_row++;
        }
    }
  }
  // set the chosen step to a nagetive value
  data[current_row][col] = -10;

  // fencepost
  printf("Lowest Elevation Change Route: (%d, %d)", path[0].x, path[0].y);
  for (int i = 1; i < width; i++) {
    printf(", (%d, %d)", path[i].x, path[i].y);
  }
}

// output the ppm image to a file
// width - int pointer; height - int pointer;
// min - int pointer; max - int pointer; data - 2D array
void output(int width, int height, int* max, int* min, int** data) {
    FILE * file_output = fopen ("mountain_graph.ppm","w");
    fprintf(file_output, "P3\n");
    fprintf(file_output, "%d %d\n", width, height);
    fprintf(file_output, "%d\n", 255);
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
          if (data[i][j] == -20) {
            fprintf(file_output, "%d %d %d\n", 255, 0, 0);
          } else if (data[i][j] == -10) {
              fprintf(file_output, "%d %d %d\n", 0, 255, 0);
          } else {
              int gray = 255 * (data[i][j] - *min) / (*max - *min);
              fprintf(file_output, "%d %d %d\n", gray, gray, gray);
          }
      }
    }
}

void free_data(int** data, int height) {
  for (int i = 0; i < height; i++) {
    free(data[i]);
  }
  free(data);
  data = NULL;
}

int main() { 
  char filename[100]; 
  printf("File name? "); 
  scanf("%s", filename);
  FILE* file = fopen(filename, "r");

  // get the width and height
  int width;
  int height;
  get_w_and_h(file, &width, &height);

  // declare the 2D array
  int** data = (int**)malloc(height * sizeof(int*));
  for (int i = 0; i < height; i++) {
      data[i] = (int*)malloc(width * sizeof(int)); 
  }

  // store the values into the 2D array
  get_data(file, &width, &height, data);
    
  int min;
  int max;
  Point lowest_point;
  Point peak;

  Point coord[width * height];
  pointer(width, height, coord);

  // find the peak and the lowest point
  find_peak_lowest(data, width, height, &peak, &lowest_point, &min, &max);
  printf("Mountain Peak: (%d, %d)\n", peak.x, peak.y);
  printf("Lowest Point: (%d, %d)\n", lowest_point.x, lowest_point.y);

  // find the path
  Point path[width];
  find_path(data, width, height, coord, path);

  // graphical output
  output(width, height, &max, &min, data);

  free_data(data, height);
}